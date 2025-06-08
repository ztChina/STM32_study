//����PWMʵ��LED�����ƣ������Ƽ�����ı�PWMռ�ձ�
//LED����PA0���ߵ�ƽ����
//ʧ��ԭ��û��LED�ƣ��治��
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
