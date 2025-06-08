#include "stm32f10x.h"                  // Device header
#include "OLED.h"


/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"	
#include "list.h"			//�б����
#include "task.h"			//�������
#include "queue.h"		//���в������ź�������
#include "FreeRTOSConfig.h"	//���ݲ�ͬ����������ļ���ÿһ����Ŀ����һ��

/* Library includes. */
#include "stm32f10x_it.h"

/* Demo app includes. */






int main()
{
	OLED_Init();
	
	OLED_ShowChar(1, 1, 'B');
	OLED_ShowString(1, 3, "HelloWorld!");
	OLED_ShowNum(2, 1, 12345, 5);
	OLED_ShowSignedNum(2, 7, -66, 2);
	OLED_ShowHexNum(3, 1, 0xAA55, 4);
	OLED_ShowBinNum(4, 1, 0xAA55, 16);
	while(1)
	{
		
	}
}
