#include "usart_funcs.h"

void USART_PutChar(unsigned char sym);

//кольцевой (циклический) буфер
volatile uint8_t RXBuf[SIZE_BUF]; //main buffer
volatile uint8_t RXtail = 0;      //"указатель" хвоста буфера
volatile uint8_t RXhead = 0;   //"указатель" головы буфера
volatile uint8_t RXcount = 0;  //счетчик символов

volatile uint8_t TXBuf[SIZE_BUF];
volatile uint8_t TXtail = 0;
volatile uint8_t TXhead = 0;
volatile uint8_t TXcount = 0;

//"очищает" буфер
void FlushBuf(void)
{ uint16_t i=0;
  RXtail = 0;
  RXhead = 0;
  RXcount = 0;
  for (i=0; i < SIZE_BUF; i++) RXBuf[i] = 0;
}

//положить символ в буфер
void PutChar(unsigned char sym)
{
  if (RXcount < SIZE_BUF){   //если в буфере еще есть место
      RXBuf[RXtail] = sym;   //помещаем в него символ
      RXcount++;             //инкрементируем счетчик символов
      RXtail++;              //и индекс хвоста буфера
      if (RXtail == SIZE_BUF) RXtail = 0;
    }
}

//взять символ из буфера
unsigned char USART_GetChar(void)
{
   unsigned char sym = 0;
   if (RXcount > 0){         //если буфер не пустой
      sym = RXBuf[RXhead];   //считываем символ из буфера
      RXcount--;             //уменьшаем счетчик символов
      RXhead++;              //инкрементируем индекс головы буфера
      if (RXhead == SIZE_BUF) RXhead = 0;
   }
   return sym;
}

void USART_PutChar(unsigned char sym)
{
  if((USART1->SR & USART_SR_TC) && (TXcount == 0)){
	  USART1->DR = sym;
  }
  else {
	USART1->CR1 |= USART_CR1_TXEIE;/*!<PE Interrupt enable */
    if (TXcount < SIZE_BUF){                    //если в буфере еще есть место
      TXBuf[TXtail] = sym;             //помещаем в него символ
      TXcount++;                                     //инкрементируем счетчик символов
      TXtail++;                                    //и индекс хвоста буфера
      if (TXtail == SIZE_BUF) TXtail = 0;
    }
  }
}

void USARTSendStr(unsigned char * data)
{
  unsigned char sym;
  while(*data){
    sym = *data++;
    USART_PutChar(sym);
  }
}

void USART1_IRQHandler(void){
	if(USART1->SR & USART_SR_RXNE){/*!<Read Data Register Not Empty */
		PutChar(USART1->DR);
	}
	if((USART1->SR & USART_SR_TXE)!=0){// If Transmit Data Register Empty
		 if (TXcount > 0){                       //если буфер не пустой
		    USART1->DR = TXBuf[TXhead];  //записываем в DR символ из буфера
		    TXcount--;                           //уменьшаем счетчик символов
		    TXhead++;                         //инкрементируем индекс головы буфера
		    if (TXhead == SIZE_BUF) TXhead = 0;
		  }
		 else{
			 USART1->CR1 &= ~USART_CR1_TXEIE;//!<PE Interrupt Disable
		 }
	}
	if((USART1->SR & USART_SR_TC) != 0){//!<Transmission Complete
		USART1->SR &= ~USART_SR_TC; //Clear flag --^
	}
}

uint8_t USARTFindResponseAdv(usart_resp *res)
{
	uint8_t local_res=0;

	while(res->resp_expect != res->resp_res){
		local_res = USARTFindResponse(res);
		if (local_res != 0)return local_res;
	}
	return 0;
}

uint8_t USARTFindResponse(usart_resp *res){
	uint8_t symb, state, i;
	enum{
		clear = 0,
		d_found,
		d_a_found,
		symb_found,
		sec_d_found,
		sec_d_a_found
	};

	state = clear;
	i=res->length; //write answer from the last char (if resp already was)
	if(i != 0){ // if this is second response search
		res->resp_data[i] = '\r'; // need to insert delimeter
		i++;
	}

	while(state != sec_d_a_found){
		symb = USART_GetChar();
		switch (symb){
			case 0:
				break;
			case 0x0d:
				if(state == clear) state = d_found;
				if(state == symb_found) state = sec_d_found;
				break;
			case 0x0a:
				if(state == d_found) state = d_a_found;
				if(state == sec_d_found)
					{
						res->resp_res++;
						res->resp_data[i] = 0;
						res->length = i;
						state = sec_d_a_found;
					}
				if(state != d_a_found && state != sec_d_a_found) state = clear;
				break;
			default:
				if (state == d_a_found) state = symb_found;
				if (state == symb_found){
					res->resp_data[i] = symb;
					if(i < USART_RESP-1)i++;
				}
				if(state != symb_found) state = clear;
				break;
		}
		res->timeout--;
		if (!res->timeout)return 1;
	}
	return 0; //needed responses amount found
}

void USARTSendCmd(uint8_t *cmd, usart_resp *res, uint32_t timeout, uint8_t res_amount)
{
	FlushResponse(res);
    FlushBuf(); ///////////Clean buffer
	res->resp_res = 0;
	res->timeout=timeout;
	res->resp_expect=res_amount;
	USARTSendStr(cmd);
}

void FlushResponse(usart_resp *res){
	res->resp_expect = 0;
	res->resp_res = 0;
	res->timeout = 0;
	res->length = 0;
	for(uint8_t i=0; i< USART_RESP; i++)res->resp_data[i]=0;
}

uint8_t find_template(uint8_t *resp, uint8_t *template)
{
	uint8_t t_size, i=0;
	t_size = sizeof(template);
	for (i=0; i < t_size; i++)
		if (resp[i] != template[i]) return 0;
	return 1;
}
