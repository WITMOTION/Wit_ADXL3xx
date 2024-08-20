#include "SENSOR.h"
#include "SPI.h"
#include "stdio.h"
#include "string.h"
#include "REG.h"
#include "delay.h"
static SPin CS = {GPIOA, GPIO_Pin_4,GPIO_Mode_Out_PP,GPIO_Speed_50MHz};

char Sensor_Init(void)
{
	unsigned char ucWriteData,ucID;
  delay_ms(10);
//  ucWriteData = 0x0f; //16k
  ucWriteData = 0x0e; //16k
  SPIWriteBytes(CS,0x2C,1,&ucWriteData);
  ucWriteData = 0x08;
  SPIWriteBytes(CS,0x2D,1,&ucWriteData);
  ucWriteData = 0x00;
  SPIWriteBytes(CS,0x2E,1,&ucWriteData);
  ucWriteData = 0x0B;
  SPIWriteBytes(CS,0x31,1,&ucWriteData);
   
  SPIReadBytes(CS,0x00|0X80,1,&ucID);
  if( ucID == 0xE5) return 1;
  else return -1;
	
	
}


char Sensor_GetValue(unsigned char *data)
{
  unsigned char ucTemp[6];
//  unsigned char ucReadCnt = 20;
//  while(ucReadCnt--)
//  {
//    SPIReadBytes(0x30|0x80,1,&ucTemp[0],NORMALMODE);
//    if ((ucTemp[0]&0x80) == 0x80) break;
//  }
//  if (ucReadCnt == 0) return 0;
  SPIReadBytes(CS,DATAX0|0xC0,6,&data[0]);
  memcpy(ucTemp, data, 6);
  data[0] = ucTemp[1];
  data[1] = ucTemp[0];
  data[2] = ucTemp[3];

  data[3] = ucTemp[2];
  data[4] = ucTemp[5];
  data[5] = ucTemp[4];
  return 1;
}
char SensorGetAsciiString(unsigned char *ucData)
{
  unsigned char ucReadData[6]={0};
  short sAcc[3];
  float fAcc[3];
  unsigned short ucX[2],ucY[2],ucZ[2];
  unsigned short usIndex = 0;
  unsigned char ucStrLen = 0;

  if(Sensor_GetValue(ucReadData) == 0)return 0;
 
    sAcc[0]=((( short)ucReadData[0]) << 8) | ucReadData[1];
    sAcc[1]=((( short)ucReadData[2]) << 8) | ucReadData[3];
    sAcc[2]=((( short)ucReadData[4]) << 8) | ucReadData[5];
    fAcc[0]=(float)sAcc[0]/20.5*100;
    fAcc[1]=(float)sAcc[1]/20.5*100;
    fAcc[2]=(float)sAcc[2]/20.5*100;
    
    memcpy(&ucData[0], "AccX:", 5);
    usIndex+=5;
    if(fAcc[0] < 0)
    {
      ucData[usIndex] = '-';
      usIndex+=1;
      ucX[0]=-(short)fAcc[0]/100;
      ucX[1]=-(short)fAcc[0]%100;
    }
    else
    {
      ucX[0]=(short)fAcc[0]/100;
      ucX[1]=(short)fAcc[0]%100;
    }
    ucStrLen = sprintf((char*)&ucData[usIndex],"%d.%d G,",ucX[0],ucX[1]);
    usIndex+=ucStrLen;
    memcpy(&ucData[usIndex], "AccY:", 5);
    usIndex+=5;
    if(fAcc[1] < 0)
    {
      ucData[usIndex] = '-';
      usIndex+=1;
      ucY[0]=-(short)fAcc[1]/100;
      ucY[1]=-(short)fAcc[1]%100;
    }
    else
    {
      ucY[0]=(short)fAcc[1]/100;
      ucY[1]=(short)fAcc[1]%100;
    }
    ucStrLen = sprintf((char*)&ucData[usIndex],"%d.%d G,",ucY[0],ucY[1]);
    usIndex+=ucStrLen;
    memcpy(&ucData[usIndex], "AccZ:", 5);
    usIndex+=5;
    if(fAcc[2] < 0)
    {
      ucData[usIndex] = '-';
      usIndex+=1;
      ucZ[0]=-(short)fAcc[2]/100;
      ucZ[1]=-(short)fAcc[2]%100;
    }
    else
    {
      ucZ[0]=(short)fAcc[2]/100;
      ucZ[1]=(short)fAcc[2]%100;
    }
    ucStrLen = sprintf((char*)&ucData[usIndex],"%d.%d G\r\n",ucZ[0],ucZ[1]);

  return 1;
  
}



