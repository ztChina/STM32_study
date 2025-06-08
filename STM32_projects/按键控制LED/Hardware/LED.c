#include "stm32f10x.h"                  // Device header


/**
  * @brief  对平衡车上STM32板的 PA4 引脚 LED 灯进行初始化,及灯的打开和关闭，
			LED 低电平亮起。
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
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
