//利用计时器计时，每一秒钟中断一次num+1
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"

uint32_t num=0;

int main()
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	while(1)
	{
		OLED_ShowString(2,1,"Num:");
		OLED_ShowNum(2,5,num,5);
	}
}
