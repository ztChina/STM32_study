//ʵ�����I2C��MPU6050ͨ��
/*
  �������ӣ�
  SCL: PB7		SDA: PB9	INT: PA12���ж����������
*/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//I2C��ʼ������
void MyI2C_Init(void)
{
	//����PBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//����PB8��PB9Ϊ��©���ģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//��©ʱ,����ߵ�ƽΪ����̬,��©ģʽ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//��SCL��SDA����Ϊ��ʼ�ߵ�ƽ�ĸ���̬
	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9);	
}

//һϵ��SCL��SDA λ��д����,���ÿ�ζ�д����ʱ10΢��
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

//ͨ����ʼ����
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);//Ϊ�����ظ���ʼSr����֪��SDA״̬����������ж�Ϊ��ֹ���������ø�SDA
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);//��ʼ������SDA���½���SCL���½�
}

//ͨ����ֹ����
void MyI2C_Stop(void)
{
	//MyI2C_W_SCL(0);//ͨ�Ž���ʱSCL���ǵ͵�ƽ�ģ�����ʡ��
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);//��ֹ������SCL��������SDA������
}

//����һ���ֽں���
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;//����λ
	for(i=0;i<8;i++) //��λ�ȷ���
	{
		MyI2C_W_SDA(Byte & (0x80>>i));//	>>i ��ʾ���� i λ
		MyI2C_W_SCL(1);//һ��SCL�����ط���һ��
		MyI2C_W_SCL(0);
	}
}

//����һ���ֽں���
uint8_t MyI2C_ReceiveByte(void)
{
	MyI2C_W_SDA(1);//��ȡ֮ǰ����Ҫ���ͷ�SDA
	uint8_t Byte=0x00;
	uint8_t i;
	for(i=0;i<8;i++)//��λ�Ƚ���
	{
		MyI2C_W_SCL(1);//һ��SCL�����ض�ȡһ��
		if(MyI2C_R_SDA()==1)
		{
			Byte= Byte | (0x80>>i);
		}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

//����Ӧ����
void MyI2C_SendAck(uint8_t AckBit)
{
		MyI2C_W_SDA(AckBit);
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
}

//����Ӧ����
uint8_t MyI2C_ReceiveBAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);//��ȡ֮ǰ����Ҫ���ͷ�SDA
 	MyI2C_W_SCL(1);
	if(MyI2C_R_SDA()==1)
	{
		AckBit=1;
	}
	MyI2C_W_SCL(0);
	return AckBit;
}
