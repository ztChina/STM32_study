#include "stm32f10x.h"                  // Device header
#include "OLED.h"




int main()
{
	OLED_Init();
	
	OLED_ShowChar(1, 1, 'B');
	OLED_ShowString(1, 3, "HelloWorld!");
	OLED_ShowNum(2, 1, 54321, 5);
	OLED_ShowSignedNum(2, 7, -69, 2);
	OLED_ShowHexNum(3, 1, 0xAA55, 4);
	OLED_ShowBinNum(4, 1, 0xAA55, 16);
	while(1)
	{
		
	}
}





