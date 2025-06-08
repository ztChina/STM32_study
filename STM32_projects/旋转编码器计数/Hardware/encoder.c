//右轮接线顺序为A2、5V、B7、B6、GND、A1分别表示电机线-、编码器5V、编码器A相、编码器B相、编码器GND、电机线+
//正转时B相滞后A相90°，反转时B相超前A相90°（设计标准）
#include "stm32f10x.h"                  // Device header

int32_t Encoder_Count=0;//有符号，计数值

void Encoder_Init(void)
{
	//同时复用PB7和PB6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef InitStructure;
	InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	InitStructure.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_6;
	InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&InitStructure);
	
	//配置EXIT电路之中断引脚选择，即GPIO到AFIO选择的电路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);

	//配置EXIT电路之EXTI边沿检测及控制,即AFIO到EXTI的电路；
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line6 | EXTI_Line7;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//中断响应，指向CPU
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿中断
	EXTI_Init(&EXTI_InitStruct);
	
	//配置NVIC，即EXTI到NVIC的电路
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//对抢占优先级和响应优先级进行分配
	NVIC_InitTypeDef NVIC_InitStruct;
	//PB7和PB6共用EXTI9_5_IRQn中断请求线
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//打开EXTI到NVIC的线
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//配置抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//配置响应优先级
	NVIC_Init(&NVIC_InitStruct);
}

//中断函数
void EXTI9_5_IRQHandler(void)
{
	//利用AB相输出波形判断
	//正转用A的下降沿和B的高电平
	//反转用B的下降沿和A的高电平
	if(EXTI_GetITStatus(EXTI_Line6)==1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)
			{
				Encoder_Count--;
			}
			
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if(EXTI_GetITStatus(EXTI_Line7)==1)
			{
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1)
					{
						Encoder_Count++;
					}
					
				EXTI_ClearITPendingBit(EXTI_Line7);
			}
}

//返回计数值
int32_t Encoder_Get(void)
{
	return Encoder_Count;
}
