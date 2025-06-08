//点灯操作，LED连接在 PA4 ，低电平时亮

#include "stm32f10x.h"                  // Device header
int main(void)
{
	// RCC：reset and clock control，时钟配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//使能GPIOA时钟
	
	// GPIO_InitTypeDef 是一种结构体类型，GPIO_InitStructure 结构体对象名
	GPIO_InitTypeDef GPIO_InitStructure;
	// GPIO_Mode、GPIO_Pin、GPIO_Speed 是该结构体的一个参数
	//可通过定义 and Ctrl+F 查找了解功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// GPIO_Mode_Out_PP 是通用推挽输出。
	//推挽输出的最大特点是可以真正能真正的输出高电平和低电平，在两种电平下都具有驱动能力。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	while(1)
	{
	
	}
}
