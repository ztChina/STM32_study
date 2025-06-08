#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

//W25Q64初始化函数
void W25Q64_Init(void)
{
	MySPI_Init();
}

//读取 W25Q64 ID号
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();	//SPI开始操作
	MySPI_SwapByte(W25Q64_JEDEC_ID);	//获取设备ID指令，接受数据没有意义
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//获取厂商ID，发送没有意义随意写
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//获取设备ID高 8 位
	*DID <<= 8;	
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//低 8 位
	MySPI_Stop();
}

//W25Q64 写使能操作
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);	//发送写使能指令
	MySPI_Stop();
}

//等待Busy标志位为0
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);	//读寄存器1指令，寄存器1最后一位是 Busy 标志位
	Timeout = 100000;
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) //等待Busy标志位变为0
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}

//写page操作
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();	//首先写使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);	//发送页编程指令
	//发送地址,W25Q64地址一共有24位，先从高位开始发
	MySPI_SwapByte(Address >> 16);	//发送高16位，但其实只有16位中的低8位有用
	MySPI_SwapByte(Address >> 8);		//发送中8位
	MySPI_SwapByte(Address);				//发送低8位
	//发送数组到页面缓冲区
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	//等待Busy
	//等待页面缓冲区中的数据从指定地址字节开始写，超出本页则到下一页
	W25Q64_WaitBusy();
}

//指定地址擦除操作，但最小擦除单位是page
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	//4KB扇区擦除
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

//指定地址读数据
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}
