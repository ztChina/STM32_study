#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "can.h"
#include "stdio.h"
/************************************************
 ALIENTEK 精英STM32F103开发板 实验25
 CAN实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
	
int main(void)
{	
   	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 canbuf_two[8]={2};
	u8 res;
	u8 mode=0; 
	
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	usmart_dev.init(84); 		   		//初始化USMART	
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
 	LCD_Init();           				//初始化LCD FSMC接口
    
 	CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,100,CAN_MODE_LOOPBACK); //CAN初始化,波特率20Kbps      
	CAN_Config();
    while(1)
    {
        key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下,发送一次数据
		{
			printf(" KEY_0 down\r\n" );
			printf("canbuf:");
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;//填充发送缓冲区
				printf(" %d",canbuf[i] );
				if(i<4)LCD_ShowxNum(30+i*32,210,canbuf[i],3,16,0X80);	//显示数据
				else LCD_ShowxNum(30+(i-4)*32,230,canbuf[i],3,16,0X80);	//显示数据
 			}
			printf("\r\n");
			res=CAN1_Send_Msg(canbuf_two,8);//发送8个字节 
			printf(" CAN sendtwo\r\n" );
			if(res)LCD_ShowString(30+80,190,200,16,16,"Failed");		//提示发送失败
			else LCD_ShowString(30+80,190,200,16,16,"OK    ");	 		//提示发送成功								   
		}else if(key==WKUP_PRES)//WK_UP按下，改变CAN的工作模式
		{	
			printf(" KEY_UP down\r\n" );			
			mode=!mode;
            if(mode==0)  CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,100,CAN_MODE_NORMAL);        //回环模式,波特率20Kbps
            else if(mode==1) CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,100,CAN_MODE_LOOPBACK);  //回环模式,波特率20Kbps
			CAN_Config();
			POINT_COLOR=RED;//设置字体为红色 
			if(mode==0)//普通模式，需要2个开发板
			{
				printf(" mode 0\r\n" );
				LCD_ShowString(30,130,200,16,16,"Nnormal Mode ");	    
			}else //回环模式,一个开发板就可以测试了.
			{
				printf(" mode 1\r\n" );
 				LCD_ShowString(30,130,200,16,16,"LoopBack Mode");
			}
 			POINT_COLOR=BLUE;//设置字体为蓝色 
		}		 
		key=CAN1_Receive_Msg(canbuf);
		if(key < 9)//接收到有数据
		{			
			LCD_Fill(30,270,160,310,WHITE);//清除之前的显示
 			for(i=0;i<key;i++)
			{									    
				if(i<4)LCD_ShowxNum(30+i*32,270,canbuf[i],3,16,0X80);	//显示数据
				else LCD_ShowxNum(30+(i-4)*32,290,canbuf[i],3,16,0X80);	//显示数据
 			}
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//提示系统正在运行	
			t=0;
			cnt++;
			LCD_ShowxNum(30+48,170,cnt,3,16,0X80);	//显示数据
		}		   
	} 
}

