#include "Header.h"

int main()
{
	DisplayData[0] = DIG_CODE[2];
	DisplayData[1] = DIG_CODE[0];
	DisplayData[2] = DIG_CODE[2];
	DisplayData[3] = DIG_CODE[0];
	DisplayData[4] = DIG_CODE[0];
	DisplayData[5] = DIG_CODE[0];
	TMOD = 0x01;//�趨������ʽΪ16λ��ʱ��
	TH0 = (65536 - 40000) / 256;//����ʱ��T0���ĵ�ֵΪ40MS����256��ģ��  
	TL0 = (65536 - 40000) % 256;//����ʱ��T0���ĵ�ֵΪ40MS����256���ࣩ  
	EA = 1;//�����ж�
	ET0 = 1;//����ʱ��T0�ж�  
	TR0 = 1;//������ʱ��T0
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