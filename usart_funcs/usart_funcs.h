#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
//#include "../globs.h"

#define SIZE_BUF 32

void FlushBuf(void);
void USART_PutChar(unsigned char sym);
unsigned char USART_GetChar(void);
void USARTSendStr(unsigned char * data);
uint8_t find_str(uint8_t *pattern);
USARTFindResponse(response *res);
//USARTSendCmd(task * resp_task, uint8_t *cmd, uint8_t timeout);

