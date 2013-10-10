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
usart_resp cmd_resp;
button *my_btn;

int main(void)
{

	uint8_t sim_status;

        InitAll();
        LCDPrintS("==LCD test OK!==*==============*");
        USARTSendStr("USART test OK\r\n");
		delay_timer_ms(1000);
		lcd_clrscr();

		InitMenu();
        while(1)
        {
        	switch(state)
        	{
        		case 0: //simple kb checking
					my_btn = get_btn();
					if (my_btn->button != 0){
						switch (my_btn->button){
							case '6':
								changeMenu(MENU_NEXT);
							break;
							case '4':
								changeMenu(MENU_PREVIOUS);
							break;
							case '2':
								changeMenu(MENU_PARENT);
							break;
							case '8':
								changeMenu(MENU_CHILD);
							break;

						}
					}
		            if(MenuChanged()){
		            	lcd_clrscr();
		            	LCDPrintS(GetCurMenuName());
		            }
        		break;

        		case 1:
					my_btn = get_btn();
					if (my_btn->button != 0){
						switch (my_btn->button){
							case '*':
								lcd_clrscr();
							break;
							case '1':
								USARTSendCmd("at\r\n", &cmd_resp, 100000);
								state = 2;
							break;
							case '2':
								USARTSendCmd("at+cfun?\r\n", &cmd_resp, 100000);
								state = 2;
							break;
							case '3':
								USARTSendCmd("at+cusd=1,\"*101#\"\r\n", &cmd_resp, 900000);
								state = 2;
							break;
							case '4':
								sim_status = SwitchSim900(1, 5);
								analize_status(sim_status);
							break;
							case '5':
								sim_status = SwitchSim900(0, 5);
								analize_status(sim_status);
							break;
							case '6':
								state = 1;
							break;
							case '7':
							break;
						}
					}
        		break;

        		case 2:
        			if (USARTFindResponse())
        			{
        				LCDPrintS(cmd_resp.resp_data);
        				state = 0;
        			}
        			if (cmd_resp.timeout == 0) state = 0;
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

    init_keyboard();

    timer2_init(10);

    InitUSART(9600);
    usart_interrupt_init();
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
