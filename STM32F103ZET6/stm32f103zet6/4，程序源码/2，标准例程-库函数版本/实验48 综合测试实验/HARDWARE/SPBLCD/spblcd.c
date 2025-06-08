#include "spblcd.h"
#include "dma.h"
#include "lcd.h"
#include "delay.h"
#include "malloc.h"
#include "usart.h"
#include "w25qxx.h"
#include "spi.h"
#include "spb.h"
#include "ucos_ii.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//SPBЧ��ʵ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/3/6
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//*******************************************************************************
//V1.1 20150322
//����gui_draw_bline����
//�޸����4.3/7��������ͼ����ʾ��ʽ,ֱ��ȫ������DMA,������ٶ�.
//V1.2 20150325
//�޸�LCD�Դ�Ĵ洢����ΪSPI FLASH
////////////////////////////////////////////////////////////////////////////////// 	

//��lcd������spi flash
//frame:֡���,0��1,������д��0֡(��ִ�в�������),�ٸ��µ�1֡
//����ֵ:0,�ɹ�;
//    ����,�������
u8 slcd_frame_lcd2spi(u8 frame)
{
	u8 temp;
	u16 *pbuf;  
	u8* p;
	u32 startx;		//��ʼ��ַ  
	u16 i,j,k;
	u32 woffset=0;	//д��ַƫ��
	startx=SPILCD_BASE+1+frame*spbdev.spbwidth*spbdev.spbheight*2;	//�õ���ʼ��ַ(�ֽ�)
	pbuf=mymalloc(SRAMIN,4096);						//�����ڴ�
	if(!pbuf)return 1;
	woffset=0; 
	k=0;
	for(i=0;i<spbdev.spbwidth;i++)
	{
		for(j=0;j<spbdev.spbheight;j++)
		{
			pbuf[k++]=LCD_ReadPoint(i,j);			//��ȡ�� 
			if(k==2048)
			{
				p=(u8*)pbuf;
				for(k=0;k<2048;k++)					//��ת�ߵ��ֽ�
				{
					temp=p[2*k];
					p[2*k]=p[2*k+1];
					p[2*k+1]=temp;
				}
				W25QXX_Write((u8*)pbuf,startx+woffset,4096);//д��һ������
				woffset+=4096;
				k=0;
			}
		}
	}
	if(k)//����һ������Ҫд��
	{
 		p=(u8*)pbuf;
		for(i=0;i<k;i++)							//��ת�ߵ��ֽ�
		{
			temp=p[2*i];
			p[2*i]=p[2*i+1];
			p[2*i+1]=temp;
		}
		W25QXX_Write((u8*)pbuf,startx+woffset,k*2);	//д�����һ������
	}
	myfree(SRAMIN,pbuf);							//�ͷ��ڴ�	
	return 0;
} 
//SPI2ģʽ����
//mode:0��8λ��ͨģʽ��1,16λDMAģʽ  
void slcd_spi2_mode(u8 mode)
{
	u16 tempreg=0;
	if(mode==0)	//8λ��ͨģʽ,��������SPI
	{
		RCC->APB1RSTR|=1<<14;	//��λSPI2
		RCC->APB1RSTR&=~(1<<14);//ֹͣ��λSPI2
		tempreg|=0<<10;			//ȫ˫��ģʽ	
		tempreg|=1<<9;			//���nss����
		tempreg|=1<<8;			 
		tempreg|=1<<2;			//SPI����  
		tempreg|=0<<11;			//8λ���ݸ�ʽ	
		tempreg|=1<<1;			//����ģʽ��SCKΪ1 CPOL=1 
		tempreg|=1<<0;			//���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1   
		tempreg|=0<<3;			//2��Ƶ,����Ϊ42MhzƵ��
		tempreg|=0<<7;			//MSB First  
		tempreg|=1<<6;			//SPI���� 
		SPI2->CR1=tempreg; 		//����CR1  
		SPI2->I2SCFGR&=~(1<<11);//ѡ��SPIģʽ 
	}else						//16λDMAģʽ
	{
		SPI2->CR1|=1<<10; 		//������ģʽ   
		SPI2->CR1|=1<<11;		//16bit���ݸ�ʽ	
		SPI2->CR2|=1<<0;		//ʹ��DMA���� 
	}   
} 
//SPI2 --> LCD_RAM dma����
//16λ,SPI2_RX���䵽LCD_RAM.
void slcd_dma_init(void)
{  
	RCC->AHBENR|=1<<0;				//����DMA1ʱ��
	delay_ms(5);					//�ȴ�DMAʱ���ȶ�
	DMA1_Channel4->CPAR=(u32)&SPI2->DR;			//�����ַΪ:SPI1->DR
	DMA1_Channel4->CMAR=(u32)(u32)&LCD->LCD_RAM;//Ŀ���ַΪLCD_RAM
 	DMA1_Channel4->CNDTR=0;			//DMA1,����������,��ʱ����Ϊ0
	DMA1_Channel4->CCR=0X00000000;	//��λ
	DMA1_Channel4->CCR|=0<<4;		//�������
	DMA1_Channel4->CCR|=0<<5;		//��ͨģʽ
	DMA1_Channel4->CCR|=0<<6;		//�����ַ������ģʽ
	DMA1_Channel4->CCR|=0<<7;		//�洢��������ģʽ
	DMA1_Channel4->CCR|=1<<8;		//�������ݿ��Ϊ16λ
	DMA1_Channel4->CCR|=1<<10;		//�洢�����ݿ��16λ
	DMA1_Channel4->CCR|=1<<12;		//�е����ȼ�
	DMA1_Channel4->CCR|=0<<14;		//�Ǵ洢�����洢��ģʽ
} 
//����һ��SPI��LCD��DMA�Ĵ��� 
void slcd_dma_enable(void)
{	  
	u32 lcdsize=spbdev.spbwidth*spbdev.spbheight;
	u32 dmatransfered=0;
	while(lcdsize)
	{     	  
		DMA1_Channel4->CCR&=~(1<<0);			//�ر�DMA���� 
		DMA1->IFCR|=1<<13;						//����ϴεĴ�����ɱ�� 
		if(lcdsize>SLCD_DMA_MAX_TRANS)
		{
			lcdsize-=SLCD_DMA_MAX_TRANS;
			DMA1_Channel4->CNDTR=SLCD_DMA_MAX_TRANS;	//���ô��䳤��
		}else
		{
			DMA1_Channel4->CNDTR=lcdsize;				//���ô��䳤��
			lcdsize=0;
		}	
 		dmatransfered+=SLCD_DMA_MAX_TRANS;		
		DMA1_Channel4->CCR|=1<<0;				//����DMA RX���� 		
		while((DMA1->ISR&(1<<13))==0);			//�ȴ�������� 
	} 
	DMA1_Channel4->CCR&=~(1<<0);				//�ر�DMA���� 
}
//��ʾһ֡,������һ��spi��lcd����ʾ.
//x:����ƫ����
void slcd_frame_show(u16 x)
{
    OS_CPU_SR cpu_sr=0;
	u32 addr=SPILCD_BASE+1+(x*spbdev.spbheight)*2; 
	LCD_Scan_Dir(U2D_L2R);		//����ɨ�跽��
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X7789)
	{
		LCD_Set_Window(spbdev.stabarheight,0,spbdev.spbheight,spbdev.spbwidth);
		LCD_SetCursor(spbdev.stabarheight,0);//���ù��λ�� 
	}else
	{
		LCD_Set_Window(0,spbdev.stabarheight,spbdev.spbwidth,spbdev.spbheight);
		if(lcddev.id!=0X1963)LCD_SetCursor(0,spbdev.stabarheight);//���ù��λ�� 		
	}
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 
	W25QXX_CS=0;                       		//ʹ������   
    SPI2_ReadWriteByte(W25X_FastReadData);	//���Ϳ��ٶ�ȡ����   
    SPI2_ReadWriteByte((u8)((addr)>>16));	//����24bit��ַ    
    SPI2_ReadWriteByte((u8)((addr)>>8));   
    SPI2_ReadWriteByte((u8)addr);  
    SPI2_ReadWriteByte(0XFF);	//8 dummy clock  
	slcd_spi2_mode(1); 			//����Ϊ16λ��,DMA����
	OS_ENTER_CRITICAL();		//�����ٽ���(�޷����жϴ��)    	
	slcd_dma_enable();			//����һ��SPI��LCD��dma���� 
	OS_EXIT_CRITICAL();			//�˳��ٽ���(���Ա��жϴ��)
	slcd_spi2_mode(0);			//�ָ�8λģʽ
	//���������,�ж��ǲ��ǰ���.
	W25QXX_CS=1;  	 	 		//��ֹW25QXX
	LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ�Ϸ���	
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//�ָ�Ĭ�ϴ��ڴ�С	 
}








 








