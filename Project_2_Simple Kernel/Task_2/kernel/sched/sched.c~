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

static void check_sleeping()
{
}

void scheduler(void)
{
  if(current_running == NULL || current_running->status != TASK_RUNNING)
  {
    current_running = queue_dequeue(&ready_queue);
    current_running->status = TASK_RUNNING;
    process_id = current_running->pid;
  }
  else
  {
    queue_push(&ready_queue, current_running);
    current_running->status = TASK_READY;
    current_running = queue_dequeue(&ready_queue);
    current_running->status = TASK_RUNNING;
    process_id = current_running->pid;
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

void do_unblock_one(queue_t *queue)
{
  // unblock the head task from the queue
  pcb_t *unblocked;
  unblocked = queue_dequeue(queue);
  queue_push(&ready_queue, unblocked);
  unblocked->status = TASK_READY;
}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
}
