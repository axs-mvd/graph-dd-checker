#ifndef QUEUE_H_DD_CHECKER
#define VISTED_H_DD_CHECKER

#include "graph.h"

struct queue_element {
    node_id id;
    uint8_t distance;
    struct queue_element *next;
};

struct queue {
    struct queue_element *front;
    struct queue_element *last;
};

struct queue *create_queue();
void clear_queue(struct queue *q);
void remove_queue(struct queue *q);
void enqueue(struct queue *q, node_id i, uint8_t d);
void dequeue(struct queue *q, node_id *i, uint8_t *d);
int  empty(struct queue *q);

#endif

