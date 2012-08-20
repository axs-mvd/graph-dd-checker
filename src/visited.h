#ifndef VISTED_H_DD_CHECKER
#define VISTED_H_DD_CHECKER
#include "graph.h"

struct visited {
    uint32_t *data;
    uint16_t data_size;
    uint32_t size;
};

struct visited * create_visited(uint32_t size);
void remove_visited(struct visited *v);
void clear_visited(struct visited *v);

void mark_visited(struct visited *v, node_id i);
int is_visited(struct visited *v, node_id i);
int all_visited(struct visited *v);

#endif
