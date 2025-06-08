#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
	
	//打开ADC1、GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//打开 DMA1 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//配置ADC的时钟，平衡转换速率和精度
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//ADC扫描引脚初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	//确定通道和序列的对应关系
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	//ADC配置初始化
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//连续模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1, &ADC_InitStructure);
	

	//初始化DMA1
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR; //DMA转运的第一个地址是ADC1的DR地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //转运数据大小是DR寄存器的低16位
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t) AD_Value;//存储器地址，数据宽度，是否自增
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_BufferSize = 4;//缓冲区大小，即计数器
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//硬件触发
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;	//是否自动重装;DMA_Mode_Circular表示重装，即循环模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;//优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);	//ADC1硬件请求是接在DMA1的通道1上
	
	//DMA使能，必须发生在初始化之后，使能后不能修改计数器数值
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//连接ADC1和DMA
	ADC_DMACmd(ADC1,ENABLE);
	//ADC使能
	ADC_Cmd(ADC1, ENABLE);
	
	//校准ADC
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}



//用在不连续重装的情况下
uint16_t AD_GetValue(void)
{
	//重置传输次数
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,4);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	//软件启动转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	//等待DMA完成
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	//手动清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
