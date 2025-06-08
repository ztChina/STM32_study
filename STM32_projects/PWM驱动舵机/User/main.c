//利用PWM实现控制舵机，舵机角度与PWM占空比有关
//利用PA5按键控制角度
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Servo.h"
#include "Key.h"

uint8_t angle=0;


int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	OLED_ShowString(2, 1, "Angle:");
	Servo_Init();
	Key_Init();
	while(1)
	{
		if (Key_GetNum()==1)
		{
			angle+=30;
			if (angle>180)
			{
				angle=0;
			}
			Servo_SetAngle(angle);
		}
		OLED_ShowNum(2,7,angle,5);
	}
}
