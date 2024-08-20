#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "SPI.h"
#include "IO.h"
#include "delay.h"
#define SPIN SPI1

SPin SCK  = {GPIOA,GPIO_Pin_5,GPIO_Mode_AF_PP,GPIO_Speed_50MHz};
SPin MISO = {GPIOA,GPIO_Pin_6,GPIO_Mode_AF_PP,GPIO_Speed_50MHz};
SPin MOSI = {GPIOA,GPIO_Pin_7,GPIO_Mode_AF_PP,GPIO_Speed_50MHz};




unsigned char SPIReadWriteByte(unsigned char TxData)                                       //SPI读写数据函数
{
	unsigned char ret=0;
	while(SPI_I2S_GetFlagStatus(SPIN, SPI_I2S_FLAG_TXE) == RESET)
	{
		ret++;
		if(ret>200) return 0;
	}
//	*(unsigned char*)&SPIN->DR = TxData;
		SPI_I2S_SendData(SPIN, TxData);
	ret=0;
	while(SPI_I2S_GetFlagStatus(SPIN, SPI_I2S_FLAG_RXNE) == RESET)
	{
		ret++;
		if(ret>200) return 0;
	}
//	return *(unsigned char*)&SPIN->DR;
	return SPI_I2S_ReceiveData(SPIN);
}

void SPIWriteBytes(SPin PinCs,unsigned char usAddr,unsigned char ucLength,unsigned char ucData[])
{
	unsigned short i;
	WriteDO(PinCs,0);
    i = 20;//CS 等待
    while(i--);
//	delay_us(100);
	SPIReadWriteByte(usAddr);
	for (i=0;i<ucLength;i++)
	SPIReadWriteByte(ucData[i]);
	    i = 100;//CS 等待
    while(i--);
	WriteDO(PinCs,1);
}
void SPIReadBytes(SPin PinCs,unsigned char usAddr,unsigned char ucLength,unsigned char ucData[])
{
	unsigned short i;
	WriteDO(PinCs,0);
    i = 20;//CS 等待
    while(i--);
//	delay_us(100);
	SPIReadWriteByte(usAddr);
	for (i=0;i<ucLength;i++)
	ucData[i] = SPIReadWriteByte(0);
	    i = 100;//CS 等待
    while(i--);
	WriteDO(PinCs,1);
}
void SPI_Initial(unsigned short usCPOL,unsigned short usCPHA)
{
	SPI_InitTypeDef SPI_InitStruct;
	DIO_Initial(SCK );
	DIO_Initial(MOSI);
	DIO_Initial(MISO);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
//	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = usCPOL;
	SPI_InitStruct.SPI_CPHA = usCPHA;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1,ENABLE);
	SPIReadWriteByte(0xff); 
}

