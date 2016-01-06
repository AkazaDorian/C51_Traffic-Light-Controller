//????????
//?????????? ?????? ??1403 ???
//P0 ? P1^6.7 ???????
//P1^1~3 ??? 38 ????????????
//P2 ?????????????????(0 ~ 99)
//P3 ???????????????
//????????:1 ??,0 ??,?? 0~7 ???????????
//K1,K5 ??????????;K2,K6 ??????????
//????,?????????????????,??????? 0.2 s

#include <REG52.H>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define GPIO_DIG P1

uchar code DIG_CODE[17] = {
	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
	0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71 };//0?1?2?3?4?5?6?7?8?9?A?b?C?d?E?F????
uchar DisplayData[6];//????????4????
uchar LastPressed = 0;//? 0 ???????????,? 1 ????
uchar PressTime = 5;//????????,LastPressed ? 0 ?? 200 ms,? 1 ?? 120 ms,? 2 ?? 80 ms
uchar BreakTime = 0;//????,??? PressTime ????????
uchar GreenTime = 20;//?????????,??????????
uchar RedTime = 20;//?????????,??????????
uchar Time = 21;//?????
uchar EastPass = 1;//? 1 ??????,? 0 ??????
uchar SecondTime = 0;//? 5 ??? 1 ?,Time + 1
uchar BlinkTime = 0;//??????,? 5 ???????
uchar TempCounter = 5;

sbit EastGreen = P0 ^ 0;
sbit EastYellow = P0 ^ 1;
sbit EastRed = P0 ^ 2;
sbit NorthGreen = P0 ^ 3;
sbit NorthYellow = P0 ^ 4;
sbit NorthRed = P0 ^ 5;
sbit NorthSidewalkGreen = P0 ^ 6;
sbit NorthSidewalkRed = P0 ^ 7;
sbit EastSidewalkGreen = P2 ^ 1;
sbit EastSidewalkRed = P2 ^ 0;

sbit GreenUp = P3 ^ 1;
sbit GreenDown = P3 ^ 5;
sbit RedUp = P3 ^ 2;
sbit RedDown = P3 ^ 6;

sbit LSA = P2 ^ 5;
sbit LSB = P2 ^ 6;
sbit LSC = P2 ^ 7;

void DigDisplay()
{
	uchar i;
	uint j;
	for (i = 0; i<6; i++)
	{
		switch (i)	 //??,????????,
		{
		case(0) :
			LSA = 0; LSB = 0; LSC = 0; break;//???1?
		case(1) :
			LSA = 1; LSB = 0; LSC = 0; break;//???2?
		case(2) :
			LSA = 0; LSB = 1; LSC = 1; break;//???5?
		case(3) :
			LSA = 1; LSB = 1; LSC = 1; break;//???6?
			case(4):
				LSA=1;LSB=1;LSC=0; break;//???3?
			case(5):
				LSA=0;LSB=0;LSC=1; break;//???4?
		}
		GPIO_DIG = DisplayData[i];//????
		j = 10;						 //????????
		while (j--);
		GPIO_DIG = 0x00;//??
	}
}

uchar GetKey()
{
	if (!GreenUp)
		return 1;
	else if (!GreenDown)
		return 2;
	else if (!RedUp)
		return 3;
	else if (!RedDown)
		return 4;
	else
		return 0;
}

