/*
用STM32Core平台SPI接口读取ADXL的数据，然后通过串口1打印到串口助手。
接线
USB-TTL工具                 STM32Core              
VCC          -----           VCC        ----        VCC
TX           -----           RX1
RX           -----           TX1
GND          -----           GND        ----        GND
                             PA4        ----        CS
							 PA5        ----        SCK
                             PA6        ----        MISO
							 PA7        ----        MOSI
------------------------------------
*/
 
#include <string.h>
#include <stdio.h>
#include "Main.h"
#include "REG.h"
#include "UART1.h"
#include "delay.h"
#include "SPI.h"
#include "IO.h"
#include "SENSOR.h"

#define MAXBUFF 128
#define ID_MAX    (int)0xfe
#define PRATE_MAX 10000
static unsigned char ucReceiveData[MAXBUFF];
static unsigned char ucReceiveFinish = 0;
static unsigned short usReceiveDataLen = 0;
unsigned char ucLastDataInterval = 0;
unsigned char ucRxTimeOut = 2;
unsigned char ucOutPutOneFlag = 0;

void ShortToChar(short sData,unsigned char cData[])
{
    cData[0]=sData&0xff;
    cData[1]=sData>>8;
}
short CharToShort(unsigned char cData[])
{
    return ((short)cData[1]<<8)|cData[0];
}

static SPin CS = {GPIOA, GPIO_Pin_4,GPIO_Mode_Out_PP,GPIO_Speed_50MHz};
unsigned char chrTemp[30];
char Compare(unsigned char *chrBuf,unsigned char *chrTemplet,unsigned short usLength)
{
	unsigned short i;
	for (i=0;i<usLength;i++)
		if (chrBuf[i]!=chrTemplet[i]) return 0;
	return 1;
}
char GetParam(unsigned char *buf,unsigned short *usParam,char cParamMaxLen)
{
  char i,j;
  unsigned short usTemp = 0;
  for(i=0;i<100;i++)
    if((buf[i]>'9')||(buf[i]<'0'))
      break;
  if((buf[i]!=0x0D)&&(buf[i+1]!=0x0A))return 0;
  if((i==0)||(i>cParamMaxLen))return 0;
  for(j=0;j<i;j++)
    usTemp = usTemp*10+(buf[j]-'0');
  *usParam = usTemp;
  return 1;
}


int main(void)
{
  long lAcc[3];
  float Acc[3];
	unsigned char i=0,ucReadData[9]={0};
	SysTick_Init();
	delay_ms(10);
	DIO_Initial(CS);
	WriteDO(CS, 1);
	SPI_Initial(SPI_CPOL_Low, SPI_CPHA_2Edge);
	Initial_UART1(115200);
	if(Sensor_Init()==1)
		printf("Sensor Init ok\r\n");
	memset(ucReceiveData,0,MAXBUFF);
	usReceiveDataLen = 0;
	while (1)
	{
//		CheckReceiveData();
		delay_ms(200);
			
			if(Sensor_GetValue(ucReadData)==0)
			{
				printf("Read Fail");
				continue;
			}
			lAcc[0]=((long)ucReadData[0]<<12) | (long)(ucReadData[1]<<4) | (ucReadData[2]>>4);
			lAcc[1]=((long)ucReadData[3]<<12) | (long)(ucReadData[4]<<4) | (ucReadData[5]>>4);
			lAcc[2]=((long)ucReadData[6]<<12) | (long)(ucReadData[7]<<4) | (ucReadData[8]>>4); 
			for(i=0;i<3;i++)
			{
				if((lAcc[i] & 0x80000))
					lAcc[i] |=0xfff00000;
			}
			Acc[0]=(float)((lAcc[0]*40)/524288.0);
			Acc[1]=(float)((lAcc[1]*40)/524288.0);
			Acc[2]=(float)((lAcc[2]*40)/524288.0);
			printf("AccX:%d mG,AccY:%d mG,AccZ:%d mG\r\n",(int)(Acc[0]*1000),(int)(Acc[1]*1000),(int)(Acc[2]*1000));


	}
}



