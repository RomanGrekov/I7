#ifndef FLASH_H
#define FLASH_H
#include "../libs/stm32f10x.h"
#include "../USART/usart2.h"
#include "../globs.h"


#define FLASH_SIZE 65536 // 65536 bytes = 64K
#define params_addr 0xfc00 //address of last page


void flash_unlock(void);
void flash_lock(void);
uint8_t flash_ready(void);
void flash_erase_all_pages(void);
void flash_erase_page(uint32_t address);
void flash_write(uint32_t address,uint32_t data);
uint8_t flash_read(uint32_t address);
void WriteDefConf(void);
void flash_write_struct(uint32_t address, uint8_t *struct_p);
void flash_read_struct(uint8_t *struct_p);

#endif

