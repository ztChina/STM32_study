//ʵ��led����200msΪ���ڽ���0.5��ռ�ձ���˸
//type-c �����ϴ�
//ƽ�⳵�ϵ�STM32F103����λ��A4���͵�λ����

#include "stm32f10x.h"                  // Device header
#include "Delay.h"						//�ٷ���ʱ��

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		Delay_ms(500);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		Delay_ms(500);
	}
}
