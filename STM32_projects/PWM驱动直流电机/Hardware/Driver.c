#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//初始化直流右轮电机
//TB6612接线如下：
// AIN1 : PB14(out)		AIN2 : PB15(out)	AO1	: 电机+		AO2	 : 电机-		PWMA : PA8(out)
// 关系：	1				0					+					-				正转（后退）
//			0				1					-					+				反转（前进）
//			0				0					OFF					OFF				停止
void Driver_Init(void)
{
	PWM_Init();
	//初始化 PB14 和 PB15 两个方向控制引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

/** 
  * @breif 直流电机速度控制函数
  * @param Speed:-100到100的值
  */
void Driver_SetSpeed(int8_t Speed)	//带符号数，有正反转
{
	if(Speed>=-100 && Speed<=100)	//限制速度范围，ARR为100，Speed/ARR = 占空比
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
