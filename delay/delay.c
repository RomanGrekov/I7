#include "delay.h"

void delay_timer_ms_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Enable timer4 clock
	TIM4->PSC = F_CPU/1000 - 1; //Presceler
	TIM4->CNT = 65535;//Загружаем число миллисекунд в счетный регистр
	TIM4->CR1 = TIM_CR1_CEN;//Запускаем счет
	TIM4->SR &= ~TIM_SR_UIF;//Теперь флаг события надо сбросить вручную
}

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
	NVIC_SetPriority(TIM2_IRQn, 15);
}

void delay_us(uint16_t us)
{
	 uint32_t n, alfa, i;
	 n = us * (F_CPU / 1000000);
	 i=0;
	 for(alfa=0;alfa<n;alfa++)
	 {
		 i++;
	 }
}

void delay_ms(uint32_t ms){
	 uint32_t n, alfa, i;
	 n = ms * (F_CPU / 1000);
	 i=0;
	 for(alfa=0;alfa<n;alfa++)
	 {
		 i++;
	 }
}
