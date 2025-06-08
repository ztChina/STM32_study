#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size = 0; //计数器大小

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size){
	MyDMA_Size = Size;
	
	//打开 DMA1 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//初始化DMA1
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;//外设（存储器）地址，数据宽度，是否自增
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_MemoryBaseAddr = AddrB;//存储器地址，数据宽度，是否自增
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_BufferSize = Size;//缓冲区大小，即计数器
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//硬件软件触发
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;	//是否自动重装
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;//优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
/*	
	//DMA使能，必须发生在初始化之后，使能后不能修改计数器数值
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//一使能便开始转运，判断转运是否结束，结束置 1
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	//手动清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
*/
}

/**
 * @breif 初始化DMA后，重装传输计数器，进行一次DMA传输
 */
void MyDMA_Tranfer(void)
{
	//传输计数器赋值前，必须要使DMA失能
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//判断是否结束，结束置 1
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	//手动清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
