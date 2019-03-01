#ifndef _INCLUDE_SD_H_
#define _INCLUDE_SD_H_
#include "type.h"
int sdread(unsigned char *buf, unsigned int base, int n);
int sdwrite(unsigned char *buf, unsigned int base, int n);

void sd_card_read(void *dest, uint32_t offset, uint32_t size);
void sd_card_write(void *dest, uint32_t offset, uint32_t size);

//int is_QEMU();
#endif