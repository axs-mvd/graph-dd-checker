#ifndef GRAPH_H_DD_CHECKER
#define GRAPH_H_DD_CHECKER

#include <stdint.h>

typedef uint32_t node_id;

struct node{
    node_id *adjacents;
    uint8_t adjacents_size;
};

struct graph{
    struct node *nodes;
    uint32_t nodes_size;
};

struct graph *create_graph(uint32_t nodes, uint8_t max_degree);
void remove_graph(struct graph *g);

void add_edge(struct graph *g, node_id i, node_id j);
int exists_edge(struct graph *g, node_id i, node_id j); 
struct node *get_node(struct graph *g, node_id i);



#endif
