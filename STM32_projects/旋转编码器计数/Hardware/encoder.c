//���ֽ���˳��ΪA2��5V��B7��B6��GND��A1�ֱ��ʾ�����-��������5V��������A�ࡢ������B�ࡢ������GND�������+
//��תʱB���ͺ�A��90�㣬��תʱB�೬ǰA��90�㣨��Ʊ�׼��
#include "stm32f10x.h"                  // Device header

int32_t Encoder_Count=0;//�з��ţ�����ֵ

void Encoder_Init(void)
{
	//ͬʱ����PB7��PB6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef InitStructure;
	InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	InitStructure.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_6;
	InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&InitStructure);
	
	//����EXIT��·֮�ж�����ѡ�񣬼�GPIO��AFIOѡ��ĵ�·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);

	//����EXIT��·֮EXTI���ؼ�⼰����,��AFIO��EXTI�ĵ�·��
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line6 | EXTI_Line7;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//�ж���Ӧ��ָ��CPU
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//�½����ж�
	EXTI_Init(&EXTI_InitStruct);
	
	//����NVIC����EXTI��NVIC�ĵ�·
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ռ���ȼ�����Ӧ���ȼ����з���
	NVIC_InitTypeDef NVIC_InitStruct;
	//PB7��PB6����EXTI9_5_IRQn�ж�������
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//��EXTI��NVIC����
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
}

//�жϺ���
void EXTI9_5_IRQHandler(void)
{
	//����AB����������ж�
	//��ת��A���½��غ�B�ĸߵ�ƽ
	//��ת��B���½��غ�A�ĸߵ�ƽ
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

//���ؼ���ֵ
int32_t Encoder_Get(void)
{
	return Encoder_Count;
}
