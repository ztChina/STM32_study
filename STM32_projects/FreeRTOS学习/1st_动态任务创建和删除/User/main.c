
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
#include "OLED.h"




/***************************��������**************************/

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );


int main()
{
	//OLED��ʼ��
	OLED_Init();
	OLED_ShowChar(4, 1, 'A');
	OLED_Clear();
	OLED_ShowChar(4, 1, 'B');
	
	
	//������̬����
	taskENTER_CRITICAL();  //�����ٽ�����ֹͣ�жϣ���ֹ������Դ������ͻ
	xTaskCreate(task1,"1",1024,NULL,2,NULL);
	xTaskCreate(task2,"2",1024,NULL,2,NULL);
	taskEXIT_CRITICAL();       //�˳��ٽ���
	OLED_ShowChar(4, 5, 'A');
	
	
	//����������
	vTaskStartScheduler();	//����������
}


/* ����һ��ʵ��LED0ÿ500ms��תһ�� */
void task1( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(1, 1, 'A');
        vTaskDelay(500);
    }
}

/* �������ʵ��LED1ÿ500ms��תһ�� */
void task2( void * pvParameters )
{
    while(1)
    {
        OLED_ShowNum(2, 1, 54321, 5);
        vTaskDelay(500);
    }
}
