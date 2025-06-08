//对编码器进行计数并显示在OLED屏幕上
//编码器为平衡车上右轮霍尔传感器
//右轮接线顺序为AO2、5V、PB7、PB6、GND、AO1分别表示电机线-、编码器5V、编码器A相、编码器B相、编码器GND、电机线+
//左轮为BO2、5V、A0、A1、GND、BO1，接线顺序与上相同
//AO 和 BO 是 TB6612 上的引脚
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"

int main()
{
	OLED_Init();
	Encoder_Init();
	while(1)
	{
		OLED_ShowSignedNum(2,1,Encoder_Get(),6);		
	}
}
