#include <REGX52.H>

/**
  * @brief	定时器0初始化，1毫秒@12.000MHz
  * @param	无参数传入
  * @retval	无返回值
  */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}

/*
定时器中断函数模板
void Timer0_Rountine() interrupt 1
{
	static unsigned int T0Count;	//静态变量，退出函数后不丢失值
	T0Count++;
	TL0 = 0x18;		//重新赋初值，防止溢出后从0开始计数
	TH0 = 0xFC;
	if(T0Count>=1000)	//每隔一秒执行一次
	{
		T0Count = 0;
		
	}
}
*/