#include "../libs/stm32f10x.h"
#include "../delay/delay.h"
#include "../usart_funcs/usart_funcs.h"


#define PWR_KEY GPIO_ODR_ODR7 //Pin to set state
#define STATUS_STATE GPIO_IDR_IDR8 //Pin to check STATUS
#define CHECK_REG IDR //Check state reg
#define SET_REG ODR //set state reg

//-------------Config PWR KEY--------------------------------//
#define SET_PWR_REG CRL //can be CRL or CRH					 //
#define PWR_CNF_REG GPIO_CRL_CNF7 // to set port as out      //
#define PWR_MODE_REG0 GPIO_CRL_MODE7_0 // to set port freq   //
#define PWR_MODE_REG1 GPIO_CRL_MODE7_1 // to set port freq   //
//-----------------------------------------------------------//

//-------------Config STATUS----------------------------------------------//
#define SET_STATUS_REG CRH //can be CRL or CRH							  //
#define STATUS_CNF_REG0 GPIO_CRH_CNF8_0 // to set port as input without + //
#define STATUS_CNF_REG1 GPIO_CRH_CNF8_1 // to set port as input without + //
#define STATUS_MODE_REG0 GPIO_CRH_MODE8_0 // to set port as input         //
#define STATUS_MODE_REG1 GPIO_CRH_MODE8_1 // to set port as input         //
//------------------------------------------------------------------------//

#define MANAGE_PORT GPIOA
#define CLOCK_REG RCC_APB2ENR_IOPAEN

void InitSim900Port(void);
uint8_t SwitchSim900(uint8_t state, uint8_t timeout);
uint8_t SimGetStatus(void);

//uint8_t cfun_cmd[]="AT+CFUN=?";
