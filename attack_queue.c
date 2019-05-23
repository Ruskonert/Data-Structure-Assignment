#include "attack_queue.h"
#include "queue.h"

int get_attack_size(AttackServiceQueue* q) {
   int a = (q->front);
   int b = (q->rear);
   return b - a;
}

void init_attack_queue(AttackServiceQueue *q) {
    q->front = 0;
    q->rear = 0;
}

int is_attack_empty(AttackServiceQueue *q) {
   return (q->front == q->rear);
}

void attack_enqueue(AttackServiceQueue *q, int item) {
   if (is_full(q)) error("큐가 포화상태입니다.");
   q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
   q->data[q->rear] = item;
}

int attack_dequeue(AttackServiceQueue *q) {
   if (is_empty(q)) error("큐가 공백상태입니다.");
   q->front = (q->front +1) % MAX_QUEUE_SIZE;
   return q->data[q->front];
}

int* attack_peek(AttackServiceQueue *q) {
   if(is_empty(q)) error("큐가 공백상태입니다.");
   return &(q->data[(q->front + 1) % MAX_QUEUE_SIZE]);
}