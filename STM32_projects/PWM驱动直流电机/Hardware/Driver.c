#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//��ʼ��ֱ�����ֵ��
//TB6612�������£�
// AIN1 : PB14(out)		AIN2 : PB15(out)	AO1	: ���+		AO2	 : ���-		PWMA : PA8(out)
// ��ϵ��	1				0					+					-				��ת�����ˣ�
//			0				1					-					+				��ת��ǰ����
//			0				0					OFF					OFF				ֹͣ
void Driver_Init(void)
{
	PWM_Init();
	//��ʼ�� PB14 �� PB15 ���������������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

/** 
  * @breif ֱ������ٶȿ��ƺ���
  * @param Speed:-100��100��ֵ
  */
void Driver_SetSpeed(int8_t Speed)	//����������������ת
{
	if(Speed>=-100 && Speed<=100)	//�����ٶȷ�Χ��ARRΪ100��Speed/ARR = ռ�ձ�
	{
		if(Speed<0)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
			GPIO_ResetBits(GPIOB,GPIO_Pin_15);
			PWM_SetCompare1(Speed);
		}
		else if(Speed>0)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_15);
				GPIO_ResetBits(GPIOB,GPIO_Pin_14);
				PWM_SetCompare1(-Speed);
			}
			else if(Speed==0)
				{
					GPIO_ResetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14);
				}
	}
}
