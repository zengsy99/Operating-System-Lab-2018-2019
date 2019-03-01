#include "time.h"
#include "test3.h"
#include "lock.h"
#include "sched.h"
#include "stdio.h"
#include "syscall.h"

static char blank[] = {"                                                "};

mutex_lock_t lock1;
mutex_lock_t lock2;

// pid = 2
void ready_to_exit_task()
{

}

// pid = 3
void wait_lock_task()
{

}

// pid = 4
void wait_exit_task()
{

}
