#include "usart.h"

void InitUSART(uint16_t boudrate){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Clock port A
    //============Pin 9 (TX) setup===================//
    GPIOA->CRH      &= ~(GPIO_CRH_CNF9_0); //Set pin 9 to alternative push-pull
    GPIOA->CRH      |= GPIO_CRH_CNF9_1; //
    GPIOA->CRH      |= (GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0); // 50 MHz
    //============Pin 10 (RX) setup===================//
    GPIOA->CRH      &= ~(GPIO_CRH_CNF10_1); //Set pin 10 to input
    GPIOA->CRH      |= GPIO_CRH_CNF10_0; // without lift to positive
    GPIOA->CRH      &= ~(GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0); // input

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enable clock for USART1

    USART1->BRR = F_CPU/boudrate; //Calculate value for speed. In my case it's 0x4e2
    USART1->CR1 |= USART_CR1_UE; //Enable USART
    USART1->CR1 &= ~(USART_CR1_M); // 8 data bit
    USART1->CR1 &= ~(USART_CR1_PCE); // Parity disabled
    USART1->CR1 |=(USART_CR1_TE | USART_CR1_RE); // Enable transmit and receive

    USART1->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1); // One stop bit
}

void send_to_usart(uint8_t data) {
	//while(!(USART1->SR & USART_SR_TC)); //Waiting until bit TC in register SR not 1
	USART1->DR=data; //Send byte
}

void usart_interrupt_init(void){
	USART1->CR1 |= USART_CR1_RXNEIE;/*!<RXNE Interrupt Enable */
	USART1->CR1 |= USART_CR1_TCIE;/*!<Transmission Complete Interrupt Enable */
	NVIC_EnableIRQ(USART1_IRQn);//Allow usart receive unterruption
	NVIC_SetPriority(USART1_IRQn, 14);
}

uint8_t read_byte(void){
	if (USART1->SR & USART_SR_RXNE){
		return 	USART1->DR;
	}
	return 0;
}

void send_string(uint8_t *data){
	while (*data){
		send_to_usart(*data);
		data++;
	}
}

//void USART1_IRQHandler(void)
