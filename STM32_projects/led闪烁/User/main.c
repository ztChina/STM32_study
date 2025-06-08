//实现led灯以200ms为周期进行0.5的占空比闪烁
//type-c 串口上传
//平衡车上的STM32F103，灯位于A4，低电位驱动

#include "stm32f10x.h"                  // Device header
#include "Delay.h"						//官方延时库

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		Delay_ms(500);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		Delay_ms(500);
	}
}
