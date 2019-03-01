#include "lock.h"
#include "sched.h"
#include "syscall.h"

void spin_lock_init(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void spin_lock_acquire(spin_lock_t *lock)
{
    while (LOCKED == lock->status)
    {
    };
    lock->status = LOCKED;
}

void spin_lock_release(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void do_mutex_lock_init(mutex_lock_t *lock, lid_t lid)
{
  lock->status = UNLOCKED;
  lock->lid = lid;
}

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
  if(lock->status == LOCKED)
  {
    if(lock->lid == 1)
      do_block(&block_queue_1);
    else if(lock->lid == 2)
      do_block(&block_queue_2);
    else if(lock->lid == 3)
      do_block(&block_queue_3);
    else
    {
      printk("UNKNOWN LID\n");
      while(1);
    }
  }
  else
  {
    lock->status = LOCKED;
    lock->pid = current_running->pid;
  }
}

void do_mutex_lock_release(mutex_lock_t *lock)
{
  pid_t pid;

  if(lock->lid == 1)
  {
    if(queue_is_empty(&block_queue_1))
      lock->status = UNLOCKED;
    else
    {
      do_unblock_one(&block_queue_1, &pid);
      lock->pid = pid;
    }
  }
  else if(lock->lid == 2)
  {
    if(queue_is_empty(&block_queue_2))
      lock->status = UNLOCKED;
    else
    {
      do_unblock_one(&block_queue_2, &pid);
      lock->pid = pid;
    }
  }
  else if(lock->lid == 3)
  {
    if(queue_is_empty(&block_queue_3))
      lock->status = UNLOCKED;
    else
    {
      do_unblock_one(&block_queue_3, &pid);
      lock->pid = pid;
    }
  }
  else
  {
    printk("UNKNOWN LID\n");
    while(1);
  }
}
