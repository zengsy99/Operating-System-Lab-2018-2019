#include "time.h"
#include "test3.h"
#include "lock.h"
#include "sched.h"
#include "stdio.h"
#include "mailbox.h"
#include "syscall.h"

struct task_info sq_task = {"SunQuan", (uint32_t)&SunQuan, USER_PROCESS};
struct task_info lb_task = {"LiuBei", (uint32_t)&LiuBei, USER_PROCESS};
struct task_info cc_task = {"CaoCao", (uint32_t)&CaoCao, USER_PROCESS};

void SunQuan(void)
{

}

void LiuBei(void)
{

}

void CaoCao(void)
{

}
