//右轮接线顺序为AO2、5V、PB7、PB6、GND、AO1分别表示电机线-、编码器5V、编码器A相、编码器B相、编码器GND、电机线+
//右轮PB7、PB6复用后是TIM4的CH2、CH1
#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	//打开TIM4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//复用PB7、PB6，但不需要打开AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//无用参数，向上计数或向下计数被编码器模式接管
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//配置输入捕获通道
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0xF;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	//配置编码器接口
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Rising);
	//上升沿参数代表高低电平不翻转,下降沿表示高低电平翻转。
	//由于右轮是反着安装的，所以要反转一个参数
	//在TI12下，一个AB相周期有4个触发计数
	
	//启动编码器
	TIM_Cmd(TIM4,ENABLE);
}	

//获取CNT计数值
//通过int将数据类型转换为有符号数，有符号数是通过补码将无符号数转换过来的
int16_t Encoder_Get(void)
{
	return TIM_GetCounter(TIM4);
	//return TIM4->CNT;
}	

//获取CNT计数值同时清零CNT
int16_t Encoder_Speed(void)
{
	int16_t Temp;
	Temp=TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4,0);//清零CNT，防止溢出
	return Temp;
}
