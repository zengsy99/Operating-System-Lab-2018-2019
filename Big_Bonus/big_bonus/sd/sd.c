#include "sd.h"

void sd_card_read(void *dest, uint32_t offset, uint32_t size){
    sdread((char*)dest, offset, size);
}
void sd_card_write(void *dest, uint32_t offset, uint32_t size){
    sdwrite((char*)dest, offset, size);
}