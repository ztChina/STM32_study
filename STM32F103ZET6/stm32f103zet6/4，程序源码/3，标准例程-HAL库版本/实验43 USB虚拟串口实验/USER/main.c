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
 ALIENTEK ��ӢSTM32F103������ ʵ��43
 USB���⴮��ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
extern USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_FS;

int main(void)
{
    u16 t;
    u16 len;
    u16 times=0;
    u8 usbstatus=0;

    HAL_Init();                    	 	//��ʼ��HAL��
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M��9��Ƶ 8Mhzx9 = 72Mhz
    delay_init(72);               		//��ʼ����ʱ����
    uart_init(115200);					//��ʼ������
    LED_Init();							//��ʼ��LED
    LCD_Init();           				//��ʼ��LCD FSMC�ӿ�
    POINT_COLOR=RED;					//��������Ϊ��ɫ
    LCD_ShowString(30,50,200,16,16,"ELITE STM32F103 ^_^");
    LCD_ShowString(30,70,200,16,16,"USB Virtual USART TEST");
    LCD_ShowString(30,90,200,16,16,"JASONZHANG@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2019/11/7");
    LCD_ShowString(30,130,200,16,16,"USB Connecting...");//��ʾUSB��ʼ����
    USB_Reset();//USB�Ͽ�������
    MX_USB_DEVICE_Init();//USB�豸��ʼ��
    LCD_ShowString(30,150,200,16,16,"USB inited...");
    while(1)
    {
        if(usbstatus!=USB_GetStatus())
        {
            usbstatus = USB_GetStatus();//��¼�µ�״̬
            if(usbstatus==USBD_STATE_CONFIGURED)
            {
                POINT_COLOR=BLUE;
                LCD_ShowString(30,170,200,16,16,"USB Connected    ");//��ʾUSB���ӳɹ�
                LED1=0;//DS1��
            } else
            {
                POINT_COLOR=RED;
                LCD_ShowString(30,170,200,16,16,"USB disConnected ");//��ʾUSB�Ͽ�
                LED1=1;//DS1��
            }
        }
        if(USB_USART_RX_STA&0x8000)
        {
            len=USB_USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
            USB_Printf("\r\n�����͵���Ϣ����Ϊ:%d\r\n",len);
            for(t=0; t<len; t++)
            {
                USB_USART_SendData(&USB_USART_RX_BUF[t]);//�ַ�����
            }
            USB_Printf("\r\n");//���뻻��
            USB_USART_RX_STA=0;
        } else
        {
            times++;
            if(times%5000==0)
            {
                USB_Printf("\r\n��ӢSTM32������USB���⴮��ʵ��\r\n");
                USB_Printf("����ԭ��@ALIENTEK\r\n\r\n");
            }
            if(times%200==0)  USB_Printf("����������,�Իس�������\r\n");
            if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
            delay_ms(10);
        }
    }
}

