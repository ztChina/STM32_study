//PA8 ����ΪPWM����ţ�PA8���ù��ܶ�Ӧ��TIM1_CH1

#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//����TIM1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//TIM1ʹ���ڲ�ʱ��
	TIM_InternalClockConfig(TIM1);
	
	//����ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//���ָ�ʱ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=100-1;	//����Ƶ��Ϊ20KHz��ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=720/20-1;	//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//��������Ƚϵ�Ԫ
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//Ϊ�������ڳ�ʼֵ���ò����������´����Ƚ��ṹ���ʼ���������޸�
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	//PWM1ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	//�ߵͼ��Լ�Ϊ��Ϊ��Ч��ƽʱ����ĵ�ƽ�ߵ�
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	//����TIM1_CH1��PA8���ú�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//PA0����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ʹ��MOE��������߼���ʱ���迪��
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	//��ʱ��ʹ��
	TIM_Cmd(TIM1,ENABLE);
}

//�ı�CCRֵ����
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1,Compare);
}
