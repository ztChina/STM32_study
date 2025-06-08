#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050ADDRESS	0xD0	//AD0未接的地址

//向MPU6050指定地址写
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
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);	//电源管理1：0x6B
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);	//电源管理1
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);	//分频器，0x09十分频
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);		//配置寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);	//测角度配置
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//测加速度配置
}

//向MPU6050指定地址读
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

//读取MPU6050的accel 和 gyro 数据
void MPU6050_GetValues(int16_t *AccX,int16_t *AccY,int16_t *AccZ,	//AccX是指针，*AccX是指向指针下的数据
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
