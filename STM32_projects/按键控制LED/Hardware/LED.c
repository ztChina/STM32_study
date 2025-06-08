#include "stm32f10x.h"                  // Device header


/**
  * @brief  ��ƽ�⳵��STM32��� PA4 ���� LED �ƽ��г�ʼ��,���ƵĴ򿪺͹رգ�
			LED �͵�ƽ����
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void LED_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

void LED_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
