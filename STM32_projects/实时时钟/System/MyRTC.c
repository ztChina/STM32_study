#include "stm32f10x.h"                  // Device header
#include "time.h"		//时间处理库

uint16_t MyRTC_Time[] = {2023, 1, 1, 23, 59, 55};


//对 RTC 设置初始时间
void MyRTC_SetTime(void);

// RTC（实时时钟） 初始化函数
void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//使能电源控制模块
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);		//使能备份寄存器模块
	PWR_BackupAccessCmd(ENABLE);	//使能 BKP 与 RTC 的读写访问
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		RCC_LSEConfig(RCC_LSE_ON);	//连通低速外部时钟
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	//获取标志位，等待结束
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	//配置 RTC 连通 LSE
		RCC_RTCCLKCmd(ENABLE);	// RTC 启用时钟
		
		RTC_WaitForSynchro();	//等待同步，主要是同步系统时钟和RTC时钟，确保读写数据时时钟同步
		RTC_WaitForLastTask();	//等待上一次写结束
		
		RTC_SetPrescaler(32768 - 1);	//设置分频值，选择的时钟是 32.678 kHz的外部时钟，所以设置为这个值
		RTC_WaitForLastTask();	//等待上一次写结束
		
		MyRTC_SetTime();	//对 RTC 设置初始时间
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);		//设置虚拟标志，防止重复配置
	}
	else
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}




//如果LSE无法起振导致程序卡死在初始化函数中
//可将初始化函数替换为下述代码，使用LSI当作RTCCLK
//LSI无法由备用电源供电，故主电源掉电时，RTC走时会暂停
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




//对 RTC 设置初始时间
void MyRTC_SetTime(void)
{
	time_t time_cnt;	//秒计数类型
	struct tm time_date;	//结构体类型
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	//时间转换
	time_cnt = mktime(&time_date) - 8 * 60 * 60;	// -8*60*60是减去东八区偏移，转换为UTC
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();	//直接对寄存器写实现初始设置
}



//读取 RTC 时间
void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	
	time_date = *localtime(&time_cnt);	//localtime返回值是地址
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
