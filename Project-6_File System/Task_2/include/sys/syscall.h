/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                       System call related processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#ifndef INCLUDE_SYSCALL_H_
#define INCLUDE_SYSCALL_H_

#include "type.h"
#include "sync.h"
#include "sched.h"
#include "queue.h"
#include "mailbox.h"

#define IGNORE 0
#define NUM_SYSCALLS 64

/* SYSCALL */
#define SYSCALL_SLEEP 2

#define SYSCALL_INIT_MAC          6
#define SYSCALL_NET_RECV          7
#define SYSCALL_NET_SEND          8
#define SYSCALL_WAIT_RECV_PACKAGE 9

#define SYSCALL_BLOCK       10
#define SYSCALL_UNBLOCK_ONE 11
#define SYSCALL_UNBLOCK_ALL 12

#define SYSCALL_MKFS    13
#define SYSCALL_MKDIR   14
#define SYSCALL_CD      15
#define SYSCALL_LS      16
#define SYSCALL_STATFS  17
#define SYSCALL_RMDIR   18

#define SYSCALL_WRITE   20
#define SYSCALL_READ    21
#define SYSCALL_CURSOR  22
#define SYSCALL_REFLUSH 23

#define SYSCALL_TOUCH   24
#define SYSCALL_CAT     25
#define SYSCALL_FOPEN   26
#define SYSCALL_FREAD   27
#define SYSCALL_FWRITE  28
#define SYSCALL_FCLOSE  29

#define SYSCALL_MUTEX_LOCK_INIT    30
#define SYSCALL_MUTEX_LOCK_ACQUIRE 31
#define SYSCALL_MUTEX_LOCK_RELEASE 32

#define SYSCALL_MBOX_INIT  35
#define SYSCALL_MBOX_OPEN  36
#define SYSCALL_MBOX_CLOSE 37
#define SYSCALL_MBOX_SEND  38
#define SYSCALL_MBOX_RECV  39

#define SYSCALL_PS     40
#define SYSCALL_CLEAR  41
#define SYSCALL_GETPID 42

#define SYSCALL_SPAWN  46
#define SYSCALL_KILL   47
#define SYSCALL_EXIT   48
#define SYSCALL_WAIT   49

#define SYSCALL_SEMAPHORE_INIT 50
#define SYSCALL_SEMAPHORE_UP   51
#define SYSCALL_SEMAPHORE_DOWN 52

#define SYSCALL_CONDITION_INIT      53
#define SYSCALL_CONDITION_WAIT      54
#define SYSCALL_CONDITION_SIGNAL    55
#define SYSCALL_CONDITION_BROADCAST 56

#define SYSCALL_BARRIER_INIT 57
#define SYSCALL_BARRIER_WAIT 58

/* syscall function pointer */ 
int (*syscall[NUM_SYSCALLS])();

void system_call_helper(int, int, int, int);
extern int invoke_syscall(int, int, int, int);

void sys_sleep(uint32_t);

void sys_block(queue_t *);
void sys_unblock_one(queue_t *);
void sys_unblock_all(queue_t *);

void sys_write(char *);
void sys_move_cursor(int, int);
void sys_reflush();

void mutex_lock_init(mutex_lock_t *);
void mutex_lock_acquire(mutex_lock_t *);
void mutex_lock_release(mutex_lock_t *);

void sys_ps(int *);
void sys_clear(int line1, int line2);
pid_t sys_getpid();

void sys_spawn(task_info_t *);
void sys_kill(pid_t pid);
void sys_exit();
void sys_waitpid(pid_t pid);

void semaphore_init(semaphore_t *, int);
void semaphore_up(semaphore_t *);
void semaphore_down(semaphore_t *);

void condition_init(condition_t *);
void condition_wait(mutex_lock_t *, condition_t *);
void condition_signal(condition_t *);
void condition_broadcast(condition_t *);

void barrier_init(barrier_t *, int);
void barrier_wait(barrier_t *);

void mbox_init();
mailbox_t *mbox_open(char *);
void mbox_close(mailbox_t *);
void mbox_send(mailbox_t *, void *, int);
void mbox_recv(mailbox_t *, void *, int);

void sys_init_mac();
uint32_t sys_net_recv(uint32_t, uint32_t, uint32_t);
void sys_net_send(uint32_t, uint32_t);
void sys_wait_recv_package();

void sys_mkfs();
uint32_t sys_mkdir(char *);
uint32_t sys_rmdir(char *);
uint32_t sys_cd(char *);
void sys_ls(char *, int *);
void sys_statfs(int *);

int sys_touch(char *);
int sys_cat(char *, int *);
int sys_fopen(char *, int);
int sys_fread(int, char *, int);
int sys_fwrite(int, char *, int);
void sys_fclose(int);

#endif
