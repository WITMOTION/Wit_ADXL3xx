#include "IO.h"
#include "stm32f10x_rcc.h"

static SPin LED = {GPIOB, GPIO_Pin_7,GPIO_Mode_Out_PP,GPIO_Speed_2MHz};
void DIO_Initial(SPin stcPin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,  ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);  
	//GPIO_Remap_SWJ_JTAGDisable: PA15-JTDI PB3-JTDO PB4-JNTRST 
	//GPIO_Remap_SWJ_Disable: PA13-SWDIO PA14-SWCLK
	if (stcPin.GPIOx == GPIOB)
	{
		if ((stcPin.GPIO_Pin == GPIO_Pin_3)|(stcPin.GPIO_Pin == GPIO_Pin_4))
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	}
	if (stcPin.GPIOx == GPIOA)
	{
		if (stcPin.GPIO_Pin == GPIO_Pin_15)
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	}	
	if (stcPin.GPIOx == GPIOA)
	{
		if ((stcPin.GPIO_Pin == GPIO_Pin_13)|(stcPin.GPIO_Pin == GPIO_Pin_14))
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	}	
	
	GPIO_StructInit(&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Mode = stcPin.GPIO_Mode;
	GPIO_InitStructure.GPIO_Speed = stcPin.GPIO_Speed;
	GPIO_InitStructure.GPIO_Pin =  stcPin.GPIO_Pin;
	GPIO_Init(stcPin.GPIOx, &GPIO_InitStructure);	
}	

void WriteDO(SPin stcPin,unsigned char ucData)
{
	if (ucData>0)
		stcPin.GPIOx->BSRR = stcPin.GPIO_Pin;
	else
		stcPin.GPIOx->BRR = stcPin.GPIO_Pin;
}
unsigned char ReadDI(SPin stcPin)
{
	return (stcPin.GPIOx->IDR&stcPin.GPIO_Pin)==stcPin.GPIO_Pin;
}
void LED_Init(void)
{	
	DIO_Initial(LED);
}
void LED_STATE(unsigned char State)
{
	if(State==1) WriteDO(LED, 1);
	else WriteDO(LED, 0);
}
