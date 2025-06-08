#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * @breif ʵ��ƽ�⳵STM32���ϰ�����ʼ����
  *  	   �������� PA5������PA5�͵�ƽ���ɿ�PA5�ߵ�ƽ��
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
  * @breif ��������ʶ��
  * @param None
  * @retval uint8_t
 */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
	{
		Delay_ms(20);//ͨ����ʱ�������ж��Ƿ����水�£����ǰ�����������
					 //��������ʱ��һ����5~10ms��ֻ��Ҫ�������ʱ�伴�ɡ�
		//֮�������ַ�ʽ����һ���ǰ��²��жϲ��Ƕ������� KeyNum=1,
		//��һ���ǰ���̧��󷵻�KeyNum=1��
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);//���������ǵڶ��ֵȴ�����̧��
														//��һ�ΰ����Ӱ��µ�̧��һ����Ҫ60~220ms
		KeyNum=1;
	}
	return KeyNum;
}
