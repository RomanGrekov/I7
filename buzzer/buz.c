#include "buz.h"

void InitBuz (void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Clock port

    GPIOA->CRL      &= ~GPIO_CRL_CNF0;
    GPIOA->CRL      |= GPIO_CRL_MODE0_0;
}

void Bzz(uint8_t variant)
{
	uint8_t lenght, d1, d2;
	uint8_t i;

	switch (variant)
	{
	case 0:
		lenght = 80;
		d1 = 20;
		d2 = 6;
		break;
	case 1:
		lenght = 120;
		d1 = 15;
		d2 = 5;
		break;
	}

	for (i=0; i<lenght; i++)
	{
		GPIOA->BSRR |= GPIO_BSRR_BS0;
		delay_us(d1);
		GPIOA->BSRR |= GPIO_BSRR_BR0;
		delay_us(d2);
	}
}
