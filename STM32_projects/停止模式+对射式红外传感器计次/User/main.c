#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
	OLED_Init();
	CountSensor_Init();
	
	//停止模式涉及到 PWR 配置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//
	
	OLED_ShowString(1, 1, "Count:");
	
	while (1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);	//进入停止模式
		SystemInit();	//由于退出停止模式时，时钟使用的是HSI，为了重新使用SYSCLK，需要重新初始化
	}
}
