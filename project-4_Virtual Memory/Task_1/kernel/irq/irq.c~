#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"

static void irq_timer()
{
  // TODO clock interrupt handler.
  // scheduler, time counter in here to do, emmmmmm maybe.
  time_elapsed += 100000;
  screen_reflush();
  init_timer();
  do_scheduler();
};

void interrupt_helper(uint32_t status, uint32_t cause)
{
  // TODO interrupt handler.
  // Leve3 exception Handler.
  // read CP0 register to analyze the type of interrupt.
  uint32_t int_type;
  int_type = (status & cause & 0xff00) >> 8;

  if(int_type == 0x80)
    irq_timer();
  else
  {
    vt100_move_cursor(1, 5);
    printk("UNKNOW INTERRUPT: STATUS = %x CAUSE = %x pid = %d, pc = %x", status, cause, process_id, current_running->user_context.regs[31]);
    while(1);
  }
}

void other_exception_handler()
{
    // TODO other exception handler
}
