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

void InitAll(void);
void analize_status(uint8_t retcode);

int main(void)
{
        InitAll();
        lcd_prints("==LCD test OK!==");
        lcd_goto(2, 0);
        lcd_prints("*==============*");
        //SendStr("USART test OK");
		delay_timer_ms(2000);
		lcd_clrscr();

        while(1)
        {
            GPIOB->ODR ^= GPIO_ODR_ODR0;
            delay_timer_ms(2000);
            //itoa(buttons[0][0], 10, buf);
            //lcd_clear();
            //lcd_out(buf);

            //kb_strobe();
            //GPIOA->BSRR = GPIO_BSRR_BS0;
            //GPIOA->BSRR = GPIO_BSRR_BR0;
            //SendStr(" hello");

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
				SendStr("at\r");
			break;
			case '2':
				SendStr("atd0506073568;\r");
			break;
			case '3':
				SendStr("ata\r");
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
			break;
		}
	}
	symbol = USART_GetChar();
	if (symbol != 0)
	{
		lcd_putcc(symbol);
		if (find_str("Ready"))Bzz(0);
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

    InitSim900();

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
