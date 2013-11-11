#ifndef USART_FUNCS_H
#define USART_FUNCS_H

#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
#include "../globs.h"

#define SIZE_BUF 200 //Rx buffer size
#define TX_SIZE_BUF 64 //Rx buffer size
#define USART_RESP 200 // Response from at command buffer size
#define USART_RESP_AMOUNT 3 // Responses stack size

struct usart_response{
		uint8_t resp_data[USART_RESP];
		uint8_t length;
};
struct found_template{
	uint8_t found;
	uint8_t shift;
};

void FlushBuf(void); //Clear rx buf
void USART_PutChar(unsigned char sym); //Put char to tx buffer
unsigned char USART_GetChar(void); //Get char from rx buffer
void USARTSendStr(unsigned char * data); //Send string
void USARTSendCmd(uint8_t *cmd);

struct found_template find_template(uint8_t *resp, uint8_t *template);

uint8_t USARTFindCmd(uint8_t *template);
uint8_t getSize(uint8_t *my_array);
void USARTCheckData(void);
void USARTPutResp(struct usart_response res);
struct usart_response* USARTGetResp(void);
uint8_t USARTHasResp(void);
uint8_t USARTFindCmdWithData(uint8_t *before_template, uint8_t *after_template,
		                     uint8_t *data);

#endif
