//计按键按下次数
#include "stm32f10x.h"                  // Device header

//用extern声明num是外部文件的参数
extern uint32_t num;

void Timer_Init(void)
{
	//开启APB1上TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置GPIOA_5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//TIM2配置为外部时钟模式2
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted,0x0F);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分割参数，TIM_CKD_DIV1=0，类似于分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//上计数模式
	TIM_TimeBaseInitStruct.TIM_Period=5-1;//自动重装器的值，计数0~9999共计10000次
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;//分频器值，0为一分频，1为二分频，以此类推
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//高级计时器才有的重复计数器
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//清除更新事件
	//TIM_TimeBaseInit()函数会自动添加一次中断申请
	//为消除这一次中断的影响，需要清除更新事件标志位
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//使能计时器
	TIM_Cmd(TIM2,ENABLE);
	
	//使能时基单位的中断请求
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//NVIC配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
}

//中断函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

//计数函数
uint16_t Get_Counter(void)
{
	return TIM_GetCounter(TIM2);
}
