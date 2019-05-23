#include "queue.h"


int get_size(QueueType* q) {
   int a = (q->front);
   int b = (q->rear);
   return b - a;
}

void error(char *message) {
   fprintf(stderr, "%s\n", message);
   exit(1);
}

void init_queue(QueueType *q) {
    q->front = 0;
    q->rear = 0;
}

int is_empty(QueueType *q) {
   return (q->front == q->rear);
}

int is_full(QueueType *q) {
   return ((q->rear +1) % MAX_QUEUE_SIZE == q->front);
}


void enqueue(QueueType *q, element item) {
   if (is_full(q)) error("큐가 포화상태입니다.");
   q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
   q->data[q->rear] = item;
}

element dequeue(QueueType *q) {
   if (is_empty(q)) error("큐가 공백상태입니다.");
   q->front = (q->front +1) % MAX_QUEUE_SIZE;
   return q->data[q->front];
}

element* peek(QueueType *q) {
   if(is_empty(q)) error("큐가 공백상태입니다.");
   return &(q->data[(q->front + 1) % MAX_QUEUE_SIZE]);
}