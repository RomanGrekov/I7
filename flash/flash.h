#ifndef FLASH_H
#define FLASH_H
#include "../libs/stm32f10x.h"

#define FLASH_SIZE 65536 // 65536 bytes = 64K

void flash_unlock(void);
void flash_lock(void);
uint8_t flash_ready(void);
void flash_erase_all_pages(void);
void flash_erase_page(uint32_t address);
void flash_write(uint32_t address,uint32_t data);
uint8_t flash_read(uint32_t address);

#endif

