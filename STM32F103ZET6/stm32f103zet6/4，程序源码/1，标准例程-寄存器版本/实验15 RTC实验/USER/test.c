#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"
#include "usmart.h"
#include "rtc.h" 
//ALIENTEK精英STM32F103开发板 实验15
//RTC实时时钟 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  

int main(void)
{		
	u8 t;		   			    
  	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,115200);	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化 
	usmart_dev.init(72);	//初始化USMART				 	
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();				//初始化LCD
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"ELITE STM32F103 ^_^");	
	LCD_ShowString(30,70,200,16,16,"RTC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/14");	
	while(RTC_Init())		//RTC初始化	，一定要初始化成功
	{ 
		LCD_ShowString(30,130,200,16,16,"RTC ERROR!   ");	
		delay_ms(800);
		LCD_ShowString(30,130,200,16,16,"RTC Trying...");	
	}		    						
	//显示时间
	POINT_COLOR=BLUE;//设置字体为蓝色					 
	LCD_ShowString(30,130,200,16,16,"    -  -     ");	   
	LCD_ShowString(30,166,200,16,16,"  :  :  ");	 		    
	while(1)
	{								    
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			LCD_ShowNum(30,130,calendar.w_year,4,16);									  
			LCD_ShowNum(70,130,calendar.w_month,2,16);									  
			LCD_ShowNum(94,130,calendar.w_date,2,16);	 
			switch(calendar.week)
			{
				case 0:
					LCD_ShowString(30,148,200,16,16,"Sunday   ");
					break;
				case 1:
					LCD_ShowString(30,148,200,16,16,"Monday   ");
					break;
				case 2:
					LCD_ShowString(30,148,200,16,16,"Tuesday  ");
					break;
				case 3:
					LCD_ShowString(30,148,200,16,16,"Wednesday");
					break;
				case 4:
					LCD_ShowString(30,148,200,16,16,"Thursday ");
					break;
				case 5:
					LCD_ShowString(30,148,200,16,16,"Friday   ");
					break;
				case 6:
					LCD_ShowString(30,148,200,16,16,"Saturday ");
					break;  
			}
			LCD_ShowNum(30,166,calendar.hour,2,16);									  
			LCD_ShowNum(54,166,calendar.min,2,16);									  
			LCD_ShowNum(78,166,calendar.sec,2,16);
			LED0=!LED0;
		}	
		delay_ms(10);								  
	};  
}


