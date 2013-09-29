#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
#include "../globs.h"

#define SIZE_BUF 32

void FlushBuf(void);
void USART_PutChar(unsigned char sym);
unsigned char USART_GetChar(void);
void USARTSendStr(unsigned char * data);
uint8_t USARTFindResponse();
void USARTSendCmd(uint8_t *cmd, usart_resp *res, uint32_t timeout);
void copy_buf(uint8_t *rxbuf, uint8_t *rxbuf1);
uint8_t get_char(uint8_t *buf);
uint8_t find_template(uint8_t *resp, uint8_t *template);
