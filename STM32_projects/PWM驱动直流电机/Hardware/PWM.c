//PA8 ：作为PWM输出脚，PA8复用功能对应着TIM1_CH1

#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//开启TIM1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//TIM1使用内部时钟
	TIM_InternalClockConfig(TIM1);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//不分割时钟
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=100-1;	//配置频率为20KHz，ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=720/20-1;	//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//配置输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//为避免由于初始值配置不完整而导致错误，先将结构体初始化完整再修改
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	//PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	//高低极性即为当为有效电平时输出的电平高低
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	//配置TIM1_CH1即PA8复用后引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//PA0配置为复用推挽模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//使能MOE主输出，高级定时器需开启
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	//定时器使能
	TIM_Cmd(TIM1,ENABLE);
}

//改变CCR值函数
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1,Compare);
}
