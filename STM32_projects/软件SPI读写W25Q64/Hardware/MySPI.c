#include "stm32f10x.h"                  // Device header

//SS写函数
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

//CLK写函数
void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

//MOSI写操作
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

//MISO读操作
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

//SPI引脚初始化
//4作为SS，5作为时钟CLK，7作为MOSI，6作为MISO
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MySPI_W_SS(1);//SS初始置1
	MySPI_W_SCK(0);//CLK初始置0
}

//SPI开始操作，就是控制片选SS引脚
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

//SPI停止操作
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

//写一个字节操作，同时也会读出一个字节
//默认选择SPI的模式0，即第一个边沿移入数据，第二个边沿移除数据
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i));	//右移输出
		MySPI_W_SCK(1);
		if (MySPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}	//右移输入
		MySPI_W_SCK(0);
	}
	
	return ByteReceive;
}
