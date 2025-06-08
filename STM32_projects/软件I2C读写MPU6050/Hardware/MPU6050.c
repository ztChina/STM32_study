#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050ADDRESS	0xD0	//AD0δ�ӵĵ�ַ

//��MPU6050ָ����ַд
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t RegData)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050ADDRESS);
	MyI2C_ReceiveBAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveBAck();
	MyI2C_SendByte(RegData);
	MyI2C_ReceiveBAck();
	MyI2C_Stop();
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);	//��Դ����1��0x6B
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);	//��Դ����1
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);	//��Ƶ����0x09ʮ��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);		//���üĴ���
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);	//��Ƕ�����
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//����ٶ�����
}

//��MPU6050ָ����ַ��
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050ADDRESS);
	MyI2C_ReceiveBAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveBAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050ADDRESS | 0x01);
	MyI2C_ReceiveBAck();
	data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return data;
}

//��ȡMPU6050��accel �� gyro ����
void MPU6050_GetValues(int16_t *AccX,int16_t *AccY,int16_t *AccZ,	//AccX��ָ�룬*AccX��ָ��ָ���µ�����
						int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)
{
	uint8_t Data_H,Data_L=0;
	
	Data_H=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	Data_L=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	Data_L=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	Data_L=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	Data_L=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	Data_L=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY=(Data_H<<8)|Data_L;
	
	Data_H=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	Data_L=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ=(Data_H<<8)|Data_L;
}
