#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"

int main()
{
	OLED_Init();
	MPU6050_Init();
	//OLED_ShowString(1, 1, "HelloWorld!");
	uint8_t data=0;
	data=MPU6050_ReadReg(0x75);
	OLED_ShowBinNum(1,1,data,8); 
	
	int16_t AX,AY,AZ,GX,GY,GZ;
	
	while(1)
	{
		MPU6050_GetValues(&AX,&AY,&AZ,&GX,&GY,&GZ);
		OLED_ShowSignedNum(2,1,AX,5);OLED_ShowSignedNum(2,8,GX,5);
		OLED_ShowSignedNum(3,1,AY,5);OLED_ShowSignedNum(3,8,GY,5);
		OLED_ShowSignedNum(4,1,AZ,5);OLED_ShowSignedNum(4,8,GZ,5);
/**
  * @note ʵ���ʾ�������ٶȷ��򣨼����£�ΪAZ,ֵΪ +2064
  * @note ʵ���ʾǰ����ٶȷ���ΪAY
  * @note ǰ�������ٶ�ΪGX
  */ 
	}
}
