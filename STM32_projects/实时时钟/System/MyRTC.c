#include "stm32f10x.h"                  // Device header
#include "time.h"		//ʱ�䴦���

uint16_t MyRTC_Time[] = {2023, 1, 1, 23, 59, 55};


//�� RTC ���ó�ʼʱ��
void MyRTC_SetTime(void);

// RTC��ʵʱʱ�ӣ� ��ʼ������
void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//ʹ�ܵ�Դ����ģ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);		//ʹ�ܱ��ݼĴ���ģ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�� BKP �� RTC �Ķ�д����
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		RCC_LSEConfig(RCC_LSE_ON);	//��ͨ�����ⲿʱ��
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	//��ȡ��־λ���ȴ�����
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	//���� RTC ��ͨ LSE
		RCC_RTCCLKCmd(ENABLE);	// RTC ����ʱ��
		
		RTC_WaitForSynchro();	//�ȴ�ͬ������Ҫ��ͬ��ϵͳʱ�Ӻ�RTCʱ�ӣ�ȷ����д����ʱʱ��ͬ��
		RTC_WaitForLastTask();	//�ȴ���һ��д����
		
		RTC_SetPrescaler(32768 - 1);	//���÷�Ƶֵ��ѡ���ʱ���� 32.678 kHz���ⲿʱ�ӣ���������Ϊ���ֵ
		RTC_WaitForLastTask();	//�ȴ���һ��д����
		
		MyRTC_SetTime();	//�� RTC ���ó�ʼʱ��
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);		//���������־����ֹ�ظ�����
	}
	else
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}




//���LSE�޷������³������ڳ�ʼ��������
//�ɽ���ʼ�������滻Ϊ�������룬ʹ��LSI����RTCCLK
//LSI�޷��ɱ��õ�Դ���磬������Դ����ʱ��RTC��ʱ����ͣ
/* 
void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		RCC_LSICmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		
		RTC_SetPrescaler(40000 - 1);
		RTC_WaitForLastTask();
		
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RCC_LSICmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}*/




//�� RTC ���ó�ʼʱ��
void MyRTC_SetTime(void)
{
	time_t time_cnt;	//���������
	struct tm time_date;	//�ṹ������
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	//ʱ��ת��
	time_cnt = mktime(&time_date) - 8 * 60 * 60;	// -8*60*60�Ǽ�ȥ������ƫ�ƣ�ת��ΪUTC
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();	//ֱ�ӶԼĴ���дʵ�ֳ�ʼ����
}



//��ȡ RTC ʱ��
void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	
	time_date = *localtime(&time_cnt);	//localtime����ֵ�ǵ�ַ
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
