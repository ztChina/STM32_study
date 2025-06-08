//通过PA6检测电池经过线性降压模块后的电压
//PA6为ADC12_IN6
#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//开启并配置PA6为模拟输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//开启ADC1时钟，并分频
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC分频因子为6,72M/6=12M,ADC最大时间不能超过14M
	
	//ADC1规则组配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_71Cycles5);//将通道0放在规则组序列1上
	
	//用结构体配置ADC1
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//配置为软件触发
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//独立模式(或者双模式)
	ADC_InitStructure.ADC_NbrOfChannel=1;//规则组通道数为1
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//连续转换或单次转换
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//非扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//开启ADC1
	ADC_Cmd(ADC1,ENABLE);
	
	//开启ADC后需要校准
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//返回值为SET，则表示校准还未结束
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1)==SET);//返回值为SET，表示校准还未结束
	
}

//获取AD值
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//返回值为RESET，则表示校准还未结束
	//当EOC变为SET后，再读取一次将自动清零
	return ADC_GetConversionValue(ADC1);
}
