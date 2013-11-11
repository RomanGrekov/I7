#include "usart_funcs.h"

void USART_PutChar(unsigned char sym);

//кольцевой (циклический) буфер
volatile uint8_t RXBuf[SIZE_BUF]; //main buffer
volatile uint8_t RXtail = 0;      //"указатель" хвоста буфера
volatile uint8_t RXhead = 0;   //"указатель" головы буфера
volatile uint8_t RXcount = 0;  //счетчик символов

volatile uint8_t TXBuf[TX_SIZE_BUF];
volatile uint8_t TXtail = 0;
volatile uint8_t TXhead = 0;
volatile uint8_t TXcount = 0;

volatile struct usart_response usart_resp[USART_RESP_AMOUNT];
volatile uint8_t RespTail = 0;
volatile uint8_t RespHead = 0;
volatile uint8_t RespCount = 0;

void USARTPutResp(struct usart_response res)
{
	uint8_t tail;
	if (RespTail == USART_RESP_AMOUNT) RespTail = 0;
	tail = RespTail;
	usart_resp[RespTail] = res;       //   put answer to buffer
	RespTail++;              		  // Increment tail
	if(RespCount < USART_RESP_AMOUNT)RespCount++;// Increment counter

	USARTSendStr("          \0");
	uint8_t debug_msg[] = {'P','u','t',':',GetSymb(tail),' ',
			               'C','n','t',':',GetSymb(RespCount),'\n',0};
	USARTSendStr(debug_msg);
}

struct usart_response* USARTGetResp(void)
{
	uint8_t head;
	struct usart_response *res;
	if (RespCount > 0){         //если буфер не пустой
		head = RespHead;
		res = &usart_resp[RespHead];   //считываем символ из буфера
		RespCount--;             //уменьшаем счетчик символов
		RespHead++;              //инкрементируем индекс головы буфера
		if (RespHead == USART_RESP_AMOUNT) RespHead = 0;
	}
	USARTSendStr("          \0");
	uint8_t debug_msg[] = {'G','e','t',':',GetSymb(head),' ',
						   'C','n','t',':',GetSymb(RespCount),' ','\n',0};
	USARTSendStr(debug_msg);
	return res;
}

uint8_t USARTHasResp(void){
	return RespCount;
}


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
    if (TXcount < TX_SIZE_BUF){                    //если в буфере еще есть место
      TXBuf[TXtail] = sym;             //помещаем в него символ
      TXcount++;                                     //инкрементируем счетчик символов
      TXtail++;                                    //и индекс хвоста буфера
      if (TXtail == TX_SIZE_BUF) TXtail = 0;
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
		    if (TXhead == TX_SIZE_BUF) TXhead = 0;
		  }
		 else{
			 USART1->CR1 &= ~USART_CR1_TXEIE;//!<PE Interrupt Disable
		 }
	}
	if((USART1->SR & USART_SR_TC) != 0){//!<Transmission Complete
		USART1->SR &= ~USART_SR_TC; //Clear flag --^
	}
}

void USARTCheckData(void){
	uint8_t symb;
	enum{
		clear = 0,
		d_found,
		d_a_found,
		symb_found,
		sec_d_found,
		sec_d_a_found
	};
	static uint8_t state = clear, i=0;
	static struct usart_response res;

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
					res.resp_data[i] = 0;
					res.length = i;
					state = clear;
					i = 0;
					USARTPutResp(res);
				}
			if(state != d_a_found && state != sec_d_a_found) state = clear;
			break;
		default:
			if (state == d_a_found) state = symb_found;
			if (state == symb_found){
				res.resp_data[i] = symb;
				if(i < USART_RESP-1)i++;
			}
			if(state != symb_found) state = clear;
			break;
	}
}

void USARTSendCmd(uint8_t *cmd)
{
    FlushBuf(); ///////////Clean buffer
	USARTSendStr(cmd);
}

struct found_template find_template(uint8_t *resp, uint8_t *template)
{
	uint8_t t_size, r_size;
	uint8_t shift=0, cnt=0, found=0;
	struct found_template t_found;
	r_size = getSize(resp);
	t_size = getSize(template);

	t_found.found = 0;
	while((r_size - shift) >= t_size && t_size <= r_size){
		found = 1;
		for (uint32_t i=0; i < t_size; i++)
			if (resp[i+shift] != template[i]){
				found = 0;
				break;
			}
		if(found == 1){
			t_found.found = 1;
			t_found.shift = shift;
			return t_found;
		}
		shift++;
	}
	return t_found;// not found
}

uint8_t getSize(uint8_t *my_array){
	uint8_t i=0;
	while(*my_array++){
		i++;
	}
	return i;
}

uint8_t USARTFindCmd(uint8_t *template){
	uint8_t RespHead_ = RespHead; //Save buffer parameters in case if command not found
	uint8_t RespCount_ = RespCount;
	uint8_t addr=0;
	struct found_template t_found;
	struct usart_response *res;

	while(USARTHasResp()){
		res = USARTGetResp();
		t_found = find_template(res->resp_data , template);
		if (t_found.found) return 1; //return shift of found template in response
	}
	RespHead = RespHead_;
	RespCount = RespCount_;
	return 0;
}

uint8_t USARTFindCmdWithData(uint8_t *before_template, uint8_t *after_template,
		                     uint8_t *data){
	uint8_t RespHead_ = RespHead; //Save buffer parameters in case if command not found
	uint8_t RespCount_ = RespCount;
	uint8_t addr=0, d_size, t1_size;
	struct found_template t1_found, t2_found;
	struct usart_response *res;

	d_size = getSize(data);
	t1_size = getSize(before_template);

	while(USARTHasResp()){
		res = USARTGetResp();
		t1_found = find_template(res->resp_data , before_template);
		t2_found = find_template(res->resp_data , after_template);
		if(t1_found.found && t2_found.found){
			for(uint8_t i=0; i<t2_found.shift; i++){
				if(i < d_size){
					data[i] = res->resp_data[t1_found.shift+t1_size+i];
				}
			}
			return 1;
		}
	}
	RespHead = RespHead_;
	RespCount = RespCount_;
	return 0;
}



