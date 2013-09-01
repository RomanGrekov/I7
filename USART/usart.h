#include "../libs/stm32f10x.h"

#define F_CPU 12000000

void InitUSART(uint16_t boudrate);
void send_to_usart(uint8_t data);
uint8_t read_byte(void);
void send_string(uint8_t * data);
void usart_interrupt_init(void);
