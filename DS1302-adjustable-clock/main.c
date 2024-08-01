#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Delay.h"
#include "Key.h"
#include "Timer0.h"

unsigned char KeyNum,MODE,TimeSetSelect,TimeSetFlashFlag;

void TimeShow()
{
		DS1302_ReadTime();
		LCD_ShowNum(1,1,DS1302_Time[0],2);//年
		LCD_ShowNum(1,4,DS1302_Time[1],2);//月
		LCD_ShowNum(1,7,DS1302_Time[2],2);//日
		LCD_ShowNum(2,1,DS1302_Time[3],2);//时
		LCD_ShowNum(2,4,DS1302_Time[4],2);//分
		LCD_ShowNum(2,7,DS1302_Time[5],2);//秒
}

void TimeSet()
{
	if(KeyNum==2)
	{
		TimeSetSelect++;
		TimeSetSelect%=6;
	}
	if(KeyNum==3)
	{
		DS1302_Time[TimeSetSelect]++;
		if(DS1302_Time[0]>99){DS1302_Time[0]=0;}//年越界判断
		if(DS1302_Time[1]>12){DS1302_Time[1]=1;}//月越界判断
		//大月31天
		if(DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 ||
			DS1302_Time[1]==7 || DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)
		{
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
		}
		//小月30天
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 ||
			DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
		}
		//2月29或28天
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)//平闰年判断
			{
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
			}
			else
			{
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
			}
		}
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}//时越界判断
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}//分越界判断
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}//秒越界判断
	}
	if(KeyNum==4)
	{
		DS1302_Time[TimeSetSelect]--;
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}//年越界判断
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}//月越界判断
		//大月31天
		if(DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 ||
			DS1302_Time[1]==7 || DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=31;}
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
		}
		//小月30天
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 ||
			DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=30;}
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
		}
		//2月29或28天
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)//平闰年判断
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=29;}
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
			}
			else
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=28;}
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
			}
		}
		if(DS1302_Time[3]<1){DS1302_Time[3]=23;}//时越界判断
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}//分越界判断
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}//秒越界判断
	}
	//更新显示
	//year
	if(TimeSetSelect==0 && TimeSetFlashFlag==1){LCD_ShowString(1,1,"  ");}
	else{LCD_ShowNum(1,1,DS1302_Time[0],2);}//年
	//month
	if(TimeSetSelect==1 && TimeSetFlashFlag==1){LCD_ShowString(1,4,"  ");}
	else{LCD_ShowNum(1,4,DS1302_Time[1],2);}//月
	//date
	if(TimeSetSelect==2 && TimeSetFlashFlag==1){LCD_ShowString(1,7,"  ");}
	else{LCD_ShowNum(1,7,DS1302_Time[2],2);}//日
	//hour
	if(TimeSetSelect==3 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");}
	else{LCD_ShowNum(2,1,DS1302_Time[3],2);}//时
	//minute
	if(TimeSetSelect==4 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}
	else{LCD_ShowNum(2,4,DS1302_Time[4],2);}//分
	//second
	if(TimeSetSelect==5 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");}
	else{LCD_ShowNum(2,7,DS1302_Time[5],2);}//秒
	
	//LCD_ShowNum(2,10,TimeSetSelect,2);
	//LCD_ShowNum(2,13,TimeSetFlashFlag,2);
}

void main()
{
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1,1,"  -  -  ");
	LCD_ShowString(2,1,"  :  :  ");

  
	DS1302_SetTime();
	
	while(1)
	{
		KeyNum=Key();
		if(KeyNum==1)
		{
			if(MODE==0){MODE=1;TimeSetSelect=0;}
			else if(MODE==1){MODE=0;DS1302_SetTime();}
		}
		switch(MODE)
		{
			case 0:TimeShow();break;
			case 1:TimeSet();break;
		}
	}
}

void Timer0_Rountine() interrupt 1
{
	static unsigned int T0Count;	//静态变量，退出函数后不丢失值
	T0Count++;
	TL0 = 0x18;		//重新赋初值，防止溢出后从0开始计数
	TH0 = 0xFC;
	if(T0Count>=500)	//每隔0.5秒执行一次
	{
		T0Count = 0;
		TimeSetFlashFlag=!TimeSetFlashFlag;
	}
}
