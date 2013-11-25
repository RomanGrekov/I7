#include "../libs/stm32f10x.h"

#define F_CPU 12000000

void InitUSART2(uint16_t boudrate);
void send_to_usart2(uint8_t data);
void send_string2(uint8_t * data);
void usart2_interrupt_init(void);
