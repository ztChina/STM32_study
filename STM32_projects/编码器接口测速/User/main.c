//ʵ�����ֲ���
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Delay.h"

int16_t encoder_get_num=0;
int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	Encoder_Init();
	while(1)
	{
		OLED_ShowSignedNum(2,1,Encoder_Speed(),5);
		Delay_ms(1000);//Delay�����谭������̣�����ʹ����һ����ʱ���Ķ�ʱ�жϺ����Ż���
	}
}
