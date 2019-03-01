/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                  The shell acts as a task running in user mode. 
 *       The main function is to make system calls through the user's output.
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

#include "test.h"
#include "stdio.h"
#include "screen.h"
#include "string.h"
#include "syscall.h"

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

#define SHLLE_BUFF_SIZE 64

static int shell_tail = 0;
static char shell_buff[SHLLE_BUFF_SIZE];

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

#if 1
struct task_info task4_1 = {"send",(uint32_t)&phy_regs_task1, USER_PROCESS};
struct task_info task4_2 = {"recv",(uint32_t)&phy_regs_task2, USER_PROCESS};
struct task_info task4_3 = {"initmac",(uint32_t)&phy_regs_task3, USER_PROCESS};
struct task_info task6_1 = {"hello",(uint32_t)&test_fs, USER_PROCESS};

struct task_info *test_tasks[19] = {&task4_3,&task4_1,&task4_2,&task6_1};
int num_test_tasks = 4;
#endif

struct task_info task_shell = {"shell", (uint32_t)&test_shell, USER_PROCESS};

void test_shell()
{
  char ch = 0;
  char buffer[16];
  int index = 0;
  int shell_location = SHELL_BEGIN;

  sys_move_cursor(1, SHELL_BEGIN-1);
  printf("------------------- COMMAND -------------------");
  sys_move_cursor(1, SHELL_BEGIN);
  printf("> root@UCAS_OS:");

  while(1)
  {
    do{
      //read command from UART port
      disable_interrupt();
      ch = read_uart_ch();
      enable_interrupt();
      printf("%c", ch);
    } while(ch == 0);

    if(ch == 13)
    {
      buffer[index] = '\0';
      if(strcmp(buffer, "ps") == 0)
      {
        shell_location ++;
        sys_move_cursor(1, shell_location);
        sys_ps(&shell_location);

      }
      else if(strcmp(buffer, "clear") == 0)
      {
        sys_clear(SHELL_BEGIN, SCREEN_HEIGHT-1);
        shell_location = SHELL_BEGIN -1;
      }
      else if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'e' && buffer[3] == 'c' && buffer[4] ==' ' && buffer[5] >= '0' && buffer[5] <= '9' && buffer[6] == '\0')
      {
        int num = buffer[5] - '0';
        sys_spawn(test_tasks[num]);
        shell_location ++;
        sys_move_cursor(1, shell_location);
        printf("exec process[%c].", buffer[5]);
      }
      else if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'e' && buffer[3] == 'c' && buffer[4] ==' ' && buffer[5] >= '0' && buffer[5] <= '9' && buffer[6] >= '0' && buffer[5] <= '6'&& buffer[7] == '\0' )
      {
        int num = 10*(buffer[5] - '0') + buffer[6] - '0';
        sys_spawn(test_tasks[num]);
        shell_location ++;
        sys_move_cursor(1, shell_location);
        printf("exec process[%c%c].", buffer[5], buffer[6]);
      }
      else if(buffer[0] == 'k' && buffer[1] == 'i' && buffer[2] == 'l' && buffer[3] == 'l' && buffer[4] ==' ' && buffer[5] >= '0' && buffer[5] <= '9' && buffer[6] == '\0')
      {
        sys_kill(buffer[5] - '0');
        shell_location ++;
        sys_move_cursor(1, shell_location);
        printf("kill process pid = %c.", buffer[5]);
      }
      else if(buffer[0] == 'k' && buffer[1] == 'i' && buffer[2] == 'l' && buffer[3] == 'l' && buffer[4] ==' ' && buffer[5] >= '0' && buffer[5] <= '9' && buffer[6] >= '0' && buffer[5] <= '6'&& buffer[7] == '\0' )
      {
        sys_kill( 10*(buffer[5] - '0') + buffer[6] - '0' );
        shell_location ++;
        sys_move_cursor(1, shell_location);
        printf("kill process pid = %c%c.", buffer[5], buffer[6]);
      }
      else if(buffer[0] == 'm' && buffer[1] == 'k' && buffer[2] == 'f' && buffer[3] == 's' && buffer[4] =='\0')
      {
        shell_location ++;
        sys_move_cursor(1, shell_location);
        sys_mkfs();
        sys_statfs(&shell_location);
      }
      else if(buffer[0] == 's' && buffer[1] == 't' && buffer[2] == 'a' && buffer[3] == 't' && buffer[4] =='f' && buffer[5] == 's' && buffer[6] == '\0')
      {
        shell_location ++;
        sys_move_cursor(1, shell_location);
        sys_statfs(&shell_location);
      }
      else if(buffer[0] == 'c' && buffer[1] == 'd' && buffer[2] == ' ')
      {
      	int k = 0;
        char name[28];

        for(k = 3; buffer[k] != '\0'; k++)
          name[k-3] = buffer[k];
        name[k-3] = '\0';

        sys_cd(name);
      }
      else if(buffer[0] == 'm' && buffer[1] == 'k' && buffer[2] == 'd' && buffer[3] == 'i' && buffer[4] =='r' && buffer[5] == ' ')
      {
        int k = 0;
        char name[28];

        for(k = 6; buffer[k] != '\0'; k++)
          name[k-6] = buffer[k];
        name[k-6] = '\0';

        sys_mkdir(name);
      }
      else if(buffer[0] == 'r' && buffer[1] == 'm' && buffer[2] == 'd' && buffer[3] == 'i' && buffer[4] =='r' && buffer[5] == ' ')
      {
        int k = 0;
        char name[28];

        for(k = 6; buffer[k] != '\0'; k++)
          name[k-6] = buffer[k];
        name[k-6] = '\0';

        sys_rmdir(name);
      }
      else if(buffer[0] == 'l' && buffer[1] == 's' && (buffer[2] == ' ' || buffer[2] == '\0') )
      {
      	int k = 0;
        char name[28];

        for(k = 3; buffer[k] != '\0'; k++)
          name[k-3] = buffer[k];
        name[k-3] = '\0';

        if(buffer[2] == '\0')
            name[0] = '\0';

        shell_location ++;
        sys_move_cursor(1, shell_location);
        sys_ls(name, &shell_location);
      }
      else if(buffer[0] == 't' && buffer[1] == 'o' && buffer[2] == 'u' && buffer[3] == 'c' && buffer[4] =='h' && buffer[5] == ' ')
      {
        int k = 0;
        char name[28];

        for(k = 6; buffer[k] != '\0'; k++)
          name[k-6] = buffer[k];
        name[k-6] = '\0';

        sys_touch(name);
      }
      else if(buffer[0] == 'c' && buffer[1] == 'a' && buffer[2] == 't' && buffer[3] == ' ' )
      {
      	int k = 0;
        char name[28];

        for(k = 4; buffer[k] != '\0'; k++)
          name[k-4] = buffer[k];
        name[k-4] = '\0';

        shell_location ++;
        sys_move_cursor(1, shell_location);
        sys_cat(name, &shell_location);
        shell_location += 9;
      }
      else if(buffer[0] == '.' && buffer[1] == '/' && buffer[2] != '\0')
      {
      	int k = 0;
        char name[28];

        for(k = 2; buffer[k] != '\0'; k++)
          name[k-2] = buffer[k];
        name[k-2] = '\0';

        sys_launch(name);
      }
      else
      {
        shell_location ++;
        sys_move_cursor(1, shell_location);
        printf("Error: Unknown command");
      }

      index = 0;
      shell_location ++;
      sys_move_cursor(1, shell_location);
      printf("> root@UCAS_OS:");
    }
    else
      buffer[index++] = ch;
  }
}
