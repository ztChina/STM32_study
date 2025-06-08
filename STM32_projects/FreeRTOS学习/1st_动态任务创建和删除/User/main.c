
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"	
#include "list.h"			//列表操作
#include "task.h"			//任务操作
#include "queue.h"		//队列操作，信号量操作
#include "FreeRTOSConfig.h"	//根据不同需求的配置文件，每一个项目都有一个

/* Library includes. */
#include "stm32f10x_it.h"

/* Demo app includes. */
#include "OLED.h"




/***************************任务配置**************************/

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );


int main()
{
	//OLED初始化
	OLED_Init();
	OLED_ShowChar(4, 1, 'A');
	OLED_Clear();
	OLED_ShowChar(4, 1, 'B');
	
	
	//创建动态任务
	taskENTER_CRITICAL();  //进入临界区，停止中断，防止共用资源发生冲突
	xTaskCreate(task1,"1",1024,NULL,2,NULL);
	xTaskCreate(task2,"2",1024,NULL,2,NULL);
	taskEXIT_CRITICAL();       //退出临界区
	OLED_ShowChar(4, 5, 'A');
	
	
	//启动调度器
	vTaskStartScheduler();	//启动调度器
}


/* 任务一，实现LED0每500ms翻转一次 */
void task1( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(1, 1, 'A');
        vTaskDelay(500);
    }
}

/* 任务二，实现LED1每500ms翻转一次 */
void task2( void * pvParameters )
{
    while(1)
    {
        OLED_ShowNum(2, 1, 54321, 5);
        vTaskDelay(500);
    }
}
