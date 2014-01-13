#include "libs/stm32f10x.h"
#include "clock/clock.h"
#include "delay/delay.h"
#include "common/common_funcs.h"
#include "kb_driver/keyboard_driver.h"
#include "USART/usart.h"
#include "USART/usart2.h"
#include "usart_funcs/usart_funcs.h"
#include "sim900/sim900.h"
#include "hd44780/hd44780.h"
#include "buzzer/buz.h"
#include "globs.h"
#include "menu/menu.h"
#include "flash/flash.h"
#include "slow_timer/slow_timer.h"

void InitAll(void);
void analize_status(uint8_t retcode);
void FirstRun(void);
void HardFault_Handler(void);
void LED(void);
void search_for_cmd(void);


uint8_t state=1;
button *my_btn;
struct SavedDomain SysConf;

int main(void)
{
	uint8_t lcd_show = 1, was_in_menu=0;
	InitAll();
	FirstRun();
	while(1)
	{
		switch(state)
		{
			case 0: //simple kb checking
				my_btn = get_btn();
				ProcessMenu(my_btn->button, my_btn->duration);


				if(is_in_menu()) was_in_menu=1;
				if(!is_in_menu() && was_in_menu){
					state=1;
					was_in_menu=0;
				}
				search_for_cmd();
			break;

			case 1:
				lcd_clrscr();
				LCDPrintS("=====DISARM=====");
				state=0;
			break;

			case 2:
			break;
		}
	}
}

void FirstRun(){
	//struct SavedDomain Conf;
	flash_read_struct(&SysConf, sizeof(SysConf));
	if(SysConf.first_run != 13){ //Wirte default config
		WriteDefConf();
	}

	USARTSendStr("USART 1 OK\r\n");
	USART2SendStr("USART 2 OK\r\n");

	Slow_Timer_Add(tm_Repeat, 1000, LED);
	Slow_Timer_Add(tm_Repeat, 10, kb_strobe);
	Slow_Timer_Add(tm_Repeat, 10, USARTCheckData);
	//Slow_Timer_Add(tm_Repeat, 10, search_for_cmd);

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
    InitUSART2(9600);
    usart_interrupt_init();
    usart2_interrupt_init();

    init_keyboard();

    //timer2_init(10);

    InitSim900Port();

    InitBuz();

	InitMenu();

	Init_Slow_Timer();
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

void LED(void){
	GPIOB->ODR ^= GPIO_ODR_ODR1;
}

void search_for_cmd(void){
		if(USARTFindCmd(SysConf.privat_tel_num_1)) USARTSendCmd("ata\r\n");
}
