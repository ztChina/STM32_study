#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

uint8_t RxData = 0;

int main()
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	
	while(1)
	{
		
		/*寄存器直接读取方式
		if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET )
		{
			RxData = USART_ReceiveData(USART1);
			OLED_ShowHexNum(1, 1, RxData, 2);
		}
		*/
		
		//中断方式
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1, 1, RxData, 2);
		}
	}
}
