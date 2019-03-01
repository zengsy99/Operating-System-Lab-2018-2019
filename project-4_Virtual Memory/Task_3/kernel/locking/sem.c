#include "sem.h"
#include "stdio.h"
#include "sched.h"

void do_semaphore_init(semaphore_t *s, int val)
{
  s->value = val;
  queue_init( &(s->block_queue) );
}

void do_semaphore_up(semaphore_t *s)
{
  s->value++;
  // wake up
  do_unblock_one( &(s->block_queue) );
}

void do_semaphore_down(semaphore_t *s)
{
  while(s->value <= 0)
    do_block( &(s->block_queue) );

  s->value--;
}
