#ifndef __TPAD_H
#define __TPAD_H
#include "sys.h"
#include "timer.h"		
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32F103开发板V1
//TPAD驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	   
//空载的时候(没有手按下),计数器需要的时间
//这个值应该在每次开机的时候被初始化一次
extern vu16 tpad_default_val;
							   	    
void TPAD_Reset(void);
u16  TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8   TPAD_Init(u8 systick);
u8   TPAD_Scan(u8 mode);
void TIM5_CH2_Cap_Init(u16 arr,u16 psc);    
#endif























