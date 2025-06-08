//���õ� MPU6050 �Ĵ����궨��
#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H

#define	MPU6050_SMPLRT_DIV		0x19	//��Ƶ��
#define	MPU6050_CONFIG			0x1A	//���üĴ����������ⲿͬ���͵�ͨ�˲���
#define	MPU6050_GYRO_CONFIG		0x1B	//�����ǣ�����ٶȣ�����
#define	MPU6050_ACCEL_CONFIG	0x1C	//���ٶȲ�������

#define	MPU6050_ACCEL_XOUT_H	0x3B	//���ٶ�ֵ�Ĵ���
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41	//�¶ȴ�����ֵ�Ĵ���
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43	//���ٶ�ֵ�Ĵ���
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B	//��Դ����1
#define	MPU6050_PWR_MGMT_2		0x6C	//��Դ����2
#define	MPU6050_WHO_AM_I		0x75	//��MPU6050������0x68

#endif
