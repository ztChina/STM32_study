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
//ALIENTEK精英STM32F103开发板 实验36
//FATFS 实验 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 
int main(void)
{		
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	 
 	Stm32_Clock_Init(9);		//系统时钟设置
	uart_init(72,115200);		//串口初始化为115200
	delay_init(72);	   	 		//延时初始化  
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
	W25QXX_Init();				//初始化W25Q128
 	my_mem_init(SRAMIN);		//初始化内部内存池
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"ELITE STM32F103 ^_^");	
	LCD_ShowString(30,70,200,16,16,"FATFS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/20"); 
	LCD_ShowString(30,130,200,16,16,"Use USMART for test");   
	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
 	exfuns_init();							//为fatfs相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 					//挂载SD卡 
 	res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(30,150,200,16,16,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(30,150,200,16,16,"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}													    
	LCD_Fill(30,150,240,150+16,WHITE);		//清除显示			  
	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(30,150,240,150+16,WHITE);	//清除显示			  
		delay_ms(200);
		LED0=!LED0;//DS0闪烁
	}													  			    
 	POINT_COLOR=BLUE;//设置字体为蓝色	   
	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
 	LCD_ShowNum(30+8*14,170,total>>10,5,16);				//显示SD卡总容量 MB
 	LCD_ShowNum(30+8*14,190,free>>10,5,16);					//显示SD卡剩余容量 MB			    
	while(1)
	{
		t++; 
		delay_ms(200);		 			   
		LED0=!LED0;
	} 
}










