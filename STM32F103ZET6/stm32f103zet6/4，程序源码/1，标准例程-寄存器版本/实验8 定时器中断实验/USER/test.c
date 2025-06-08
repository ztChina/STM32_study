#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 
#include "timer.h" 
//ALIENTEK��ӢSTM32F103������ ʵ��8
//��ʱ���ж� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
int main(void)
{					   
	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,115200); 	//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms  
   	while(1)
	{
		LED0=!LED0;
		delay_ms(200);		   
	}	
}	 

