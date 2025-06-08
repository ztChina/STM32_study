#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	OLED_ShowString(1, 1, "IWDG TEST");
	
	
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)	//���Ź���λʱ�ᴥ��
	{
		OLED_ShowString(2, 1, "IWDGRST");
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
	
	
	//���ÿ��Ź���1000ms
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//���д����
	IWDG_SetPrescaler(IWDG_Prescaler_16);	//���÷�Ƶ��
	IWDG_SetReload(2499);					//����������ֵ
	IWDG_ReloadCounter();		//��ιһ�ι�
	IWDG_Enable();	//���ÿ��Ź�
	
	while (1)
	{
		Key_GetNum();	//���һֱ��ס�Ῠ������
		
		//ι��
		IWDG_ReloadCounter();
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}
}
