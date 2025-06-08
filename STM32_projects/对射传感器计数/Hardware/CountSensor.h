#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

uint16_t CountSensor_Get(void);
void CountSensor_Init(void);
void EXTI4_IRQHandler(void);
//中断函数不需要声明

#endif
