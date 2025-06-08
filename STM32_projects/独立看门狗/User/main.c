#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	OLED_ShowString(1, 1, "IWDG TEST");
	
	
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)	//看门狗复位时会触发
	{
		OLED_ShowString(2, 1, "IWDGRST");
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
	
	
	//配置看门狗，1000ms
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_16);	//配置分频器
	IWDG_SetReload(2499);					//配置重载数值
	IWDG_ReloadCounter();		//先喂一次狗
	IWDG_Enable();	//启用看门狗
	
	while (1)
	{
		Key_GetNum();	//设计一直按住会卡在这里
		
		//喂狗
		IWDG_ReloadCounter();
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}
}
