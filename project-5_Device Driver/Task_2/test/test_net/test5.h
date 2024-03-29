#ifndef INCLUDE_TEST4_H_
#define INCLUDE_TEST4_H_
#include "queue.h"

extern queue_t recv_block_queue;
#define EPT_ARP 0x0608 /* type: ARP */

static void init_data(uint32_t *addr);

#if 1
void phy_regs_task1(void);
void phy_regs_task2(void);
void phy_regs_task3(void);
#endif
static void init_mac(void);

#endif