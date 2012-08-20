#include "queue.h"
#include <stdlib.h>


struct queue* create_queue() {
    struct queue *q;
    q = malloc(sizeof(struct queue));
    q->front = 0;
    q->last  = 0;
    return q;
}

void clear_queue(struct queue *q) {
    struct queue_element *actual, *last;
    actual = q->front;
    while (actual != 0) {
        last = actual;
        actual = actual->next;    
        free(last);
    }
    q->front = 0;
    q->last  = 0;
}

void remove_queue(struct queue *q) {
    clear_queue(q);
    free(q);
}

void enqueue(struct queue *q, node_id i, uint8_t d) {
    struct queue_element *new_elem;

    new_elem = malloc(sizeof(struct queue_element));
    new_elem->id = i; 
    new_elem->distance = d; 
    new_elem->next = 0; 

    if (q->last != 0) {
        q->last->next = new_elem;
    }

    if (q->front== 0) {
        q->front = new_elem;
    }

    q->last = new_elem;
}

void dequeue(struct queue *q, node_id *i, uint8_t *d) {
    struct queue_element *e;
    if (q->front != 0) {
        (*i) = q->front->id;
        (*d) = q->front->distance;
        e = q->front;
        q->front = q->front->next;

        if (e->next == 0) {
            q->last = 0;
        }

        free(e);
    } else {
        /* do something */
    }
}

int empty(struct queue *q) {
    return q->front == 0;
}

