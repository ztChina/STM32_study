//Ê§°Ü
//Ê§°ÜÔ­Òò£º°´¼ü¶¶¶¯Ì«À÷º¦
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
		OLED_ShowString(3,1,"CNT:");
		OLED_ShowNum(3,5,Get_Counter(),5);
	}
}
