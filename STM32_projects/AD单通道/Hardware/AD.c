//ͨ��PA6����ؾ������Խ�ѹģ���ĵ�ѹ
//PA6ΪADC12_IN6
#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//����������PA6Ϊģ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//����ADC1ʱ�ӣ�����Ƶ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����Ϊ6,72M/6=12M,ADC���ʱ�䲻�ܳ���14M
	
	//ADC1����������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_71Cycles5);//��ͨ��0���ڹ���������1��
	
	//�ýṹ������ADC1
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//����Ϊ�������
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//����ģʽ(����˫ģʽ)
	ADC_InitStructure.ADC_NbrOfChannel=1;//������ͨ����Ϊ1
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//����ת���򵥴�ת��
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//��ɨ��ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//����ADC1
	ADC_Cmd(ADC1,ENABLE);
	
	//����ADC����ҪУ׼
	ADC_ResetCalibration(ADC1);//��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//����ֵΪSET�����ʾУ׼��δ����
	ADC_StartCalibration(ADC1);//��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1)==SET);//����ֵΪSET����ʾУ׼��δ����
	
}

//��ȡADֵ
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//����ֵΪRESET�����ʾУ׼��δ����
	//��EOC��ΪSET���ٶ�ȡһ�ν��Զ�����
	return ADC_GetConversionValue(ADC1);
}
