#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h"  
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"	 
//ALIENTEK精英STM32F103开发板 实验37
//汉字显示 实验 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 
int main(void)
{		
	u32 fontcnt;		  
	u8 i,j;
	u8 fontx[2];//gbk码
	u8 key,t;
 	Stm32_Clock_Init(9);		//系统时钟设置
	uart_init(72,115200);		//串口初始化为115200
	delay_init(72);	   	 		//延时初始化  
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
	W25QXX_Init();				//初始化W25Q128
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH.
	while(font_init()) 			//检查字库
	{
UPD:    
		LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(30,50,200,16,16,"ELITE STM32F103 ^_^");	
		while(SD_Init())			//检测SD卡
		{
			LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,"SD Card OK");
		LCD_ShowString(30,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//更新字库
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
		delay_ms(1500);	
		LCD_Clear(WHITE);//清屏	       
	}  
	POINT_COLOR=RED;       
	Show_Str(30,50,200,16,"精英STM32F103开发板",16,0);				    	 
	Show_Str(30,70,200,16,"GBK字库测试程序",16,0);				    	 
	Show_Str(30,90,200,16,"正点原子@ALIENTEK",16,0);				    	 
	Show_Str(30,110,200,16,"2015年1月20日",16,0);
	Show_Str(30,130,200,16,"按KEY0,更新字库",16,0);
 	POINT_COLOR=BLUE;  
	Show_Str(30,150,200,16,"内码高字节:",16,0);				    	 
	Show_Str(30,170,200,16,"内码低字节:",16,0);				    	 
	Show_Str(30,190,200,16,"汉字计数器:",16,0);

	Show_Str(30,220,200,24,"对应汉字为:",24,0); 
	Show_Str(30,244,200,16,"对应汉字(16*16)为:",16,0);			 
	Show_Str(30,260,200,12,"对应汉字(12*12)为:",12,0);			 
	while(1)
	{
		fontcnt=0;
		for(i=0x81;i<0xff;i++)
		{		
			fontx[0]=i;
			LCD_ShowNum(118,150,i,3,16);		//显示内码高字节    
			for(j=0x40;j<0xfe;j++)
			{
				if(j==0x7f)continue;
				fontcnt++;
				LCD_ShowNum(118,170,j,3,16);	//显示内码低字节	 
				LCD_ShowNum(118,190,fontcnt,5,16);//汉字计数显示	 
			 	fontx[1]=j;
				Show_Font(30+132,220,fontx,24,0);	  
				Show_Font(30+144,244,fontx,16,0);	  		 		 
				Show_Font(30+108,260,fontx,12,0);	  		 		 
				t=200;
				while(t--)//延时,同时扫描按键
				{
					delay_ms(1);
					key=KEY_Scan(0);
					if(key==KEY0_PRES)goto UPD;
				}
				LED0=!LED0;
			}   
		}	
	} 
}










