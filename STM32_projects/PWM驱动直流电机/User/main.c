//����PWMʵ�ֿ�������ֱ�������PWMռ�ձȿ����ٶ�
//���������ٶ�
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Driver.h"
#include "Key.h"

int8_t Speed=0;
uint8_t Num=0;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	OLED_ShowString(2, 1, "Speed:");
	Driver_Init();
	while(1)
	{
		Num=Key_GetNum();
		if(Num==1)
		{
			Speed+=20;
			if (Speed>100)
			{
				Speed=-100;
			}
			Driver_SetSpeed(Speed);
		}
		OLED_ShowSignedNum(2,7,Speed,4);
	}
}
