#ifndef __SENSOR_H
#define __SENSOR_H

#define DATAX0      0x32
#define DATAX1      0x33
#define DATAY0      0x34
#define DATAY1      0x35
#define DATAZ0      0x36
#define DATAZ1      0x37

char Sensor_Init(void);
char SensorGetAsciiString(unsigned char *ucData);

#endif
