#include "usart_funcs.h"

void USART_PutChar(unsigned char sym);

//кольцевой (циклический) буфер
unsigned char RXBuf[SIZE_BUF];
unsigned char RXtail = 0;      //"указатель" хвоста буфера
unsigned char RXhead = 0;   //"указатель" головы буфера
unsigned char RXcount = 0;  //счетчик символов

unsigned char TXBuf[SIZE_BUF];
unsigned char TXtail = 0;
unsigned char TXhead = 0;
unsigned char TXcount = 0;

//"очищает" буфер
void FlushBuf(void)
{
  RXtail = 0;
  RXhead = 0;
  RXcount = 0;
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

uint8_t find_str(uint8_t *pattern)
{
	uint16_t i=0, ii=0;
		while (i < sizeof(RXBuf))
		{
			if (RXBuf[i] == pattern[ii]) ii++;
			else ii=0;
			i++;
			if (ii == sizeof(pattern))return 1;
			if (i > (sizeof(RXBuf) - sizeof(pattern))) return 0;
		}
	return 0;
}
/*
usart_resp* find_response(struct usart_resp *res)
{
	uint8_t cr_cl[2]={0x0d, 0x0a};
	uint8_t cr_cl_flag1 = 0;
	uint16_t i=0, ii=0;

	while (cr_cl_flag1 != 1 && i < SIZE_BUF-3)//while start of the string isn't found
	{
		if (RXBuf[i] == cr_cl[0] && RXBuf[i+1] == cr_cl[1] &&
				RXBuf[i+2] != cr_cl[0] && RXBuf[i+2] != cr_cl[1])// if start found
			{
				cr_cl_flag1 = 1;
			}
		i++;
	}
	if (cr_cl_flag1 == 1) //If start found
	{
		i++; // we don't want store 0x0a byte
		while (i < SIZE_BUF-1)//while end of the string isn't found
		{
			if (RXBuf[i] == cr_cl[0] && RXBuf[i+1] == cr_cl[1])
				{
					res->result = 0;
					return res;
				}
			res->data[ii] = RXBuf[i];
			ii++;
			i++;
		}
	}
	res->result = 1;
	return res;
}

USARTSendCmd(task *resp_task, uint8_t *cmd, uint8_t timeout)
{
	resp_task->in_progress = 1;
	resp_task->timer = timeout;
	USARTSendStr(cmd);
}
*/
