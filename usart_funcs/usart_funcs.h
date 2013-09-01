#include "../libs/stm32f10x.h"

#define SIZE_BUF 16

void FlushBuf(void);
void PutChar(unsigned char sym);
unsigned char GetChar(void);
void SendStr(unsigned char * data);
