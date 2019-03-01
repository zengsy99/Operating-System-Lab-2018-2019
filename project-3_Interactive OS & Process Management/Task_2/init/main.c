/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */
#include "irq.h"
#include "lock.h"
#include "time.h"
#include "regs.h"
#include "test.h"
#include "stdio.h"
#include "queue.h"
#include "sched.h"
#include "screen.h"
#include "common.h"
#include "syscall.h"

uint32_t exception_handler[32];

static void init_pcb()
{
  int i;

  for(i = 0; i < NUM_MAX_TASK; i++)
  {
    pcb[i].status = TASK_EXITED;
    queue_init(&pcb[i].wait_queue);
    queue_init(&pcb[i].lock_queue);
  }

  give_pcb(&pcb[1], &task_shell, 1);

  pcb[0].user_context.regs[29] = STACK_TOP;    
  pcb[0].kernel_context.regs[29] = STACK_TOP - STACK_SIZE/2;
  pcb[0].user_stack_top = STACK_TOP;
  pcb[0].kernel_stack_top = STACK_TOP - STACK_SIZE/2;
  pcb[0].pid = 0;
  pcb[0].status = TASK_RUNNING;
  pcb[0].priority = FISRT_PRIORITY;
  current_running = &pcb[0];
}

static void init_exception_handler()
{
  int i = 0;
  for (i = 0; i < 32; i++) 
    exception_handler[i] = (uint32_t) handle_int;

  exception_handler[INT] = (uint32_t) handle_int;
  exception_handler[SYS] = (uint32_t) handle_syscall;
}

static void init_exception()
{
  // Get CP0_STATUS
  uint32_t cp0_status = get_cp0_status();

  // Disable all interrupt
  dis_interrupt();

  //initialize exception_handlers
  init_exception_handler();

  // Copy the level 2 exception handling code to 0x80000180
  memcpy(BEV0_EBASE+BEV0_OFFSET, exception_handler_begin, exception_handler_end-exception_handler_begin);

  // intialize CP0_STATUS & CP0_COUNT & CP0_COMPARE
  init_timer();
  init_cp0_status(STATUS_CU0 | cp0_status | 0x8001);
}

static void init_syscall(void)
{
  syscall[SYSCALL_SLEEP] = (int (*)()) &do_sleep;

  syscall[SYSCALL_BLOCK] = (int (*)()) &do_block;
  syscall[SYSCALL_UNBLOCK_ONE] = (int (*)()) &do_unblock_one;
  syscall[SYSCALL_UNBLOCK_ALL] = (int (*)()) &do_unblock_all;

  syscall[SYSCALL_WRITE] = (int (*)()) &screen_write;
  syscall[SYSCALL_REFLUSH] = (int (*)()) &screen_reflush;
  syscall[SYSCALL_CURSOR] = (int (*)()) &screen_move_cursor;
  
  syscall[SYSCALL_MUTEX_LOCK_INIT] = (int (*)()) &do_mutex_lock_init;
  syscall[SYSCALL_MUTEX_LOCK_ACQUIRE] = (int (*)()) &do_mutex_lock_acquire;
  syscall[SYSCALL_MUTEX_LOCK_RELEASE] = (int (*)()) &do_mutex_lock_release;
  
  syscall[SYSCALL_PS] = (int (*)()) &ps;
  syscall[SYSCALL_CLEAR] = (int (*)()) &screen_clear;
  
  syscall[SYSCALL_SPAWN] = (int (*)()) &do_spawn;
  syscall[SYSCALL_KILL] = (int (*)()) &do_kill;
  syscall[SYSCALL_EXIT] = (int (*)()) &do_exit;
  syscall[SYSCALL_WAIT] = (int (*)()) &do_wait;
}

static void init_queue()
{
  queue_init(&First_ready_queue);
  queue_init(&Second_ready_queue);
  queue_init(&Third_ready_queue);
  queue_init(&Forth_ready_queue);
  queue_init(&Fifth_ready_queue);

  queue_init(&sleep_queue);
}

void __attribute__((section(".entry_function"))) _start(void)
{
  // Close the cache, no longer refresh the cache 
  // when making the exception vector entry copy
  asm_start();

  int i = 0;
  time_elapsed = 0;
  for( ; i< SCREEN_HEIGHT * SCREEN_WIDTH; i++)
  {
    new_screen[i] = 0;
    old_screen[i] = 0;
  }

  // init system call table (0_0)
  init_syscall();
  printk("> [INIT] System call initialized successfully.\n");

  // init queue (@_@)
  init_queue();
  printk("> [INIT] Queue initialized successfully.\n");

  // init Process Control Block (-_-!)
  init_pcb();
  printk("> [INIT] PCB initialization succeeded.\n");

  // init screen (QAQ)
  init_screen();

  // init interrupt (^_^)
  init_exception();

  while (1)
  {
 //   do_scheduler();
  }

  return;
}
