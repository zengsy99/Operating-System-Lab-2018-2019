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
#include "test.h"
#include "stdio.h"
#include "sched.h"
#include "screen.h"
#include "common.h"
#include "syscall.h"

static void init_pcb()
{
  int i, j;
  for(i=0; i < num_lock_tasks; i++)
  {	
    for(j=0; j < 31; j++ )
    {
      pcb[i].kernel_context.regs[j] = 0;
      pcb[i].user_context.regs[j] = 0;
    }
    pcb[i].kernel_context.regs[31] = 0;
    pcb[i].user_context.regs[31] = lock_tasks[i] -> entry_point;

    pcb[i].user_context.regs[29] = STACK_TOP - i*STACK_SIZE;    

    pcb[i].kernel_context.cp0_status = 0;
    pcb[i].kernel_context.hi = 0;
    pcb[i].kernel_context.lo = 0;
    pcb[i].kernel_context.cp0_badvaddr = 0;
    pcb[i].kernel_context.cp0_cause = 0;
    pcb[i].kernel_context.cp0_epc = 0;

    pcb[i].user_context.cp0_status = 0;
    pcb[i].user_context.hi = 0;
    pcb[i].user_context.lo = 0;
    pcb[i].user_context.cp0_badvaddr = 0;
    pcb[i].user_context.cp0_cause = 0;
    pcb[i].user_context.cp0_epc = 0;

    pcb[i].user_stack_top = STACK_TOP - i*STACK_SIZE;

    pcb[i].prev = NULL;
    pcb[i].next = NULL;

    pcb[i].pid = i;

    pcb[i].type = lock_tasks[i] -> type;

/*    pcb[i].priority = FISRT_PRIORITY;
    queue_push(&First_ready_queue, &pcb[i]);*/

    pcb[i].status = TASK_READY;
  }

    pcb[0].priority = FISRT_PRIORITY;
    queue_push(&First_ready_queue, &pcb[0]);

    pcb[1].priority = THIRD_PRIORITY;
    queue_push(&Third_ready_queue, &pcb[1]);

    pcb[2].priority = SECOND_PRIORITY;
    queue_push(&Second_ready_queue, &pcb[2]);

  pcb[num_lock_tasks].pid = num_lock_tasks;
  pcb[num_lock_tasks].status = TASK_RUNNING;
  pcb[num_lock_tasks].priority = FISRT_PRIORITY;
  current_running = &pcb[num_lock_tasks];
}

static void init_exception_handler()
{
}

static void init_exception()
{
	// 1. Get CP0_STATUS
	// 2. Disable all interrupt
	// 3. Copy the level 2 exception handling code to 0x80000180
	// 4. reset CP0_COMPARE & CP0_COUNT register
}

static void init_syscall(void)
{
	// init system call table.
}

static void init_queue()
{
  queue_init(&First_ready_queue);
  queue_init(&Second_ready_queue);
  queue_init(&Third_ready_queue);
  queue_init(&Forth_ready_queue);
  queue_init(&Fifth_ready_queue);

  queue_init(&block_queue_1);
  queue_init(&block_queue_2);
  queue_init(&block_queue_3);
}
// jump from bootloader.
// The beginning of everything >_< ~~~~~~~~~~~~~~
void __attribute__((section(".entry_function"))) _start(void)
{
  // Close the cache, no longer refresh the cache 
  // when making the exception vector entry copy
  asm_start();

  // init interrupt (^_^)
  init_exception();
  printk("> [INIT] Interrupt processing initialization succeeded.\n");

  // init system call table (0_0)
  init_syscall();
  printk("> [INIT] System call initialized successfully.\n");

  init_queue();
  printk("> [INIT] Queue initialized successfully.\n");

  // init Process Control Block (-_-!)
  init_pcb();
  printk("> [INIT] PCB initialization succeeded.\n");

  // init screen (QAQ)
  init_screen();
  printk("> [INIT] SCREEN initialization succeeded.\n");

  // TODO Enable interrupt

  while (1)
  {
    do_scheduler();
  }

  return;
}
