//ʧ��
//ʧ��ԭ�򣺸��ÿ�©/�������ģʽ��������EXTI�ĸ���GPIO����
//EXTI��GPIO���������֣�����������������������������

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
