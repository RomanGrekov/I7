#include "delay.h"

void delay_timer_ms(uint16_t ms)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Enable timer4 clock
	TIM4->PSC = F_CPU/1000 - 1; //Presceler 
	TIM4->CNT = 65535-ms;//Загружаем число миллисекунд в счетный регистр
	TIM4->CR1 = TIM_CR1_CEN;//Запускаем счет
	while((TIM4->SR & TIM_SR_UIF)==0);//Ждем окончания счета
	TIM4->SR &= ~TIM_SR_UIF;//Теперь флаг события надо сбросить вручную
}

void timer2_init(uint16_t ms)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Enable timer4 clock
	TIM2->PSC = F_CPU/1000 - 1; //Presceler
	TIM2->ARR = ms; //Max cnt
	TIM2->CR1 |= TIM_CR1_CEN; // Enable count
	TIM2->DIER|= TIM_DIER_UIE;//Enable interruption by overcount
	
	NVIC_EnableIRQ(TIM2_IRQn);//Allow timer3 unterruption
}
