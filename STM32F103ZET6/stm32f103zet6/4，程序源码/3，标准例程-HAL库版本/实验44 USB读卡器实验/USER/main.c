#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "malloc.h"
#include "sdio_sdcard.h"
#include "w25qxx.h"
#include "hw_config.h"
#include "usb_device.h"
#include "usbd_storage_if.h"
/************************************************
 ALIENTEK ��ӢSTM32F103������ ʵ��44
 USB������ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

extern vu8 USB_STATUS_REG;			//USB״̬
extern vu8 bDeviceState;			//USB���� ���
extern USBD_HandleTypeDef hUsbDeviceFS;
extern SD_HandleTypeDef hsd;     	//SD�����


int main(void)
{
    u8 offline_cnt=0;
    u8 tct=0;
    u8 USB_STA;
    u8 Device_STA;
    HAL_Init();                    	 	//��ʼ��HAL��
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
    delay_init(72);               		//��ʼ����ʱ����
    uart_init(115200);					//��ʼ������
    LED_Init();							//��ʼ��LED
    LCD_Init();           				//��ʼ��LCD FSMC�ӿ�
    W25QXX_Init();						//��ʼ��W25Q128
    my_mem_init(SRAMIN);				//��ʼ���ڲ��ڴ��

    POINT_COLOR=RED;					//��������Ϊ��ɫ
    LCD_ShowString(30,50,200,16,16,"Elite STM32");
    LCD_ShowString(30,70,200,16,16,"USB Card Reader TEST");
    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2019/10/10");
    if(SD_Init())//SD����ʼ��
		LCD_ShowString(30,130,200,16,16,"SD Card Error!");	//���SD������
    else //SD ������
    {
        LCD_ShowString(30,130,200,16,16,"SD Card Size:     MB");
        SD_GetCardInfo(&SDCardInfo);    //��ȡSD����Ϣ
        Mass_Memory_Size[1]=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//����SD������
	    Mass_Block_Size[1] =MSC_MEDIA_PACKET;//��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�. �궨������ΪMSC_MEDIA_PACKET
	    Mass_Block_Count[1]=Mass_Memory_Size[1]/Mass_Block_Size[1];
        LCD_ShowNum(134,130,Mass_Memory_Size[1]>>20,5,16);	//��ʾSD������
    }
	
	if(W25QXX_TYPE!=W25Q128 && W25QXX_TYPE!=NM25Q128)
		LCD_ShowString(30,130,200,16,16,"W25Q128 Error!");	//���SD������
    else //SPI FLASH ����
    {
		Mass_Memory_Size[0]=1024*1024*12;	//ǰ12M�ֽ�
	    Mass_Block_Size[0] =512;			//����SPI FLASH�Ĳ���������СΪ512
	    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
        LCD_ShowString(30,150,200,16,16,"SPI FLASH Size:12MB");
    }
    LCD_ShowString(30,170,200,16,16,"USB Connecting...");//��ʾUSB��ʼ����
    USB_Reset();
    MX_USB_DEVICE_Init();//usb��ʼ��
    LCD_ShowString(30,190,200,16,16,"USB inited..."); 
	delay_ms(1800);	
    while(1)
    {
        delay_ms(1);
        if(USB_STA!=USB_STATUS_REG)//״̬�ı���
        {
            LCD_Fill(30,190,240,190+16,WHITE);//�����ʾ
            if(USB_STATUS_REG&0x01)//����д
            {
                LCD_ShowString(30,190,200,16,16,"USB Writing...");//��ʾUSB����д������
            }
            if(USB_STATUS_REG&0x02)//���ڶ�
            {
                LCD_ShowString(30,190,200,16,16,"USB Reading...");//��ʾUSB���ڶ�������
            }
            if(USB_STATUS_REG&0x04)
				LCD_ShowString(30,210,200,16,16,"USB Write Err ");//��ʾд�����
            else 
				LCD_Fill(30,210,240,210+16,WHITE);//�����ʾ
            if(USB_STATUS_REG&0x08)
				LCD_ShowString(30,230,200,16,16,"USB Read  Err ");//��ʾ��������
            else LCD_Fill(30,230,240,230+16,WHITE);//�����ʾ
            USB_STA=USB_STATUS_REG;//��¼����״̬
        }
        if(Device_STA!=USB_GetStatus())
        {
            Device_STA = USB_GetStatus();//��¼�µ�״̬
            if(Device_STA==USBD_STATE_CONFIGURED)
            {
                POINT_COLOR=BLUE;
                LCD_ShowString(30,170,200,16,16,"USB Connected    ");//��ʾUSB���ӳɹ�
            } else
            {
                POINT_COLOR=RED;
                LCD_ShowString(30,170,200,16,16,"USB disConnected ");//��ʾUSB�Ͽ�
            }
        }
        tct++;
        if(tct==200)
        {
            tct=0;
            LED0=!LED0;//��ʾϵͳ������
            if(USB_STATUS_REG&0x10)
            {
				offline_cnt=0;//USB������,�����offline������
				bDeviceState=USBD_STATE_CONFIGURED;
            } else//û�еõ���ѯ
            {
                offline_cnt++;
                if(offline_cnt>10)
				{
					bDeviceState=USBD_STATE_SUSPENDED;//2s��û�յ����߱��,����USB���γ���
				}
            }
            USB_STATUS_REG=0;
        }
    }
}

