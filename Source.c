#include "Header.h"

int main()
{
	DisplayData[0] = DIG_CODE[2];
	DisplayData[1] = DIG_CODE[0];
	DisplayData[2] = DIG_CODE[2];
	DisplayData[3] = DIG_CODE[0];
	DisplayData[4] = DIG_CODE[0];
	DisplayData[5] = DIG_CODE[0];
	TMOD = 0x01;//设定工作方式为16位定时器
	TH0 = (65536 - 40000) / 256;//赋定时器T0高四的值为40MS（对256求模）  
	TL0 = (65536 - 40000) % 256;//赋定时器T0低四的值为40MS（对256求余）  
	EA = 1;//开总中断
	ET0 = 1;//开定时器T0中断  
	TR0 = 1;//启动定时器T0
	EastGreen = 1;
	EastYellow = 1;
	EastRed = 0;
	NorthGreen = 1;
	NorthYellow = 1;
	NorthRed = 0;
	NorthSidewalkGreen = 1;
	NorthSidewalkRed = 0;
	EastSidewalkGreen = 1;
	EastSidewalkRed = 0;
	do
	{
		Delay200ms();
		EastRed = !EastRed;
		NorthRed = !NorthRed;
		NorthSidewalkRed = !NorthSidewalkRed;
		EastSidewalkRed = !EastSidewalkRed;
	} while (--TempCounter < 5);
	EastGreen = 0;
	EastRed = 1;
	NorthRed = 0;	
	NorthSidewalkRed = 0;
	EastSidewalkGreen = 0;
	EastSidewalkRed = 1;
	while (1)
		DigDisplay();
}