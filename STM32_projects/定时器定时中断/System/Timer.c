//��ʱ��
#include "stm32f10x.h"                  // Device header

//��extern����num���ⲿ�ļ��Ĳ���
extern uint32_t num;

void Timer_Init(void)
{
	//����APB1��TIM2��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//TIM2����Ϊ�ڲ�ʱ��ģʽ
	TIM_InternalClockConfig(TIM2);
	
	//����ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷָ������TIM_CKD_DIV1=0�������ڷ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//�ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;//�Զ���װ����ֵ������0~9999����10000��
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200;//��Ƶ��ֵ��0Ϊһ��Ƶ��1Ϊ����Ƶ���Դ�����
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//�߼���ʱ�����е��ظ�������
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//��������¼�
	//TIM_TimeBaseInit()�������Զ����һ���ж�����
	//Ϊ������һ���жϵ�Ӱ�죬��Ҫ��������¼���־λ
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//ʹ�ܼ�ʱ��
	TIM_Cmd(TIM2,ENABLE);
	
	//ʹ��ʱ����λ���ж�����
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//NVIC����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
}

//�жϺ���
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
