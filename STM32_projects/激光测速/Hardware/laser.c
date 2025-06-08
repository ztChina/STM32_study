#include "stm32f10x.h"                  // Device header

void laser_init()
{
	//红色LED初始化，PA4，设置为复用推挽模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef InitStructure;
	InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	InitStructure.GPIO_Pin=GPIO_Pin_4;
	InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
	//配置EXIT电路之中断引脚选择，即GPIO到AFIO选择的电路
	//中断源选择LED灯的 PA4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	//配置EXIT电路之EXTI边沿检测及控制,即AFIO到EXTI的电路；
	//中断源选择第4号线，对应PA4
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//中断响应，指向CPU
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿中断
	EXTI_Init(&EXTI_InitStruct);
	
	//配置NVIC，即EXTI到NVIC的电路
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//对抢占优先级和响应优先级进行分配
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;//打开EXTI到NVIC的4号线，对应着GPIOA_Pin_4
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//配置抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//配置响应优先级
	NVIC_Init(&NVIC_InitStruct);
}
