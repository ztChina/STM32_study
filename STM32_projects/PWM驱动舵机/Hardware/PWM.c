//PA0 ��PWM����ţ�PA0���ÿɶ�Ӧ��TIM2_CH1
//PA5 ����������ţ����¸ߵ�ƽ
#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//TIM2ʹ���ڲ�ʱ��
	TIM_InternalClockConfig(TIM2);
	
	//����ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//���ָ�ʱ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=20000-1;	//���ü�ʱƵ��Ϊ50Hz��ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;	//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//��������Ƚϵ�Ԫ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	//PWM1ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	//�ߵͼ��Լ�Ϊ��Ϊ��Ч��ƽʱ����ĵ�ƽ�ߵ�
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//��ʼ��CCR��ֵ
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	//����TIM2_CH1��PA0���ú�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʱ��ʹ��
	TIM_Cmd(TIM2,ENABLE);
}

//�ı�CCRֵ����
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}
