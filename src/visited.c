#include "visited.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

struct visited *create_visited(uint32_t size) {
    struct visited *v;
    v = malloc(sizeof(struct visited));
    v->data_size = size / 8 * sizeof(uint32_t) + 1;
    v->size = size;
    v->data = malloc(v->data_size * sizeof(uint32_t));
    bzero(v->data, v->data_size * sizeof(uint32_t));
    return v;
}

void remove_visited(struct visited *v) {
    free(v->data);
    free(v);
}

void clear_visited(struct visited *v) {
    bzero(v->data, v->data_size * sizeof(uint32_t));
}

void set_in_a_uint32_t(uint32_t *d, uint16_t pos) {
    uint32_t temp;
    temp = 1;
    (*d) |= (temp << pos);
}

int get_of_a_uint32_t(uint32_t *d, uint16_t pos) {
    uint32_t temp;
    temp = 1;
    return (*d) & (temp << pos);
}

void mark_visited(struct visited *v, node_id i) {
    uint32_t array_position; 
    uint16_t in_position; 
    array_position = i / (sizeof(uint32_t)*8);
    in_position    = i % (sizeof(uint32_t)*8);
 
    set_in_a_uint32_t(&(v->data[array_position]), in_position);
}
int is_visited(struct visited *v, node_id i) {
    uint32_t array_position;
    uint16_t in_position;

    array_position = i / (sizeof(uint32_t)*8);
    in_position    = i % (sizeof(uint32_t)*8);
    return get_of_a_uint32_t(&(v->data[array_position]), in_position);
}

int all_visited(struct visited *v) {
    uint32_t i;
    uint8_t r;
    r = 1;
    for (i = 0; i < v->size; i++) {
        if (!is_visited(v, i)) {
            printf("%u, is not visited\n", i);
            /*return 0;*/
            r = 0;
        }
    }
    /*return 1;*/
    return r;
}

