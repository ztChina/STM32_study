#include "stm32f10x.h"                  // Device header

void laser_init()
{
	//��ɫLED��ʼ����PA4������Ϊ��������ģʽ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef InitStructure;
	InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	InitStructure.GPIO_Pin=GPIO_Pin_4;
	InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
	//����EXIT��·֮�ж�����ѡ�񣬼�GPIO��AFIOѡ��ĵ�·
	//�ж�Դѡ��LED�Ƶ� PA4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	//����EXIT��·֮EXTI���ؼ�⼰����,��AFIO��EXTI�ĵ�·��
	//�ж�Դѡ���4���ߣ���ӦPA4
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//�ж���Ӧ��ָ��CPU
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//�½����ж�
	EXTI_Init(&EXTI_InitStruct);
	
	//����NVIC����EXTI��NVIC�ĵ�·
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ռ���ȼ�����Ӧ���ȼ����з���
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;//��EXTI��NVIC��4���ߣ���Ӧ��GPIOA_Pin_4
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
}
