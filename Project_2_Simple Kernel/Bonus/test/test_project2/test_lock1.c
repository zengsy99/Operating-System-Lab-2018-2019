#include "test2.h"
#include "lock.h"
#include "stdio.h"
#include "syscall.h"

int is_init = FALSE;
static char blank[] = {"                                             "};

mutex_lock_t mutex_lock_1, mutex_lock_2, mutex_lock_3;

void lock_task1(void)
{
        int print_location_1 = 1;
	int print_location_2 = 2;
	int print_location_3 = 3;
        while (1)
        {
                int i;
                if (!is_init)
                {
                        do_mutex_lock_init(&mutex_lock_1, 1);
			do_mutex_lock_init(&mutex_lock_2, 2);
			do_mutex_lock_init(&mutex_lock_3, 3);
                        is_init = TRUE;
                }

                vt100_move_cursor(1, print_location_1);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_2);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_3);
                printk("%s", blank);

                vt100_move_cursor(1, print_location_1);
                printk("> [TASK] Applying for a lock_1.\n");
                vt100_move_cursor(1, print_location_2);
                printk("> [TASK] Applying for a lock_2.\n");
                vt100_move_cursor(1, print_location_3);
                printk("> [TASK] Applying for a lock_3.\n");

                do_scheduler();

                do_mutex_lock_acquire(&mutex_lock_1);
		do_mutex_lock_acquire(&mutex_lock_2);
		do_mutex_lock_acquire(&mutex_lock_3);

                for (i = 0; i < 20; i++)
                {
                        vt100_move_cursor(1, print_location_1);
                        printk("> [TASK] Has acquired lock_1 and running.(%d)\n", i);
                        vt100_move_cursor(1, print_location_2);
                        printk("> [TASK] Has acquired lock_2 and running.(%d)\n", i);
                        vt100_move_cursor(1, print_location_3);
                        printk("> [TASK] Has acquired lock_3 and running.(%d)\n", i);
                        do_scheduler();
                }

                vt100_move_cursor(1, print_location_1);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_2);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_3);
                printk("%s", blank);

                vt100_move_cursor(1, print_location_1);
                printk("> [TASK] Has acquired lock_1 and exited.\n");
                vt100_move_cursor(1, print_location_2);
                printk("> [TASK] Has acquired lock_2 and exited.\n");
                vt100_move_cursor(1, print_location_3);
                printk("> [TASK] Has acquired lock_3 and exited.\n");

                do_mutex_lock_release(&mutex_lock_1);
		do_mutex_lock_release(&mutex_lock_2);
		do_mutex_lock_release(&mutex_lock_3);

                do_scheduler();
        }
}

void lock_task2(void)
{
        int print_location_5 = 5;
	int print_location_6 = 6;
	int print_location_7 = 7;
        while (1)
        {
                int i;
                if (!is_init)
                {
                        do_mutex_lock_init(&mutex_lock_1, 1);
			do_mutex_lock_init(&mutex_lock_2, 2);
			do_mutex_lock_init(&mutex_lock_3, 3);
                        is_init = TRUE;
                }

                vt100_move_cursor(1, print_location_5);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_6);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_7);
                printk("%s", blank);

                vt100_move_cursor(1, print_location_5);
                printk("> [TASK] Applying for a lock_1.\n");
                vt100_move_cursor(1, print_location_6);
                printk("> [TASK] Applying for a lock_2.\n");
                vt100_move_cursor(1, print_location_7);
                printk("> [TASK] Applying for a lock_3.\n");

                do_scheduler();

                do_mutex_lock_acquire(&mutex_lock_1);
		do_mutex_lock_acquire(&mutex_lock_2);
		do_mutex_lock_acquire(&mutex_lock_3);

                for (i = 0; i < 20; i++)
                {
                        vt100_move_cursor(1, print_location_5);
                        printk("> [TASK] Has acquired lock_1 and running.(%d)\n", i);
                        vt100_move_cursor(1, print_location_6);
                        printk("> [TASK] Has acquired lock_2 and running.(%d)\n", i);
                        vt100_move_cursor(1, print_location_7);
                        printk("> [TASK] Has acquired lock_3 and running.(%d)\n", i);
                        do_scheduler();
                }

                vt100_move_cursor(1, print_location_5);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_6);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_7);
                printk("%s", blank);

                vt100_move_cursor(1, print_location_5);
                printk("> [TASK] Has acquired lock_1 and exited.\n");
                vt100_move_cursor(1, print_location_6);
                printk("> [TASK] Has acquired lock_2 and exited.\n");
                vt100_move_cursor(1, print_location_7);
                printk("> [TASK] Has acquired lock_3 and exited.\n");

                do_mutex_lock_release(&mutex_lock_1);
		do_mutex_lock_release(&mutex_lock_2);
		do_mutex_lock_release(&mutex_lock_3);

                do_scheduler();
        }
}

void lock_task3(void)
{
        int print_location_9 = 9;
	int print_location_10 = 10;
	int print_location_11 = 11;
        while (1)
        {
                int i;
                if (!is_init)
                {
                        do_mutex_lock_init(&mutex_lock_1, 1);
			do_mutex_lock_init(&mutex_lock_2, 2);
			do_mutex_lock_init(&mutex_lock_3, 3);
                        is_init = TRUE;
                }

                vt100_move_cursor(1, print_location_9);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_10);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_11);
                printk("%s", blank);

                vt100_move_cursor(1, print_location_9);
                printk("> [TASK] Applying for a lock_1.\n");
                vt100_move_cursor(1, print_location_10);
                printk("> [TASK] Applying for a lock_2.\n");
                vt100_move_cursor(1, print_location_11);
                printk("> [TASK] Applying for a lock_3.\n");

                do_scheduler();

                do_mutex_lock_acquire(&mutex_lock_1);
		do_mutex_lock_acquire(&mutex_lock_2);
		do_mutex_lock_acquire(&mutex_lock_3);

                for (i = 0; i< 20; i++)
                {
                        vt100_move_cursor(1, print_location_9);
                        printk("> [TASK] Has acquired lock_1 and running.(%d)\n", i);
                        vt100_move_cursor(1, print_location_10);
                        printk("> [TASK] Has acquired lock_2 and running.(%d)\n", i);
                        vt100_move_cursor(1, print_location_11);
                        printk("> [TASK] Has acquired lock_3 and running.(%d)\n", i);
                        do_scheduler();
                }

                vt100_move_cursor(1, print_location_9);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_10);
                printk("%s", blank);
                vt100_move_cursor(1, print_location_11);
                printk("%s", blank);

                vt100_move_cursor(1, print_location_9);
                printk("> [TASK] Has acquired lock_1 and exited.\n");
                vt100_move_cursor(1, print_location_10);
                printk("> [TASK] Has acquired lock_2 and exited.\n");
                vt100_move_cursor(1, print_location_11);
                printk("> [TASK] Has acquired lock_3 and exited.\n");

                do_mutex_lock_release(&mutex_lock_1);
		do_mutex_lock_release(&mutex_lock_2);
		do_mutex_lock_release(&mutex_lock_3);

                do_scheduler();
        }
}
