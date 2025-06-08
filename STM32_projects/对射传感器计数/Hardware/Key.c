#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * @breif 实现平衡车STM32板上按键初始化，
  *  	   按键接在 PA5，按下PA5低电平，松开PA5高电平。
  * @param None
  * @retval None
 */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
}

/**
  * @breif 按键操作识别
  * @param None
  * @retval uint8_t
 */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
	{
		Delay_ms(20);//通过延时方法来判断是否是真按下，还是按键抖动现象，
					 //按键抖动时间一般在5~10ms，只需要大于这个时间即可。
		//之后有两种方式，第一种是按下并判断不是抖动返回 KeyNum=1,
		//另一种是按键抬起后返回KeyNum=1。
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);//加上这句就是第二种等待按键抬起，
														//按一次按键从按下到抬起一般需要60~220ms
		KeyNum=1;
	}
	return KeyNum;
}
