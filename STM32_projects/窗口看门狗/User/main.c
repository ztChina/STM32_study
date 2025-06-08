#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	OLED_ShowString(1, 1, "WWDG TEST");
	
	
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)	//���Ź���λʱ�ᴥ��
	{
		OLED_ShowString(2, 1, "WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();	//�����λ��־λ
	}
	else	//��һ�����к�������λʱ�ᴥ��
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	
	//���ô��ڿ��Ź�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	//���ڿ��Ź�ʱ��
	WWDG_SetPrescaler(WWDG_Prescaler_8);	//���÷�Ƶֵ
	WWDG_SetWindowValue(0x40 | 21);		//��������ֵ20ms��������30ms
	WWDG_Enable(0x40 | 54);				//��������ֵ50ms����ʹ�� ���Ź�
	
	
	while (1)
	{
		Key_GetNum();	//���һֱ��ס�Ῠ������
		
		//ι��
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		WWDG_SetCounter(0x40 | 54);
		
	}
}
