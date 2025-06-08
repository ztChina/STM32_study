#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	OLED_ShowString(1, 1, "WWDG TEST");
	
	
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)	//看门狗复位时会触发
	{
		OLED_ShowString(2, 1, "WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();	//清除复位标志位
	}
	else	//第一次运行和其它复位时会触发
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	
	//配置窗口看门狗
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	//窗口看门狗时钟
	WWDG_SetPrescaler(WWDG_Prescaler_8);	//配置分频值
	WWDG_SetWindowValue(0x40 | 21);		//配置下限值20ms，窗口是30ms
	WWDG_Enable(0x40 | 54);				//配置上限值50ms，并使能 看门狗
	
	
	while (1)
	{
		Key_GetNum();	//设计一直按住会卡在这里
		
		//喂狗
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		WWDG_SetCounter(0x40 | 54);
		
	}
}