void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	i = 2;
	j = 103;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void timer0()interrupt 1
{
	TH0 = (65536 - 40000) / 256;//????T0?????40MS(?256??)  
	TL0 = (65536 - 40000) % 256;//????T0?????40MS(?256??)  
	if (++BlinkTime == 5)
	{
		BlinkTime = 0;
		if(GreenTime && RedTime)
		{
			if(Time == 1 && SecondTime == 4)
				if (EastPass)
				{
					NorthRed = 1;
					NorthGreen = 0;
						NorthSidewalkGreen = 0;
					NorthSidewalkRed = 1;
					EastRed = 0;
					EastGreen = 1;
					EastYellow = 1;
					EastSidewalkGreen = 1;
					EastSidewalkRed = 0;
					EastPass = 0;
					Time = RedTime + 1;
				}
				else
				{
					NorthRed = 0;
					NorthGreen = 1;
					NorthYellow = 1;
					NorthSidewalkGreen = 1;
					NorthSidewalkRed = 0;
					EastRed = 1;
					EastGreen = 0;
					EastSidewalkGreen = 0;
					EastSidewalkRed = 1;
					EastPass = 1;
					Time = GreenTime + 1;
				}
			if (EastPass)
			{
				if (Time <= 5)
				{
					EastGreen = 1;
					EastYellow = !EastYellow;
				}
			}
			else
			{
				if (Time <= 5)
				{
					NorthGreen = 1;
					NorthYellow = !NorthYellow;
				}
			}
			if (++SecondTime == 5)
			{
				SecondTime = 0;
				--Time;
				DisplayData[4] = DIG_CODE[Time / 10];
				DisplayData[5] = DIG_CODE[Time % 10];
			}
		}
		else if(GreenTime != 0 && RedTime == 0)
			{
				DisplayData[4] = DIG_CODE[GreenTime / 10];
				DisplayData[5] = DIG_CODE[GreenTime % 10];
				EastPass = 1;
					NorthRed = 0;
					NorthGreen = 1;
					NorthYellow = 1;
					NorthSidewalkGreen = 1;
					NorthSidewalkRed = 0;
					EastRed = 1;
					EastGreen = 0;
					EastYellow = 1;
					EastSidewalkGreen = 0;
					EastSidewalkRed = 1;
				Time = GreenTime;
			}
		else if(RedTime != 0 && GreenTime == 0)
			{
				DisplayData[4] = DIG_CODE[RedTime / 10];
				DisplayData[5] = DIG_CODE[RedTime % 10];
				EastPass = 0;
					NorthRed = 1;
					NorthGreen = 0;
					NorthYellow = 1;
					NorthSidewalkGreen = 0;
					NorthSidewalkRed = 1;
					EastRed = 0;
					EastGreen = 1;
					EastYellow = 1;
					EastSidewalkGreen = 1;
					EastSidewalkRed = 0;
				Time = RedTime;
			}
		else if(!(GreenTime || RedTime))
			{
				DisplayData[4] = DIG_CODE[9];
				DisplayData[5] = DIG_CODE[9];
					NorthRed = 0;
					NorthGreen = 1;
					NorthYellow = 1;
					NorthSidewalkGreen = 1;
					NorthSidewalkRed = 0;
					EastRed = 0;
					EastGreen = 1;
					EastYellow = 1;
					EastSidewalkGreen = 1;
					EastSidewalkRed = 0;
			}
	}
	OutOfIf:
	if (++BreakTime == PressTime)
	{
		BreakTime = 0;
		switch (GetKey())
		{
		case(0) :
			goto NoKeyGot;
		case(1) :
			if (GreenTime < 99)
			{
				++GreenTime;
				DisplayData[0] = DIG_CODE[GreenTime / 10];
				DisplayData[1] = DIG_CODE[GreenTime % 10];
			}
				break;
		case(2) :
			if (GreenTime > 0)
			{
				--GreenTime;
				DisplayData[0] = DIG_CODE[GreenTime / 10];
				DisplayData[1] = DIG_CODE[GreenTime % 10];
			}
				break;
		case(3) :
			if (RedTime < 99)
			{
				++RedTime;
				DisplayData[2] = DIG_CODE[RedTime / 10];
				DisplayData[3] = DIG_CODE[RedTime % 10];
			}
				break;
		case(4) :
			if (RedTime > 0)
			{
				--RedTime;
				DisplayData[2] = DIG_CODE[RedTime / 10];
				DisplayData[3] = DIG_CODE[RedTime % 10];
			}
				break;
		}
	}
	else
		return;
	switch (LastPressed)
	{
	case (0) :
		PressTime = 8;
		++LastPressed;
		break;
	case(1) :
		PressTime = 5;
		++LastPressed;
		break;
	case(2) :
		PressTime = 3;
		break;
	}
	return;
NoKeyGot:
	LastPressed = 0;
	PressTime = 1;
	return;
}