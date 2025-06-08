/* Standard includes. */
#include <stdio.h>
#include "stm32f10x.h"                  // Device header


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



/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
StackType_t     start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t    start_task_tcb;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
StackType_t     task1_stack[TASK1_STACK_SIZE];
StaticTask_t    task1_tcb;
void task1( void * pvParameters );

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
StackType_t     task2_stack[TASK2_STACK_SIZE];
StaticTask_t    task2_tcb;
void task2( void * pvParameters );

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    task3_handler;
StackType_t     task3_stack[TASK3_STACK_SIZE];
StaticTask_t    task3_tcb;
void task3( void * pvParameters );
/******************************************************************************************************/
/* 空闲任务配置 */
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];

/* 软件定时器任务配置 */
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务内存分配 */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,			//由于是静态任务，需要自己分配内存
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    * ppxIdleTaskTCBBuffer = &idle_task_tcb;		//指向将要保存空闲任务控制块（TCB）指针的变量的地址
    * ppxIdleTaskStackBuffer = idle_task_stack;	//指向将要保存空闲任务栈的地址的变量的地址
    * pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;	//指向将要保存空闲任务栈大小的变量的地址
}


/* 软件定时器内存分配 */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,		//软件定时器也是被管理的一个任务控制块
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
    
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
int main()
{   
		OLED_Init();
		OLED_ShowChar(1, 1, 'D');
    start_task_handler = xTaskCreateStatic( (TaskFunction_t )   start_task,
                                            (char *         )   "start_task", 
                                            (uint32_t       )   START_TASK_STACK_SIZE,		//任务堆栈大小
                                            (void *         )   NULL,											//传入参数
                                            (UBaseType_t    )   START_TASK_PRIO,					//优先级
                                            (StackType_t *  )   start_task_stack,					//任务堆栈区域
                                            (StaticTask_t * )   &start_task_tcb );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    
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
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现LED0每500ms翻转一次 */
void task1( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(4, 1, 'A');
    }
}

/* 任务二，实现LED1每500ms翻转一次 */
void task2( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(3, 1, 'B');
				vTaskDelay(500);
    }
}

/* 任务三，判断按键KEY0，按下KEY0删除task1 */
void task3( void * pvParameters )
{
    while(1)
    {
        OLED_ShowChar(2, 1, 'C');
				vTaskDelay(500);
    }
}
