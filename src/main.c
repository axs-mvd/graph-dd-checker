#include "graph.h"
#include "visited.h"
#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

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
            } else {
                /*printf("visited!\n");*/
            }
        }
    }
    return all_visited(v);
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

void test(char *file_name, uint8_t expected_diameter) {
    uint32_t order;
    uint16_t degree;
    node_id i; 
    struct graph *g;
    struct queue *q;
    struct visited *v;

    printf("file: %s expected_diameter: %d\n", file_name, expected_diameter);
    printf("computing order and max degree..\n");
    get_order_and_degree(file_name, &order, &degree);
    printf("order: %u, degree: %d\n", order, degree);
    
    printf("loading graph.. \n");
    g = create_graph(order, degree);
    load_graph_from_file(g, file_name);

    printf("check directionality.. \n");
    if (check_undirected(g)) {
        printf("\tgraph is undirected\n");
    } else {
        printf("\tgraph is directed\n");
        exit(1);
    }
   
    v = create_visited(order); 
    q = create_queue();

    for (i = 0; i < order; i++) {
        printf("walking from %u ", i);
        if (!walk(g, q, v, i, expected_diameter)) {
            printf(" [FAIL] could not reach all nodes in %d steps\n", expected_diameter);
            exit(1);
        }
        printf("[OK]\n");
    }

    remove_queue(q);
    remove_visited(v);
    remove_graph(g);

    printf("diameter is less or equal to %d\n", expected_diameter);
}

int main (int argc, char *argv[]) {
    char *file_name;
    uint8_t expected_diameter;
   
    if (argc != 3) {
        printf("usage: %s <file> <diameter>\n", argv[0]);
    } else {
        file_name = argv[1];
        expected_diameter = atoi(argv[2]);
        test(file_name, expected_diameter);
    }

    return 0;
}


