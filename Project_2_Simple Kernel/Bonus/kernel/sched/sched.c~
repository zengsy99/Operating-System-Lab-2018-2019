#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"

pcb_t pcb[NUM_MAX_TASK];

/* current running task PCB */
pcb_t *current_running;

/* global process id */
pid_t process_id = 1;

/* queue */
queue_t First_ready_queue;
queue_t Second_ready_queue;
queue_t Third_ready_queue;
queue_t Forth_ready_queue;
queue_t Fifth_ready_queue;

queue_t block_queue_1;
queue_t block_queue_2;
queue_t block_queue_3;

static void check_sleeping()
{
}

void switch_current_running()
{
  if(!queue_is_empty(&First_ready_queue))
    current_running = queue_dequeue(&First_ready_queue);
  else if(!queue_is_empty(&Second_ready_queue))
    current_running = queue_dequeue(&Second_ready_queue);
  else if(!queue_is_empty(&Third_ready_queue))
    current_running = queue_dequeue(&Third_ready_queue);
  else if(!queue_is_empty(&Forth_ready_queue))
    current_running = queue_dequeue(&Forth_ready_queue);
  else if(!queue_is_empty(&Fifth_ready_queue))
    current_running = queue_dequeue(&Fifth_ready_queue);
  else
  {
    printk("NO PROCESS TO RUN");
    while(1);
  }
  current_running->status = TASK_RUNNING;
  process_id = current_running->pid;
}
/*
void push_to_ready_queue(pcb_t *PCB)
{
  PCB->status = TASK_READY;
  if(PCB->priority == FISRT_PRIORITY)
    queue_push(&First_ready_queue, PCB);
  else if(PCB->priority == SECOND_PRIORITY)
    queue_push(&Second_ready_queue, PCB);
  else if(PCB->priority == THIRD_PRIORITY)
    queue_push(&Third_ready_queue, PCB);
  else if(PCB->priority == FORTH_PRIORITY)
    queue_push(&Forth_ready_queue, PCB);
  else
    queue_push(&Fifth_ready_queue, PCB);
}
*/

void push_to_ready_queue(pcb_t *PCB)
{
  PCB->status = TASK_READY;
  if(PCB->priority == FISRT_PRIORITY)
  {
    queue_push(&Second_ready_queue, PCB);
    PCB->priority = SECOND_PRIORITY;
  }
  else if(PCB->priority == SECOND_PRIORITY)
  {
    queue_push(&Third_ready_queue, PCB);
    PCB->priority = THIRD_PRIORITY;
  }
  else if(PCB->priority == THIRD_PRIORITY)
  {
    queue_push(&Forth_ready_queue, PCB);
    PCB->priority = FORTH_PRIORITY;
  }
  else if(PCB->priority == FORTH_PRIORITY)
  {
    queue_push(&Fifth_ready_queue, PCB);
    PCB->priority = FIFTH_PRIORITY;
  }
  else
    queue_push(&Fifth_ready_queue, PCB);
}

void scheduler(void)
{

  if(current_running == NULL || current_running->status != TASK_RUNNING)
    switch_current_running();
  else
  {
    push_to_ready_queue(current_running);
    switch_current_running();
  }
}

void do_sleep(uint32_t sleep_time)
{
    // TODO sleep(seconds)
}

void do_block(queue_t *queue)
{
  // block the current_running task into the queue
  queue_push(queue, current_running);
  current_running->status = TASK_BLOCKED;
  do_scheduler();
}

void do_unblock_one(queue_t *queue, pid_t *pid)
{
  // unblock the head task from the queue
  pcb_t *unblocked;
  pcb_t *next = queue->head;
  priority_t priority_temp = 5;

  do{
    if(next->priority < priority_temp)
    {
      unblocked = next;
      priority_temp = unblocked->priority;
    }

    next = next->next;
  } while(next != NULL);

  queue_remove(queue, unblocked);
  push_to_ready_queue(unblocked);

  *pid = unblocked->pid;
}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
}
