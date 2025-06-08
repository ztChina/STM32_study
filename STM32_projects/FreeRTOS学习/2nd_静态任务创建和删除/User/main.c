/* Standard includes. */
#include <stdio.h>
#include "stm32f10x.h"                  // Device header


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



/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
StackType_t     start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t    start_task_tcb;
void start_task( void * pvParameters );

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
StackType_t     task1_stack[TASK1_STACK_SIZE];
StaticTask_t    task1_tcb;
void task1( void * pvParameters );

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
StackType_t     task2_stack[TASK2_STACK_SIZE];
StaticTask_t    task2_tcb;
void task2( void * pvParameters );

/* TASK3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    task3_handler;
StackType_t     task3_stack[TASK3_STACK_SIZE];
StaticTask_t    task3_tcb;
void task3( void * pvParameters );
/******************************************************************************************************/
/* ������������ */
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];

/* �����ʱ���������� */
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/* ���������ڴ���� */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,			//�����Ǿ�̬������Ҫ�Լ������ڴ�
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    * ppxIdleTaskTCBBuffer = &idle_task_tcb;		//ָ��Ҫ�������������ƿ飨TCB��ָ��ı����ĵ�ַ
    * ppxIdleTaskStackBuffer = idle_task_stack;	//ָ��Ҫ�����������ջ�ĵ�ַ�ı����ĵ�ַ
    * pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;	//ָ��Ҫ�����������ջ��С�ı����ĵ�ַ
}


/* �����ʱ���ڴ���� */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,		//�����ʱ��Ҳ�Ǳ������һ��������ƿ�
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
    
/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
int main()
{   
		OLED_Init();
		OLED_ShowChar(1, 1, 'D');
    start_task_handler = xTaskCreateStatic( (TaskFunction_t )   start_task,
                                            (char *         )   "start_task", 
                                            (uint32_t       )   START_TASK_STACK_SIZE,		//�����ջ��С
                                            (void *         )   NULL,											//�������
                                            (UBaseType_t    )   START_TASK_PRIO,					//���ȼ�
                                            (StackType_t *  )   start_task_stack,					//�����ջ����
                                            (StaticTask_t * )   &start_task_tcb );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* �����ٽ��� */
    
    task1_handler = xTaskCreateStatic(  (TaskFunction_t )   task1,
                                        (char *         )   "task1", 
                                        (uint32_t       )   TASK1_STACK_SIZE,
                                        (void *         )   NULL,
                                        (UBaseType_t    )   TASK1_PRIO,
                                        (StackType_t *  )   task1_stack,
                                        (StaticTask_t * )   &task1_tcb );
                        
    task2_handler = xTaskCreateStatic(  (TaskFunction_t )   task2,
                                        (char *         )   "task2", 
                                        (uint32_t       )   TASK2_STACK_SIZE,
                                        (void *         )   NULL,
                                        (UBaseType_t    )   TASK2_PRIO,
                                        (StackType_t *  )   task2_stack,
                                        (StaticTask_t * )   &task2_tcb );
                        
    task3_handler = xTaskCreateStatic(  (TaskFunction_t )   task3,
                                        (char *         )   "task3", 
                                        (uint32_t       )   TASK3_STACK_SIZE,
                                        (void *         )   NULL,
                                        (UBaseType_t    )   TASK3_PRIO,
                                        (StackType_t *  )   task3_stack,
                                        (StaticTask_t * )   &task3_tcb );
    
    vTaskDelete(start_task_handler);
    taskEXIT_CRITICAL();                /* �˳��ٽ��� */
}

/* ����һ��ʵ��LED0ÿ500ms��תһ�� */
void task1( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(4, 1, 'A');
    }
}

/* �������ʵ��LED1ÿ500ms��תһ�� */
void task2( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(3, 1, 'B');
				vTaskDelay(500);
    }
}

/* ���������жϰ���KEY0������KEY0ɾ��task1 */
void task3( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(2, 1, 'C');
				vTaskDelay(500);
    }
}
