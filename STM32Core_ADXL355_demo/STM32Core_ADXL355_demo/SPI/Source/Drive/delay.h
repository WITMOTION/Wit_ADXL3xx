#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"

void SysTick_Init(void);
void delay_ms(unsigned long nTime);
void delay_us(unsigned long nTime);

#endif

//------------------End of File----------------------------
