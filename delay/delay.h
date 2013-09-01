#include "../libs/stm32f10x.h"

#define F_CPU 12000000

void delay_timer_ms(uint16_t ms);
void delay_ms(uint32_t val);

void timer2_init(uint16_t ms);
