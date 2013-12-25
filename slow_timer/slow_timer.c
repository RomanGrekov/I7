#include "slow_timer.h"
#include "../bitbanding.h"
#include "../globs.h"

#if (SLOW_TIMER_NUMBER == 1)||(SLOW_TIMER_NUMBER==8)||(SLOW_TIMER_NUMBER==9)||(SLOW_TIMER_NUMBER==10)||(SLOW_TIMER_NUMBER==11)
#define SYS_FREQ_TIMER                          F_CPU
#define TIMER_IRQ_SUFFIX                        _UP_
#define TIMER_SLOW_APBENR                       APB2ENR
#else
#define SYS_FREQ_TIMER                         (SystemFrequency_APB1Clk*2)
#define TIMER_IRQ_SUFFIX                        _
#define TIMER_SLOW_APBENR                       APB1ENR
#endif

#define __SLOW_TIMER(N)                         TIM##N
#define _SLOW_TIMER(N)                          __SLOW_TIMER(N)
#define SLOW_TIMER                              _SLOW_TIMER(SLOW_TIMER_NUMBER)

#define __SLOW_TIMER_IRQH(N,S)                  TIM##N##S##IRQHandler
#define _SLOW_TIMER_IRQH(N,S)                   __SLOW_TIMER_IRQH(N,S)

#if SLOW_TIMER_NUMBER>0
#define SLOW_TIMER_IRQHandler                   _SLOW_TIMER_IRQH(SLOW_TIMER_NUMBER, TIMER_IRQ_SUFFIX)
#else
#define SLOW_TIMER_IRQHandler                   SysTick_Handler
#endif

#define __SLOW_TIMER_IRQN(N,S)                  TIM##N##S##IRQn
#define _SLOW_TIMER_IRQN(N,S)                   __SLOW_TIMER_IRQN(N,S)
#define SLOW_TIMER_IRQn                         _SLOW_TIMER_IRQN(SLOW_TIMER_NUMBER, TIMER_IRQ_SUFFIX)

#define __SLOW_TIMER_RCC_EN(A, N)               RCC_##A##_TIM##N##EN
#define _SLOW_TIMER_RCC_EN(A, N)                __SLOW_TIMER_RCC_EN(A, N)
#define SLOW_TIMER_RCC_EN                       _SLOW_TIMER_RCC_EN(TIMER_SLOW_APBENR, SLOW_TIMER_NUMBER)

#define SLOW_TIMER_PRESCALER                    ((SYS_FREQ_TIMER/100000)-1)
#define SLOW_TIMER_PERIOD                       ((100000/SLOW_TIMER_FREQUENCY)-1)

typedef struct
{
   uint8_t timer_mode;
   uint16_t counter;
   uint16_t compare;
   void (*handler)();
} Slow_Timer_t;

volatile Slow_Timer_t Slow_Timers[SLOW_TIMER_COUNT];

void Slow_Timer_Lock(void)
{
#if SLOW_TIMER_NUMBER > 0
  BIT_BAND_PER(SLOW_TIMER->DIER,TIM_DIER_UIE)=RESET;
#else
  SysTick->CTRL&=~(SysTick_CTRL_TICKINT_Msk);
#endif
}

void Slow_Timer_Unlock(void)
{
#if SLOW_TIMER_NUMBER > 0
  BIT_BAND_PER(SLOW_TIMER->DIER,TIM_DIER_UIE)=SET;
#else
  SysTick->CTRL|=(SysTick_CTRL_TICKINT_Msk);
#endif
}

void Slow_Timer_Delete(int8_t TimerId)
{
  if (TimerId<SLOW_TIMER_COUNT)
  {
    Slow_Timers[TimerId].timer_mode=tm_Not_Change;
    Slow_Timers[TimerId].handler=0;
  }
}

void Init_Slow_Timer(void)
{
  uint8_t i;
  for(i=0;i<SLOW_TIMER_COUNT;i++)
    Slow_Timer_Delete(i);
  #if SLOW_TIMER_NUMBER > 0
    BIT_BAND_PER(RCC->TIMER_SLOW_APBENR,SLOW_TIMER_RCC_EN)=SET;
    SLOW_TIMER->PSC = SLOW_TIMER_PRESCALER;
    SLOW_TIMER->CNT = 0x0000;
    SLOW_TIMER->ARR = SLOW_TIMER_PERIOD;
    NVIC_SetPriority(SLOW_TIMER_IRQn,15);
    NVIC_EnableIRQ(SLOW_TIMER_IRQn);
    BIT_BAND_PER(SLOW_TIMER->EGR,TIM_EGR_UG)=SET;
    BIT_BAND_PER(SLOW_TIMER->SR,TIM_SR_UIF)=SET;
    BIT_BAND_PER(SLOW_TIMER->DIER,TIM_DIER_UIE)=SET;
    BIT_BAND_PER(SLOW_TIMER->CR1,TIM_CR1_CEN)=SET;
  #else
    //SysTick_Config(SystemFrequency/SLOW_TIMER_FREQUENCY);
  #endif
}

void Slow_Timer_Modify(int8_t TimerId, Timer_Mode_t mode, uint16_t counter, uint16_t compare)
{
  if ((TimerId<SLOW_TIMER_COUNT)&&(TimerId>=0))
  {
    Slow_Timer_Lock();
    if (mode!=tm_Not_Change)
      Slow_Timers[TimerId].timer_mode = mode;
    if (counter)
      Slow_Timers[TimerId].counter = counter;
    if (compare)
      Slow_Timers[TimerId].compare = compare;
    Slow_Timer_Unlock();
  }
}

int8_t Slow_Timer_Add(Timer_Mode_t mode, uint16_t compare, void (*handler)())
{
  uint8_t i;
  if(mode!=tm_Not_Change)
  {
    for (i=0;i<SLOW_TIMER_COUNT;i++)
    {
      if (Slow_Timers[i].timer_mode==tm_Not_Change)
      {
        Slow_Timers[i].counter = 1;
        Slow_Timers[i].compare = compare;
        Slow_Timers[i].handler = handler;
        Slow_Timers[i].timer_mode = mode;
        return i;
      }
    }
  }
  return -1;
}

uint16_t Slow_Timer_Get_Counter(int8_t TimerId)
{
  uint16_t counter=0;
  if ((TimerId>=0)&&(TimerId<SLOW_TIMER_COUNT))
  {
    Slow_Timer_Lock();
    counter=Slow_Timers[TimerId].counter;
    Slow_Timer_Unlock();
  }
  return counter;
}

void SLOW_TIMER_IRQHandler(void)
{
  uint8_t i;
  #if SLOW_TIMER_NUMBER > 0
    BIT_BAND_PER(SLOW_TIMER->SR,TIM_SR_UIF)=RESET;
  #endif
  for(i=0;i<SLOW_TIMER_COUNT;i++)
  {
    if(Slow_Timers[i].timer_mode > tm_Stop)
    {
      if(Slow_Timers[i].counter >= Slow_Timers[i].compare)
      {
        Slow_Timers[i].counter = 1;
        if(Slow_Timers[i].timer_mode==tm_Once)
          Slow_Timers[i].timer_mode = tm_Stop;
        if (Slow_Timers[i].handler)
          Slow_Timers[i].handler();
      }
      else
        Slow_Timers[i].counter++;
    }
  }
}
