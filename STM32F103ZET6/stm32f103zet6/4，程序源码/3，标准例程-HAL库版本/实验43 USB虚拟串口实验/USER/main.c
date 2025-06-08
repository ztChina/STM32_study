#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "usbd_ctlreq.h"
#include "hw_config.h"
#include "usbd_core.h"
/************************************************
 ALIENTEK 精英STM32F103开发板 实验43
 USB虚拟串口实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/
extern USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_FS;

int main(void)
{
    u16 t;
    u16 len;
    u16 times=0;
    u8 usbstatus=0;

    HAL_Init();                    	 	//初始化HAL库
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M，9倍频 8Mhzx9 = 72Mhz
    delay_init(72);               		//初始化延时函数
    uart_init(115200);					//初始化串口
    LED_Init();							//初始化LED
    LCD_Init();           				//初始化LCD FSMC接口
    POINT_COLOR=RED;					//设置字体为红色
    LCD_ShowString(30,50,200,16,16,"ELITE STM32F103 ^_^");
    LCD_ShowString(30,70,200,16,16,"USB Virtual USART TEST");
    LCD_ShowString(30,90,200,16,16,"JASONZHANG@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2019/11/7");
    LCD_ShowString(30,130,200,16,16,"USB Connecting...");//提示USB开始连接
    USB_Reset();//USB断开再重连
    MX_USB_DEVICE_Init();//USB设备初始化
    LCD_ShowString(30,150,200,16,16,"USB inited...");
    while(1)
    {
        if(usbstatus!=USB_GetStatus())
        {
            usbstatus = USB_GetStatus();//记录新的状态
            if(usbstatus==USBD_STATE_CONFIGURED)
            {
                POINT_COLOR=BLUE;
                LCD_ShowString(30,170,200,16,16,"USB Connected    ");//提示USB连接成功
                LED1=0;//DS1亮
            } else
            {
                POINT_COLOR=RED;
                LCD_ShowString(30,170,200,16,16,"USB disConnected ");//提示USB断开
                LED1=1;//DS1灭
            }
        }
        if(USB_USART_RX_STA&0x8000)
        {
            len=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
            USB_Printf("\r\n您发送的消息长度为:%d\r\n",len);
            for(t=0; t<len; t++)
            {
                USB_USART_SendData(&USB_USART_RX_BUF[t]);//字符发送
            }
            USB_Printf("\r\n");//插入换行
            USB_USART_RX_STA=0;
        } else
        {
            times++;
            if(times%5000==0)
            {
                USB_Printf("\r\n精英STM32开发板USB虚拟串口实验\r\n");
                USB_Printf("正点原子@ALIENTEK\r\n\r\n");
            }
            if(times%200==0)  USB_Printf("请输入数据,以回车键结束\r\n");
            if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
            delay_ms(10);
        }
    }
}

