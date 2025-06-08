//利用PWM实现LED呼吸灯，呼吸灯即起伏改变PWM占空比
//LED连接PA0，高电平点亮
//失败原因：没有LED灯，玩不了
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "Delay.h"

uint8_t i;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	PWM_Init();
	while(1)
	{
		for(i=0;i<=100;i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for(i=0;i<=100;i++)
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
		}
	}
}
