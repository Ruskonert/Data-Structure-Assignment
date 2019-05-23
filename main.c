#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "queue.h"
#include "attack_queue.h"

#define EMPLOYEES_LENGTH 4
#define ENGINEER_LANGTH 10


//
// 업무가 가장 적은 은행원이 작업을 맡깁니다.
// 이 일을 맡은 직원의 번호가 반환됩니다.
int insert_task(QueueType* employees, element customer)
{
    int employees_number = 0;
    int i = 0;
    int task_amount = MAX_QUEUE_SIZE;

    for(; i < EMPLOYEES_LENGTH; i++) {
        int task_amount_tmp = get_size(employees + i);
        if(task_amount_tmp < task_amount) {
            task_amount = task_amount_tmp;
            employees_number = i;
        }
    }
    enqueue(employees + employees_number, customer);
    return employees_number;
}

void progress_service(QueueType* employees, int *clock, int* total_wait, int is_unavailable_service) {
   int j = 0;
    for(int i = 0; i < EMPLOYEES_LENGTH; i++) {
        if(!is_empty(&(employees[i]))) {
           if(! is_unavailable_service)
           {
               element* last_task = peek(employees + i);
               if(last_task->service_time > 0) {
                  last_task->service_time = last_task->service_time - 1;
                  printf("직원 %d이(가) 고객 %d의 업무를 처리중입니다.\n", i, last_task->id);
               }
               else
               {
                  element customer = dequeue(employees + i);
                  printf("직원 %d 작업 완료 -> 고객 %d\n", i, customer.id);  
               }
           }

            else {
              printf("[위험] 직원 %d이(가) 사이버 공격으로 인해 업무 처리가 불가능합니다.\n", i);
           }

            for(j = employees[i].front + 1; j < employees[i].rear - 1; j++) {
               element customer = employees[i].data[j];
               if(customer.id == 0) continue;
               printf("직원 %d에 대기중인 고객 %d이 여전히 대기중입니다.\n", i, customer.id);
               *total_wait += 1;
            }
        }
    }
}

typedef struct _engineer {
   int task_amount;
   int time;
}Engineer;

int check_not_tasking(Engineer* engineer) {
   if(engineer->time == -1) {
      return 1;
   }
   else {
      return 0;
   }
}

void progress_blocking_attack(Engineer* engineers) {
   for(int i = 0; i < ENGINEER_LANGTH; i++) {
      Engineer* engineer = engineers + i;
      if(! check_not_tasking(engineer)) {
         if(engineer->time > 0) {
            engineer->time--;
         }
         else {
            printf("[방어 완료] 보안 엔니지어 %d이(가) 공격을 막고 작업을 완료하였습니다.\n", i);
            engineer->time--;
            engineer->task_amount++;
         }
      }
   }
}

int insert_task_for_cyber(Engineer* engineers, int progress_time) {
   for(int i = 0; i < ENGINEER_LANGTH; i++) {
      if(check_not_tasking(engineers + i)) {
         (engineers + i)->time = progress_time;
         return i;
      }
   }
   return 0;
}

void init_engineer(Engineer* e)
{
   e->task_amount = 0;
   e->time = -1;
}

void progress_attack_for_queue(Engineer* engineers, AttackServiceQueue* queue, int *total_time) {
   if(is_attack_empty(queue)) return;
   else {
      while(!is_attack_empty(queue)) {
         int attack = *attack_peek(queue);
         if(insert_task_for_cyber(engineers, attack) == 0) {
            printf("공격이 쌓였지만 여전히 처리중입니다.\n");
            return;
         }
         else {
            *total_time += attack;
            attack_dequeue(queue);
         }
      }
   }
}

int main(void) 
{
   int minutes = 6000;
   int total_wait = 0;
   int total_customers = 1;
   int service_of_employes = 0;
   int clock = 0;

   // 공격으로 인한 서비스 총 시간(대기 시간) 
   int total_attack_time = 0;

   // 보안 대응 서비스 시간
   int service_time = 0;

   // 총 공격을 받은 횟수
   int total_attack_amount = 0;

   // 공격받을 확률
   int attacked = 0;

   // 사이버 공격을 막는 멤버
   Engineer *engineers = (Engineer*)malloc(sizeof(Engineer)*ENGINEER_LANGTH);

   // 직원에 대한 정보 초기화
   QueueType *bank_employees = (QueueType*)malloc(sizeof(QueueType)*EMPLOYEES_LENGTH);

   // 더이상 공격을 막을 멤버가 없을 때, 공격에 대해서 임시로 저장하는 큐
   AttackServiceQueue *queue = (AttackServiceQueue*)malloc(sizeof(AttackServiceQueue));

   init_attack_queue(queue);

   for(clock = 0; clock < ENGINEER_LANGTH; clock++) {
       init_engineer(engineers + clock);
   }

   for(clock = 0; clock < EMPLOYEES_LENGTH; clock++) {
       init_queue(bank_employees + clock);
   }


   srand(time(NULL));
   for (clock = 0; clock < minutes; clock++) {
      attacked = (rand()%10);
      service_time = 10 + (rand() % 21);

      // 큐에 쌓인 공격이 있는지 확인합니다.
      progress_attack_for_queue(engineers, queue, &total_attack_time);

      // 공격 받은걸 해결하고 있습니다.
      progress_blocking_attack(engineers);

      // 사이버 공격을 받았을 경우 (60%)
      if(attacked < 7) {
         int engineer_number = insert_task_for_cyber(engineers, service_time);
         if(engineer_number != -1) {
            printf("[공격 받음] 공격을 받았습니다. %d번 엔지니어가 처리중입니다. 예상 처리 시간: %d분\n", engineer_number, service_time);
            total_attack_time += service_time;
         }
         else {
            printf("[공격 받음] 모든 엔니니어들이 일하고 있습니다! 공격으로 인한 서비스 지연이 불가피합니다.\n");
            attack_enqueue(queue, service_time);
         }
         total_attack_amount++;
      }

      // 시간이 경과하기 전에 일을 처리합니다.
      progress_service(bank_employees, &clock, &total_wait, !is_attack_empty(queue));

      // 시간이 경과합니다.
      printf("현재시각=%d\n", clock);

      // 손님이 들어올 확률은 40% 입니다.
      if((rand()%10) < 4)
      {
         element customer;
         customer.id = total_customers++;
         customer.arrival_time = clock;
         customer.service_time = rand() % 3 + 1;
         service_of_employes = insert_task(bank_employees, customer);
         printf("고객 %d이(가) %d분에 들어 옵니다. 직원 %d이(가) 해당 업무를 맡을 것입니다. 업무처리시간 = %d분\n",
            customer.id, customer.arrival_time, service_of_employes, customer.service_time);
      }
   }
   printf("전체 대기 시간 = %d분, 총 %d번의 공격을 당했습니다.\n또한, 평균 공격에 의한 대기 시간은: %d분이며, 여전히 남아 있는 공격 큐는 %d개입니다.\n", 
   total_wait, total_attack_amount, total_attack_time / total_attack_amount, get_attack_size(queue));
   for(int i = 0; i < ENGINEER_LANGTH; i++) {
      printf("%d번 엔지니어 방어 횟수 - %d번\n", i, (engineers + i)->task_amount);
   }
   return 0;
}