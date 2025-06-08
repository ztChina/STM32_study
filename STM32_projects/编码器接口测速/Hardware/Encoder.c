//���ֽ���˳��ΪAO2��5V��PB7��PB6��GND��AO1�ֱ��ʾ�����-��������5V��������A�ࡢ������B�ࡢ������GND�������+
//����PB7��PB6���ú���TIM4��CH2��CH1
#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	//��TIM4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//����PB7��PB6��������Ҫ��AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	//����ʱ����Ԫ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ò��������ϼ��������¼�����������ģʽ�ӹ�
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//�������벶��ͨ��
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0xF;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	//���ñ������ӿ�
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Rising);
	//�����ز�������ߵ͵�ƽ����ת,�½��ر�ʾ�ߵ͵�ƽ��ת��
	//���������Ƿ��Ű�װ�ģ�����Ҫ��תһ������
	//��TI12�£�һ��AB��������4����������
	
	//����������
	TIM_Cmd(TIM4,ENABLE);
}	

//��ȡCNT����ֵ
//ͨ��int����������ת��Ϊ�з��������з�������ͨ�����뽫�޷�����ת��������
int16_t Encoder_Get(void)
{
	return TIM_GetCounter(TIM4);
	//return TIM4->CNT;
}	

//��ȡCNT����ֵͬʱ����CNT
int16_t Encoder_Speed(void)
{
	int16_t Temp;
	Temp=TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4,0);//����CNT����ֹ���
	return Temp;
}
