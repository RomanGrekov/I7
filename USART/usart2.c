#include "usart2.h"

void InitUSART2(uint16_t boudrate){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Clock port A

    //============Pin 2 (TX) setup===================//
    GPIOA->CRL      &= ~(GPIO_CRL_CNF2_0); //Set pin 2 to alternative push-pull
    GPIOA->CRL      |= GPIO_CRL_CNF2_1; //
    GPIOA->CRL      |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0); // 50 MHz
    //============Pin 3 (RX) setup===================//
    GPIOA->CRL      &= ~(GPIO_CRL_CNF3_1); //Set pin 3 to input
    GPIOA->CRL      |= GPIO_CRL_CNF3_0; // without lift to positive
    GPIOA->CRL      &= ~(GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0); // input

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable clock for USART2

    USART2->BRR = F_CPU/boudrate; //Calculate value for speed. In my case it's 0x4e2
    USART2->CR1 |= USART_CR1_UE; //Enable USART
    USART2->CR1 &= ~(USART_CR1_M); // 8 data bit
    USART2->CR1 &= ~(USART_CR1_PCE); // Parity disabled
    USART2->CR1 |=(USART_CR1_TE | USART_CR1_RE); // Enable transmit and receive

    USART2->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1); // One stop bit
}

void send_to_usart2(uint8_t data) {
	//while(!(USART1->SR & USART_SR_TC)); //Waiting until bit TC in register SR not 1
	USART1->DR=data; //Send byte
}

void usart2_interrupt_init(void){
	USART2->CR1 |= USART_CR1_RXNEIE;/*!<RXNE Interrupt Enable */
	USART2->CR1 |= USART_CR1_TCIE;/*!<Transmission Complete Interrupt Enable */
	NVIC_EnableIRQ(USART2_IRQn);//Allow usart receive unterruption
	NVIC_SetPriority(USART2_IRQn, 15);
}

void send_string2(uint8_t *data){
	while (*data){
		send_to_usart2(*data);
		data++;
	}
}
