//PA0 ：PWM输出脚，PA0复用可对应着TIM2_CH1
#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//开启TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//TIM2使用内部时钟
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//不分割时钟
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=100-1;	//配置计时频率为1KHz，ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=720-1;	//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//配置输出比较单元
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//带“//"是高级定时器才要的
	//TIM_OCInitStructure.TIM_OCIdleState=;
	//TIM_OCInitStructure.TIM_OCNIdleState=;
	//TIM_OCInitStructure.TIM_OCNPolarity=;
	//TIM_OCInitStructure.TIM_OutputNState=;
	TIM_OCStructInit(&TIM_OCInitStructure);//为避免由于初始值配置不完整而导致错误，先将结构体初始化完整再修改
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	//PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	//高低极性即为当为有效电平时输出的电平高低
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=50;//输出比较寄存器CCR的值,占空比=50/(ARR+1)
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	//配置TIM2_CH1即PA0复用后引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//不需要打开复用时钟AFIO，只有在使用事件控制寄存器、复用重映射
	//和调试寄存器以及外部中断寄存器的时候才打开，只需要使用复用推挽或复用开漏模式即可复用功能
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//PA0配置为复用推挽模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*
	//也可以使用重映射将TIM2_CH1映射在PA15，操作如下
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开复用外设时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关掉PA15默认的复用JTAG功能
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//部分映射模式1开启，参考手册
	*/
	
	//定时器使能
	TIM_Cmd(TIM2,ENABLE);
}

//改变CCR值函数
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}
