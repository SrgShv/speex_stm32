/*
**
**                           timer.h
**
**
**********************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

typedef unsigned int    DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;

extern void IRQ_timer(void);

void TIM2_Config(void);
void TIM2_IRQHandler(void);
void NVIC_Config(void);
void TIM2_Start(void);
void TIM2_Stop(void);

extern void debugBlinkGreenLed(const int divScaler);


#endif /* __TIMER_H */
