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
  FlushBuf(); ///////////Clean bufer
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

void USARTFindResponse(usart_resp *res)
{
	uint32_t rx_cnt, rx_hd;
	uint8_t symb, state = 0;
	uint8_t i=0;

	rx_cnt = RXcount;
	rx_hd = RXhead;

	while (state != 3)//while start of the string isn't found
	{
		symb = USART_GetChar();
		switch (symb){
			case 0:
				RXcount = rx_cnt;
				RXhead = rx_hd;
				return;
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
	if (state == 3) //If start found
	{
		res->resp_data[i] = symb;
		i++;

		while (state != 5)//while end of the string isn't found
		{
			symb = USART_GetChar();
			switch (symb){
				case 0:
					RXcount = rx_cnt;
					RXhead = rx_hd;
					return;
				break;
				case 0x0d:
					state = 4;
				break;
				case 0x0a:
					if(state == 4)
						{
							state = 5;
							res->resp_res = 1;
							res->resp_data[i] = '\0';
							return;
						}
					else state = 3;
				break;
				default:
					state = 3;
					res->resp_data[i] = symb;
					i++;
				break;
			}
		}
	}
	RXcount = rx_cnt;
	RXhead = rx_hd;
	res->resp_res = 0;
	return;
}
/*
USARTSendCmd(task *resp_task, uint8_t *cmd, uint8_t timeout)
{
	resp_task->in_progress = 1;
	resp_task->timer = timeout;
	USARTSendStr(cmd);
}
*/
