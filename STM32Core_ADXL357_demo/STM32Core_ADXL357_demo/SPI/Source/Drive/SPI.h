#ifndef _SPI_H
#define _SPI_H
#include "stm32f10x_spi.h"
#include "IO.h"

void SPIWriteBytes(SPin PinCs,unsigned char ucAddr,unsigned char ucLength,unsigned char ucData[]);
void SPIReadBytes(SPin PinCs,unsigned char ucAddr,unsigned char ucLength,unsigned char ucData[]);
void SPI_Initial(unsigned short usCPOL,unsigned short usCPHA);

#endif


