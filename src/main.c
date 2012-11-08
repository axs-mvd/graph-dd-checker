#include "graph.h"
#include "visited.h"
#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct walk_thread_parameters {
    struct graph   *g;
    struct queue   *q;
    struct visited *v;
    node_id i;
    uint8_t max_distance;
};

/*=================================================*/
uint8_t walk(struct graph *g, struct queue *q, struct visited *v, 
        node_id i, uint8_t max_distance) {
    struct node *node;
    node_id adj_idx;
    node_id adj;
    uint8_t current_distance;
    node_id current_node;
    clear_queue(q);
    clear_visited(v);

    enqueue(q, i, 0);
    mark_visited(v, i);
    while (!empty(q)) {
        dequeue(q, &current_node, &current_distance);
        if (current_distance > max_distance) {
            return 0;
        }
        node = get_node(g, current_node);
        for (adj_idx = 0; adj_idx < node->adjacents_size; adj_idx++) {
            adj = node->adjacents[adj_idx];
            if (!is_visited(v, adj) && current_distance +1 <= max_distance) {
                mark_visited(v, adj);
                enqueue(q, adj, current_distance +1);
            }
        }
    }
    return all_visited(v);
}

/*=================================================*/
void *walk_thread(void *args) {
    struct walk_thread_parameters *param; 
    uint8_t *ret;

    param = (struct walk_thread_parameters *)args;

    ret = malloc(sizeof(uint8_t));

    *ret = walk(param->g,
             param->q,
             param->v,
             param->i,
             param->max_distance);

    pthread_exit((void *) ret);  
    return 0;
}

/*=================================================*/
int check_undirected(struct graph *g) {
    node_id i;
    node_id j;
    uint16_t k;

    struct node *n;

    for (i = 0; i < g->nodes_size; i++) {
        n = get_node(g, i);
        for (k = 0; k < n->adjacents_size; k++) {
            j = n->adjacents[k];
            if (!exists_edge(g, j, i)) {
                return 0;
            }
        }
    }

    return 1;
}
/*=================================================*/
void get_order_and_degree(char *file_name, uint32_t *order, uint16_t *degree) {
    FILE *file; 
    char line[1024*10], *p, *e;
    uint16_t current_degree;
    (*order) = 0;
    (*degree) = 0;

    file = fopen(file_name, "r");
    if (file == 0) {
        printf("could not open file: %s\n", file_name);
        exit(1);
    }
    
    while (fgets(line, sizeof(line), file)) {
        p = line;
        
        current_degree = 0; 
        for (p = line; ; p = e) {
            strtol(p, &e, 10);
            if (p == e)
                break;
            current_degree++;
        }
        if ((*degree) < current_degree) {
            (*degree) = current_degree;
        }
        (*order)++;
    } 
    fclose(file);
}

/*=================================================*/
void load_graph_from_file(struct graph *g, char *file_name) {
    FILE *file; 
    char line[1024*10], *p, *e;
    node_id i,j;

    file = fopen(file_name, "r");
    if (file == 0) {
        printf("could not open file: %s\n", file_name);
        exit(1);
    }

    i = 0;
    j = 0;
    
    while (fgets(line, sizeof(line), file)) {
        p = line;
        
        for (p = line; ; p = e) {
            j = strtol(p, &e, 10);
            if (p == e)
                break;

            add_edge(g, i, j);
        }
        i++;
    } 
    fclose(file);
}

void test(char *file_name, uint8_t expected_diameter, uint8_t thread_count) {
    uint32_t order;
    uint16_t degree;
    node_id i; 
    uint32_t idx;
    struct graph *g;
    pthread_t *thread_array;
    struct walk_thread_parameters *thread_parameters_array;
    uint8_t *ret;
  
    printf("file: %s expected_diameter: %d threads: %d\n", file_name, expected_diameter, thread_count);
    printf("computing order and max degree..\n");
    get_order_and_degree(file_name, &order, &degree);
    printf("order: %u, degree: %d\n", order, degree);
    
    printf("loading graph.. \n");
    g= create_graph(order, degree);
    load_graph_from_file(g, file_name);

    printf("check directionality.. \n");
    if (check_undirected(g)) {
        printf("\tgraph is undirected\n");
    } else {
        printf("\tgraph is directed\n");
        exit(1);
    }
    thread_parameters_array = malloc(sizeof(struct walk_thread_parameters) * thread_count); 
    thread_array  = malloc(sizeof(pthread_t       ) * thread_count);
    
    for (idx = 0; idx  < thread_count; idx++) {
        thread_parameters_array[idx].v = create_visited(order);
        thread_parameters_array[idx].q = create_queue(order);
        thread_parameters_array[idx].g = g;
        thread_parameters_array[idx].max_distance = expected_diameter;
    }   

    i = 0;
    while (i < order) {
        for (idx = 0; idx < thread_count && i + idx < order; idx++) {
            thread_parameters_array[idx].i = i + idx;
            if (pthread_create(&(thread_array[idx]), 0, walk_thread, (void *)&(thread_parameters_array[idx])) != 0) {
                printf("[FAIL] could not create thread, aborting!\n");
                exit(1);
            }
        }
        for (idx = 0; idx < thread_count && i + idx < order; idx++) {
            if (pthread_join(thread_array[idx], (void *)&ret) != 0) {
                printf("[FAIL] could not join thread, aborting!\n");
                exit(1);
            }
            if (*ret != 1) {
                printf(" [FAIL] could not reach all nodes in %d steps\n", expected_diameter);
                exit(1);
            }
            printf("[OK] walking from %u\n", i + idx);
            free(ret);
        }
        i+=thread_count;
    }

    for (idx = 0; idx  < thread_count; idx++) {
        remove_visited(thread_parameters_array[idx].v);
        remove_queue(thread_parameters_array[idx].q);
    }   
    free(thread_parameters_array);
    free(thread_array);

    remove_graph(g);

    printf("diameter is less or equal to %d\n", expected_diameter);
    fflush(stdout);
}

int main (int argc, char *argv[]) {
    char *file_name;
    uint8_t expected_diameter;
    uint8_t threads_count;
   
    if (argc != 4) {
        printf("usage: %s <file> <diameter> <threads>\n", argv[0]);
    } else {
        file_name = argv[1];
        expected_diameter = atoi(argv[2]);
        threads_count = atoi(argv[3]);

        test(file_name, expected_diameter, threads_count);
    }

    return 0;
}


