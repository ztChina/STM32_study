#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

int main()
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	
	uint8_t data=0x41;
	Serial_SendByte(data);
	
	uint8_t MyArray[] = { 0x42,0x43,0x44,0x45 };
	Serial_SendArray(MyArray,4);
	
	Serial_SendString("Hello World\r\n");
	
	printf("Num=%d\r\n",666);
	
	Serial_Printf("Num=%d\r\n",666);
	
	while(1)
	{
		
	}
}
