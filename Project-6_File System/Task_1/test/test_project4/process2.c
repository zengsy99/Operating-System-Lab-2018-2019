#include "sched.h"
#include "stdio.h"
#include "syscall.h"
#include "time.h"
#include "test4.h"
#include "screen.h"

#define RW_TIMES 2

int rand()
{
  int current_time = get_timer();
  return current_time;
}

static void disable_interrupt()
{
    uint32_t cp0_status = get_cp0_status();
    cp0_status &= 0xfffffffe;
    set_cp0_status(cp0_status);
}

static void enable_interrupt()
{
    uint32_t cp0_status = get_cp0_status();
    cp0_status |= 0x01;
    set_cp0_status(cp0_status);
}

static char read_uart_ch(void)
{
    char ch = 0;
    unsigned char *read_port = (unsigned char *)(0xbfe48000 + 0x00);
    unsigned char *stat_port = (unsigned char *)(0xbfe48000 + 0x05);

    while ((*stat_port & 0x01))
    {
        ch = *read_port;
    }
    return ch;
}

static void scanf(int *mem)
{
  int index = 0, num = 0;
  char buffer[16];
  char ch;
  static x = 4;

  disable_interrupt();

  vt100_move_cursor(1, x%4 +1);
  printk("                       ");
  vt100_move_cursor(1, x%4 +1);
  printk("Enter  ADDR:");
  x++;

  while(1)
  {
    do{
      ch = read_uart_ch();
      printk("%c", ch);
    } while(ch == 0);

    buffer[index ++] = ch;

    if(ch == 13)
      break;
  }

  index--;

  for(index = 2; buffer[index] != 13; index ++)
  {
    if(buffer[index] >= '0' && buffer[index] <= '9')
      num = (buffer[index] - '0')+ num*16;
    else if(buffer[index] >= 'a' && buffer[index] <= 'f')
      num = (buffer[index] - 'a' + 10 )+ num*16;
    else if(buffer[index] >= 'A' && buffer[index] <= 'F')
      num = (buffer[index] - 'A' + 10 )+ num*16;
  }

  *mem = num;

  enable_interrupt();
}

void rw_task1(void)
{

  int mem1, mem2 = 0;
  int curs = 0;
  int memory[RW_TIMES];
  int i = 0, j;

  for(i = 0; i < RW_TIMES; i++)
  {
    scanf(&mem1);
    memory[i] = mem2 = rand();

    *(int *)mem1 = mem2;

    sys_move_cursor(25, curs + i);
    printf("Write: 0x%x, %d            ", mem1, mem2);

    for(j = 0; j < 150; j ++)  // wait for irq_timer so that screen can be reflushed && 
      sys_move_cursor(0, curs + i);
  }

  curs = RW_TIMES;

  for(i = 0; i < RW_TIMES; i++)
  {

    scanf(&mem1);
    memory[i+RW_TIMES] = *(int *)mem1;

    sys_move_cursor(25, curs + i);
    if(memory[i+RW_TIMES] == memory[i])
      printf("Read succeed: %d           ",memory[i]);
    else
      printf("Read error: %d             ",memory[i+RW_TIMES]);
    
    if(i == 0)
      for(j = 0; j < 150; j ++)  // wait for irq_timer so that screen can be reflushed
        sys_move_cursor(0, curs + i);
  }

  while(1);
}
