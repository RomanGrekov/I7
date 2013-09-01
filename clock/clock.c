#include "clock.h"


void init_bad_clock_inter(void)
{
	/* Включаем защиту HSE от сбоев - CSS */
	RCC->CR |= RCC_CR_CSSON;
}

uint32_t InitRCC(void) 
{
	uint32_t StartUpCounter = 0, HSEStatus = 0;
 
	/* Конфигурацяи  SYSCLK, HCLK, PCLK2 и PCLK1 */
	/* Включаем HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
	/* Ждем пока HSE не выставит бит готовности либо не выйдет таймаут*/
  do 
 	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
  } 
 	while( (HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));
		
	/* Если HSE запустился нормально */ 
	if ( (RCC->CR & RCC_CR_HSERDY) != RESET) 
 	{
			
 		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
 	  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;
  }
	else 
	{
		//Do something
  }
 
return HSEStatus;
}
 
 
/* Обработчик прерывания NMI Возникает если кварц сбойнул. Отключить его нельзя*/
void NMI_Handler( void) 
{
/*Очищаем флаг прерывания CSS иначе прерывание будет генерироваться бесконечно*/
RCC->CIR |= RCC_CIR_CSSC;
 
//Some code to handle situation
}
