//通过PA6检测电池经过线性降压模块后的电压
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "Delay.h"

uint16_t ADValue=0;
float Voltage=0.0;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	OLED_ShowString(2,1,"Bat_V:");
	OLED_ShowString(2,9,".");
	OLED_ShowString(2,12,"V");
	AD_Init();
	while(1)
	{
		ADValue=AD_GetValue();
		Voltage=(float)ADValue*3.3*11/4096;//单位V
		OLED_ShowNum(2,7,Voltage,2);
		OLED_ShowNum(2,10,(uint16_t)(Voltage*100)%100,2);//取出小数两位
		//取余计算需要为整数
		Delay_ms(1000);
	}
}
