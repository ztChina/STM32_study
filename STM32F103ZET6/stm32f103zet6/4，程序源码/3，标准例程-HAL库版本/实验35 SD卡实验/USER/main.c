#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sram.h"
#include "malloc.h" 
#include "sdio_sdcard.h"
/************************************************
 ALIENTEK ��ӢSTM32F103������ ʵ��35
 SD��ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/  
    
//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	uint64_t CardCap;	//SD������
	HAL_SD_CardCIDTypeDef SDCard_CID;

	HAL_SD_GetCardCID(&SDCARD_Handler,&SDCard_CID);	//��ȡCID
	SD_GetCardInfo(&SDCardInfo);                    //��ȡSD����Ϣ
	switch(SDCardInfo.CardType)
	{
		case CARD_SDSC:
		{
			if(SDCardInfo.CardVersion == CARD_V1_X)
				printf("Card Type:SDSC V1\r\n");
			else if(SDCardInfo.CardVersion == CARD_V2_X)
				printf("Card Type:SDSC V2\r\n");
		}
		break;
		case CARD_SDHC_SDXC:printf("Card Type:SDHC\r\n");break;
	}	
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//����SD������
  	printf("Card ManufacturerID:%d\r\n",SDCard_CID.ManufacturerID);					//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);								//����Ե�ַ
	printf("LogBlockNbr:%d \r\n",(u32)(SDCardInfo.LogBlockNbr));					//��ʾ�߼�������
	printf("LogBlockSize:%d \r\n",(u32)(SDCardInfo.LogBlockSize));					//��ʾ�߼����С
	printf("Card Capacity:%d MB\r\n",(u32)(CardCap>>20));							//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);						//��ʾ���С
}

//����SD���Ķ�ȡ
//��secaddr��ַ��ʼ,��ȡseccnt������������
//secaddr:������ַ
//seccnt:������
void sd_test_read(u32 secaddr,u32 seccnt)
{
	u32 i; u8 *buf; u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//�����ڴ�
	sta=SD_ReadDisk(buf,secaddr,seccnt);//��ȡsecaddr������ʼ������
	if(sta==0)						
	{	 
		printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)printf("%x ",buf[i]);//��ӡsecaddr��ʼ����������    	   
		printf("\r\nDATA ENDED\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);	//�ͷ��ڴ�	   
}

//����SD����д��(����,���дȫ��0XFF������,���������SD��.)
//��secaddr��ַ��ʼ,д��seccnt������������
//secaddr:������ַ
//seccnt:������
void sd_test_write(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf; u8 sta=0; 
	buf=mymalloc(SRAMEX,seccnt*512);		//��SRAM�����ڴ�
	for(i=0;i<seccnt*512;i++) buf[i]=i*3; 	//��ʼ��д�������,��3�ı���.
	sta=SD_WriteDisk(buf,secaddr,seccnt);	//��secaddr������ʼд��seccnt����������
	if(sta==0) printf("Write over!\r\n");		  
	else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);						//�ͷ��ڴ�	   
}

int main(void)
{	
    u8 key;		 
	u32 sd_size;
	u8 t=0;	
	u8 *buf;
	uint64_t CardCap;				    //SD������
	
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������	
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
 	LCD_Init();           				//��ʼ��LCD FSMC�ӿ� 
	SRAM_Init();						//��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);				//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);				//��ʼ���ⲿ�ڴ��
	
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"ELITE STM32 ^_^");	
	LCD_ShowString(30,70,200,16,16,"SD CARD TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2019/9/19");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Read Sector 0");	   
 	while(SD_Init())//��ⲻ��SD��
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	show_sdcard_info();	//��ӡSD�������Ϣ
 	POINT_COLOR=BLUE;	//��������Ϊ��ɫ 
	//���SD���ɹ� 											    
	LCD_ShowString(30,150,200,16,16,"SD Card OK    ");
	LCD_ShowString(30,170,200,16,16,"SD Card Size:     MB");
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//����SD������
	LCD_ShowNum(30+13*8,170,CardCap>>20,5,16);//��ʾSD������
	while(1)
    {
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0������
		{
			buf=mymalloc(0,512);		//�����ڴ�
			if(SD_ReadDisk(buf,0,1)==0)	//��ȡ0����������
			{	
				LCD_ShowString(30,190,200,16,16,"USART1 Sending Data...");
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//��ӡ0��������    	   
				printf("\r\nDATA ENDED\r\n");
				LCD_ShowString(30,190,200,16,16,"USART1 Send Data Over!");
			}
			myfree(0,buf);//�ͷ��ڴ�	   
		}   
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;
			t=0;
		}
	}
}

