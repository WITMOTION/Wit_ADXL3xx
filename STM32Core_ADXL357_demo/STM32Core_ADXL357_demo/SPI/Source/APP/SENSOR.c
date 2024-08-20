#include "SENSOR.h"
#include "SPI.h"
#include "stdio.h"
#include "string.h"
#include "REG.h"
#include "delay.h"
static SPin CS = {GPIOA, GPIO_Pin_4,GPIO_Mode_Out_PP,GPIO_Speed_50MHz};

signed char Sensor_Init(void)
{
  unsigned char ucReadData,ucWriteData,ucID;
  ucWriteData = 0x52; 

  SPIWriteBytes(CS,0x2F<<1,1,&ucWriteData);
  delay_ms(20);
  SPIReadBytes(CS,(0x00<<1)|0X01,1,&ucID);
  SPIReadBytes(CS,(0x01<<1)|0X01,1,&ucReadData);
  SPIReadBytes(CS,(0x02<<1)|0X01,1,&ucReadData);
  SPIReadBytes(CS,(0x03<<1)|0X01,1,&ucReadData);
  ucWriteData = ucReadData|0X03; 
  SPIWriteBytes(CS,0x2C<<1,1,&ucWriteData);
  ucWriteData = 0x06; 
  SPIWriteBytes(CS,0x2D<<1,1,&ucWriteData);
	delay_ms(20);
//  delay_10ms(10);   
  if( ucID == 0xAD) return 1;
   else return -1;
}
//short sQUIT_TIME = 0;//定时器中断调用自减
char Sensor_GetValue(unsigned char *data)
{
  SPIReadBytes(CS,(0x08<<1)|0X01,9,data);
  return 1;
}





