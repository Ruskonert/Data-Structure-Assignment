#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>

#define MAX_QUEUE_SIZE 7

typedef struct _element {
   int id;
   int arrival_time;
   int service_time;
} element;

typedef struct {
   element data[MAX_QUEUE_SIZE];
   int front, rear;
} QueueType;

int get_size(QueueType* q);

void error(char *message);

void init_queue(QueueType *q);

int is_empty(QueueType *q);

int is_full(QueueType *q);

void enqueue(QueueType *q, element item);

element dequeue(QueueType *q);

element* peek(QueueType *q);

#endif