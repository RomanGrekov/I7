#include "libs/stm32f10x.h"
#include "clock/clock.h"
#include "delay/delay.h"
#include "common/common_funcs.h"
#include "kb_driver/keyboard_driver.h"
#include "USART/usart.h"
#include "usart_funcs/usart_funcs.h"
#include "sim900/sim900.h"
#include "hd44780/hd44780.h"

void InitAll(void);

int main(void)
{
        InitAll();

        lcd_prints("==LCD test OK!==");
        lcd_goto(2, 0);
        lcd_prints("*==============*");
        SendStr("USART test OK");
		delay_timer_ms(5000);
		lcd_clrscr();

        while(1)
        {
            GPIOB->ODR ^= GPIO_ODR_ODR0;
            delay_timer_ms(500);
            //itoa(buttons[0][0], 10, buf);
            //lcd_clear();
            //lcd_out(buf);

            //kb_strobe();
            //GPIOA->BSRR = GPIO_BSRR_BS0;
            //GPIOA->BSRR = GPIO_BSRR_BR0;

    }

}

void TIM2_IRQHandler(void)
{
	uint16_t my_btn;
	uint8_t tmp;
	TIM2->SR &= ~TIM_SR_UIF;
	//GPIOB->ODR ^= GPIO_ODR_ODR1;
	kb_strobe();
	my_btn = get_btn();
	if (my_btn != 0){
		//lcd_send_byte(my_btn, 1);
		//lcd_send_byte('-',1);
		//lcd_send_byte((my_btn & 0xff00)>>8, 1);
		lcd_putc(my_btn);
		tmp = my_btn;
		if(tmp == '*')lcd_clrscr();
		if(tmp == '1')SendStr("at\r");
		if(tmp == '2')SendStr("atd0506073568;\r");
		if(tmp == '3')SendStr("ata\r");
		if(tmp == '4')SwitchSim900(1);
		if(tmp == '5')SwitchSim900(0);
		//USART_PutChar(tmp);
		//lcd_out(my_btn);
	}

	tmp = GetChar();
	if (tmp != 0){
		//lcd_clear();
		//if (char_cnt == 16)
		lcd_putc(tmp);
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
   }

// где-то в main.c
void HardFault_Handler(void)
{// пустой обработчик вместо бесконечного цикла

}

