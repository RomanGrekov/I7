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


void InitAll(void);
task* InitTasks(task *my_task);
void analize_status(uint8_t retcode);

uint8_t state=0;

int main(void)
{
		usart_resp cmd_resp;
		uint32_t trials = 0;

        InitAll();
        LCDPrintS("==LCD test OK!==*==============*");
        USARTSendStr("USART test OK\r\n");
		delay_timer_ms(2000);
		lcd_clrscr();

        while(1)
        {
        	switch(state)
        	{
        		case 0:
		            GPIOB->ODR ^= GPIO_ODR_ODR0;
		            delay_timer_ms(1000);
		            trials = 0;
        		break;

        		case 1:
        			USARTSendStr("at\r\n");
        			delay_timer_ms(1000);
        			state = 2;
        			trials = 20000;
        		break;

        		case 2:
        			cmd_resp.resp_expect = 0;
        			cmd_resp.resp_res = 0;
        			USARTFindResponse(&cmd_resp);
        			if (cmd_resp.resp_res == 1)
        			{
        				LCDPrintS(cmd_resp.resp_data);
        				state = 0;
        			}
        			if (trials == 0) state = 0;
        			trials--;
        		break;
        	}
        }

}

void TIM2_IRQHandler(void)
{
	uint16_t my_btn;
	uint8_t pressed, sim_status;
	uint8_t symbol, test[32];

	TIM2->SR &= ~TIM_SR_UIF;

	kb_strobe();

	my_btn = get_btn();
	if (my_btn != 0){
		//lcd_send_byte(my_btn, 1);
		//lcd_send_byte('-',1);
		//lcd_send_byte((my_btn & 0xff00)>>8, 1);

		pressed = my_btn;
		switch (pressed){
			case '*':
				lcd_clrscr();
			break;
			case '1':
				USARTSendStr("at\r");
			break;
			case '2':
				USARTSendStr("at+cfun?\r\n");
				state = 2;
			break;
			case '3':
				USARTSendStr("ata\r");
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
		}
	}
	//symbol = USART_GetChar();
	if (symbol != 0)
	{
		//lcd_putcc(symbol);
		//USART_PutChar(symbol);
	}
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
