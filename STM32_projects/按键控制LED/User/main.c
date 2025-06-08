#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Key.h"
#include "Delay.h"

int main()
{
	LED_Init();
	Key_Init();
	while(1)
	{
		if(Key_GetNum()==1)
		{
			LED_ON();
			Delay_ms(1000);
		}
		else if(Key_GetNum()==0)
			{
				LED_OFF();
				Delay_ms(1000);
			}
	}
}
