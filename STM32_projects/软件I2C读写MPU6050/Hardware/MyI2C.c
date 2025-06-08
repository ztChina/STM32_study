//实现软件I2C与MPU6050通信
/*
  引脚连接：
  SCL: PB7		SDA: PB9	INT: PA12（中断引脚输出）
*/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//I2C初始化函数
void MyI2C_Init(void)
{
	//开启PB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置PB8和PB9为开漏输出模式
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏时,输出高电平为高阻态,开漏模式可以输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//将SCL和SDA配置为初始高电平的高阻态
	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9);	
}

//一系列SCL、SDA 位读写操作,最好每次读写后延时10微秒
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)BitValue);
	Delay_us(10);
}
void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)BitValue);
	Delay_us(10);
}
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
	Delay_us(10);
	return BitValue;
}

//通信起始函数
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);//为兼容重复起始Sr，不知道SDA状态，避免错误判断为终止条件，先置高SDA
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);//起始条件：SDA先下降，SCL再下降
}

//通信终止函数
void MyI2C_Stop(void)
{
	//MyI2C_W_SCL(0);//通信结束时SCL就是低电平的，所以省略
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);//终止条件：SCL先上升，SDA再上升
}

//发送一个字节函数
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;//右移位
	for(i=0;i<8;i++) //高位先发送
	{
		MyI2C_W_SDA(Byte & (0x80>>i));//	>>i 表示右移 i 位
		MyI2C_W_SCL(1);//一个SCL上升沿发送一次
		MyI2C_W_SCL(0);
	}
}

//接收一个字节函数
uint8_t MyI2C_ReceiveByte(void)
{
	MyI2C_W_SDA(1);//读取之前主机要先释放SDA
	uint8_t Byte=0x00;
	uint8_t i;
	for(i=0;i<8;i++)//高位先接收
	{
		MyI2C_W_SCL(1);//一个SCL上升沿读取一次
		if(MyI2C_R_SDA()==1)
		{
			Byte= Byte | (0x80>>i);
		}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

//发送应答函数
void MyI2C_SendAck(uint8_t AckBit)
{
		MyI2C_W_SDA(AckBit);
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
}

//接收应答函数
uint8_t MyI2C_ReceiveBAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);//读取之前主机要先释放SDA
 	MyI2C_W_SCL(1);
	if(MyI2C_R_SDA()==1)
	{
		AckBit=1;
	}
	MyI2C_W_SCL(0);
	return AckBit;
}
