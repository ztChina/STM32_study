#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"  
#include "stmflash.h" 
#include "iap.h"   
//ALIENTEK精英STM32F103开发板 实验42
//IAP实验 Bootloader V1.0 代码 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

int main(void)
{		
	u8 t; 
	u8 key;
	u16 oldcount=0;				//老的串口接收数据值
	u16 applenth=0;				//接收到的app代码长度
	u8 clearflag=0;   
 	Stm32_Clock_Init(9);		//系统时钟设置
	uart_init(72,115200);		//串口初始化为115200
	delay_init(72);	   	 		//延时初始化 
  	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD  
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"ELITE STM32F103 ^_^");	
	LCD_ShowString(30,70,200,16,16,"IAP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/27");  
	LCD_ShowString(30,130,200,16,16,"KEY_UP:Copy APP2FLASH"); 
	LCD_ShowString(30,150,200,16,16,"KEY1:Run FLASH APP");
	LCD_ShowString(30,170,200,16,16,"KEY0:Run SRAM APP");
	POINT_COLOR=BLUE;
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色	  
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0;
			t=0;
			if(clearflag)
			{
				clearflag--;
				if(clearflag==0)LCD_Fill(30,210,240,210+16,WHITE);//清除显示
			}
		}	  	 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
				LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
					printf("固件更新完成!\r\n");	
				}else 
				{
					LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
				LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示									 
		} 
		if(key==KEY1_PRES)
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	  
		}
		if(key==KEY0_PRES)
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM地址
			}else 
			{
				printf("非SRAM应用程序,无法执行!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	 
		}				   
		 
	}   	   
}










