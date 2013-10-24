#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
#include "../globs.h"

void FlushBuf(void); //Clear rx buf
void FlushResponse(usart_resp *res); // Clear response
void USART_PutChar(unsigned char sym); //Put char to tx buffer
unsigned char USART_GetChar(void); //Get char from rx buffer
void USARTSendStr(unsigned char * data); //Send string
uint8_t USARTFindResponse(usart_resp *res);   //Find response in rx buffer(single
											  //response only). Use Adv version!
uint8_t USARTFindResponseAdv(usart_resp *res);//Find response in rx buffer
//Send at command
void USARTSendCmd(uint8_t *cmd, usart_resp *res, uint32_t timeout, uint8_t res_amount);

uint8_t find_template(uint8_t *resp, uint8_t *template);

uint8_t USARTFindCmdResponse(usart_resp*res, uint8_t *template);
uint8_t getSize(uint8_t *my_array);
