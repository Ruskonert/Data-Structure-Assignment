#ifndef __ATTACK_QUEUE
#define __ATTACK_QUEUE
#include <stdio.h>
#define MAX_ATTACK_SAVED_SIZE 255

typedef struct {
   int data[MAX_ATTACK_SAVED_SIZE];
   int front, rear;
} AttackServiceQueue;

void init_attack_queue(AttackServiceQueue *q);

int is_attack_empty(AttackServiceQueue *q);

void attack_enqueue(AttackServiceQueue *q, int item);

int attack_dequeue(AttackServiceQueue *q);

int* attack_peek(AttackServiceQueue *q);

#endif


