#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(1, 1, "SYSCLK:");
	// SystemCoreClock 在文件 system_stm32f10x.c 中，用来修改代码主频
	// SystemCoreClock 是一个配置值，不是晶振参数值
	OLED_ShowNum(1, 8, SystemCoreClock, 8);
	
	while (1)
	{
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
}
