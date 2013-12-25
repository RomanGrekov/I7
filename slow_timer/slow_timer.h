#ifndef SLOW_TIMER_H
#define SLOW_TIMER_H
#include "../libs/stm32f10x.h"

#define SLOW_TIMER_COUNT                20
#define SLOW_TIMER_NUMBER               1              //Timer 1..15 or SYSTICK (==0)
#define SLOW_TIMER_FREQUENCY            1000            // 5..1000

typedef enum
{
 tm_Not_Change,
 tm_Stop,
 tm_Repeat,
 tm_Once
} Timer_Mode_t;

#include <stdint.h>

void Init_Slow_Timer(void);
int8_t Slow_Timer_Add(Timer_Mode_t mode, uint16_t compare, void (*handler)());
uint16_t Slow_Timer_Get_Counter(int8_t TimerId);
void Slow_Timer_Delete(int8_t TimerId);
void Slow_Timer_Modify(int8_t TimerId, Timer_Mode_t mode, uint16_t counter, uint16_t compare);
void Slow_Timer_Lock(void);
void Slow_Timer_Unlock(void);
#endif
