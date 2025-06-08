//失败
//失败原因：复用开漏/推挽输出模式不能用于EXTI的复用GPIO配置
//EXTI的GPIO配置有三种：浮空输入或带上拉输入或带下拉输入

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include "CountSensor.h"
#include "Delay.h"

int main()
{
	Key_Init();
	OLED_Init();
	CountSensor_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	while(1)
	{
		if (Key_GetNum() == 1)
		{
			OLED_ShowString(2, 1, "put one");
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
			Delay_ms(1000);
		}
	}
}
