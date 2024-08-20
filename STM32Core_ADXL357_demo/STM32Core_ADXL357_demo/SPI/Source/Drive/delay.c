#include "stm32f10x.h"

#include "delay.h"

volatile unsigned long ulTick = 0;
void SysTick_Init(void)
{
	SysTick->LOAD = SystemCoreClock/7200-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x03;//SysTick
}

void delay_ms(unsigned long nTime)
{
		SysTick->LOAD = SystemCoreClock/7200-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x03;//SysTick
    ulTick = 0;
    for(;ulTick <= nTime;);
}
void delay_us(unsigned long nTime)
{
	SysTick->LOAD = SystemCoreClock/7200000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x03;//SysTick
	ulTick=0;
	for(;ulTick <= nTime;);
}
void SysTick_Handler(void)
{
	ulTick++;
}
