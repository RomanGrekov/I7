#include "../libs/stm32f10x.h"

#define SIZE_BUF 32

void FlushBuf(void);
void USART_PutChar(unsigned char sym);
unsigned char USART_GetChar(void);
void SendStr(unsigned char * data);
uint8_t find_str(uint8_t *pattern);

