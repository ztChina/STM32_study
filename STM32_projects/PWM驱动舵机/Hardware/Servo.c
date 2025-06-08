#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//用来初始化舵机
void Servo_Init(void)
{
		PWM_Init();
}	

//控制舵机角度(这种舵机只能转180度)
//舵机周期为20ms,占用0.5~2.5ms分别对应0~180°，两者线性关系
void Servo_SetAngle(float Angle)
{
	if(Angle>=0 && Angle<=180)
	{
		PWM_SetCompare1(Angle*2000/180+500);
	}
}
