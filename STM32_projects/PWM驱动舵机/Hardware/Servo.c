#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//������ʼ�����
void Servo_Init(void)
{
		PWM_Init();
}	

//���ƶ���Ƕ�(���ֶ��ֻ��ת180��)
//�������Ϊ20ms,ռ��0.5~2.5ms�ֱ��Ӧ0~180�㣬�������Թ�ϵ
void Servo_SetAngle(float Angle)
{
	if(Angle>=0 && Angle<=180)
	{
		PWM_SetCompare1(Angle*2000/180+500);
	}
}
