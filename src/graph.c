#include "graph.h"

#include <stdlib.h>
#include <strings.h>

struct graph *create_graph(uint32_t nodes, uint8_t max_degree) {
    struct graph *g;
    uint32_t i;

    g = malloc(sizeof(struct graph));
    g->nodes = malloc(sizeof(struct node) * nodes);
    g->nodes_size = nodes;

    for (i = 0; i < nodes; i++) {
        g->nodes[i].adjacents = malloc(sizeof(node_id) * max_degree);
        g->nodes[i].adjacents_size = 0;
        bzero(g->nodes[i].adjacents, sizeof(node_id) * max_degree);
    }
    return g;
}

void remove_graph(struct graph * g) {
    uint32_t i;
    for (i = 0; i < g->nodes_size; i++) {
        free(g->nodes[i].adjacents);
    }
    free(g->nodes);
    free(g);
}

void add_edge(struct graph *g, node_id i, node_id j) {
    uint8_t temp;
    temp = g->nodes[i].adjacents_size;
    g->nodes[i].adjacents[temp] = j;
    g->nodes[i].adjacents_size++;
}

int exists_edge(struct graph *g, node_id i, node_id j) {
    uint8_t k;
    for (k = 0; k < g->nodes[i].adjacents_size; k++) {
        if (g->nodes[i].adjacents[k] == j) {
            return 1;
        }
    }
    return 0;
}

struct node *get_node(struct graph *g, node_id i) {
    return &(g->nodes[i]);
}

