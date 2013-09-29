#include "usart_funcs.h"

void USART_PutChar(unsigned char sym);

//кольцевой (циклический) буфер
volatile uint8_t RXBuf[SIZE_BUF]; //main buffer
volatile uint8_t RXtail = 0;      //"указатель" хвоста буфера
volatile uint8_t RXhead = 0;   //"указатель" головы буфера
volatile uint8_t RXcount = 0;  //счетчик символов

volatile uint8_t RXhead1 = 0;   //"указатель" головы буфера
volatile uint8_t RXcount1 = 0;  //счетчик символов

volatile uint8_t TXBuf[SIZE_BUF];
volatile uint8_t TXtail = 0;
volatile uint8_t TXhead = 0;
volatile uint8_t TXcount = 0;

usart_resp *response;

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

void copy_buf(uint8_t *rxbuf, uint8_t *rxbuf1)
{
	RXcount1 = 0;
	RXhead1 = 0;
	uint8_t i=0;
	while (rxbuf[i]){
		rxbuf1[i] = rxbuf[i];
		i++;
		RXcount1++;
	}
}
//взять символ из буфера
uint8_t get_char(uint8_t *buf)
{
   uint8_t sym = 0;
   if (RXcount1 > 0){         //если буфер не пустой
      sym = buf[RXhead1];   //считываем символ из буфера
      RXcount1--;             //уменьшаем счетчик символов
      RXhead1++;              //инкрементируем индекс головы буфера
      if (RXhead1 == SIZE_BUF) RXhead1 = 0;
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

uint8_t USARTFindResponse(void)
{
	uint8_t symb, state = 0;
	uint8_t i=0;
	uint8_t buf[SIZE_BUF];//find command buffer

	copy_buf(RXBuf, buf);

	response->timeout--;

	while (state != 3)//while start of the string isn't found
	{
		symb = get_char(buf);
		switch (symb){
			case 0:
				return 0;
			break;
			case 0x0d:
				state = 1;
			break;
			case 0x0a:
				if (state == 1)state = 2;
				else state = 0;
			break;
			default:
				if (state != 2) state = 0;
				else state = 3;
			break;
		}
	}
	response->resp_data[i] = symb;
	i++;
	while (1)//while end of the string isn't found
	{
		symb = get_char(buf);
		switch (symb){
			case 0:
				return 0;
			break;
			case 0x0d:
				state = 4;
			break;
			case 0x0a:
				if(state == 4)
					{
						response->resp_res = 1;
						response->resp_data[i] = '\0';
						return 1;
					}
				else state = 3;
			break;
			default:
				state = 3;
				response->resp_data[i] = symb;
				i++;
			break;
		}
	}
	return 0;
}

void USARTSendCmd(uint8_t *cmd, usart_resp *res, uint32_t timeout)
{
	response = res;
    FlushBuf(); ///////////Clean buffer
	response->resp_expect = 0;
	response->resp_res = 0;
	response->timeout=timeout;
	USARTSendStr(cmd);
}

uint8_t find_template(uint8_t *resp, uint8_t *template)
{
	uint8_t t_size, i=0;
	t_size = sizeof(template);
	for (i=0; i < t_size; i++)
		if (resp[i] != template[i]) return 0;
	return 1;
}
