//��Ʋ�����LED������ PA4 ���͵�ƽʱ��

#include "stm32f10x.h"                  // Device header
int main(void)
{
	// RCC��reset and clock control��ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//ʹ��GPIOAʱ��
	
	// GPIO_InitTypeDef ��һ�ֽṹ�����ͣ�GPIO_InitStructure �ṹ�������
	GPIO_InitTypeDef GPIO_InitStructure;
	// GPIO_Mode��GPIO_Pin��GPIO_Speed �Ǹýṹ���һ������
	//��ͨ������ and Ctrl+F �����˽⹦��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// GPIO_Mode_Out_PP ��ͨ�����������
	//�������������ص��ǿ�������������������ߵ�ƽ�͵͵�ƽ�������ֵ�ƽ�¶���������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	while(1)
	{
	
	}
}
