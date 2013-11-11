#include "libs/stm32f10x.h"
#include "clock/clock.h"
#include "delay/delay.h"
#include "common/common_funcs.h"
#include "kb_driver/keyboard_driver.h"
#include "USART/usart.h"
#include "usart_funcs/usart_funcs.h"
#include "sim900/sim900.h"
#include "hd44780/hd44780.h"
#include "buzzer/buz.h"
#include "globs.h"
#include "menu/menu.h"

void InitAll(void);
task* InitTasks(task *my_task);
void analize_status(uint8_t retcode);

uint8_t state=0;
button *my_btn;

int main(void)
{
        InitAll();
        LCDPrintS("==LCD test OK!==*==============*");
		delay_timer_ms(1000);
		lcd_clrscr();

		InitMenu();

        while(1)
        {
        	switch(state)
        	{
        		case 0: //simple kb checking
					my_btn = get_btn();
					if(my_btn->button)USART_PutChar(my_btn->button);
        			ProcessMenu(my_btn->button, my_btn->duration);

					if(USARTFindCmd("0506073568")) USARTSendCmd("ata\r\n");
        		break;

        		case 1:
        		break;

        		case 2:
        		break;
        	}
        }

}

void TIM2_IRQHandler(void)
{
	static uint16_t i=0;
	TIM2->SR &= ~TIM_SR_UIF;

	kb_strobe();

	if (i == 100)
	{
		GPIOB->ODR ^= GPIO_ODR_ODR0;
		i=0;
	}
	i++;

	USARTCheckData();
}

void InitAll(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //Clock port

    GPIOB->CRL      &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
    GPIOB->CRL      |= (GPIO_CRL_MODE0_0 | GPIO_CRL_MODE1_0);

    InitRCC();
	//Разрешаем прерывание если кварц плохо
	init_bad_clock_inter();

    delay_timer_ms_init();

    lcd_init();

    InitUSART(9600);
    usart_interrupt_init();

    init_keyboard();

    timer2_init(10);

    FlushBuf();

    InitSim900Port();

    InitBuz();

   }

// где-то в main.c
void HardFault_Handler(void)
{// пустой обработчик вместо бесконечного цикла
	Bzz(1);
}

void analize_status(uint8_t retcode)
{
	if (retcode)
	{
		GPIOB->BSRR |= GPIO_BSRR_BS1; // 1 means fail
		Bzz(0);
	}
	else GPIOB->BSRR |= GPIO_BSRR_BR1; // 0 means success
}

task* InitTasks(task *my_task)
{
	my_task->done='0';
	my_task->to_run='1';
	return &my_task;
}
