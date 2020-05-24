
#ifndef _WS2812_H_
#define _WS2812_H_


/*******************************通信接口配置***********************************/
#include "sys.h"


#define MaxRGBLium 255
#define RGBIndex 8

#define RGBCH1 PDout(2)
#define RGBCH2 PBout(3)
#define RGBCH3 PBout(4)
#define RGBCH4 PBout(5)


extern float RGBTAG[RGBIndex][3];
extern float RGBRAM[RGBIndex][3];
void SK6812_Write(u8 data1,u8 data2,u8 data3);
void SK6812_Wheel(u8 Type,u16 WheelPos);
void SK6812_WheelS(u8 Type,u16 WheelPos);
void SK6812_send_L(void);
void SK6812_send_H(void);
void SK6812_Refrash(void);
void SK6812_Flow(void);
void SK6812_Same(u8 data1,u8 data2,u8 data3);
void SK6812_Init(void);
void SK6812_WheelAll(u16 WheelPos);

#endif

//-------------------------THE END------------------------*/   

