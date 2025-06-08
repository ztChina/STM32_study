#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"



uint8_t KeyNum;		//按键是否按下
uint16_t ArrayWrite[] = {0x1234, 0x5678};
uint16_t ArrayRead[2];



int main()
{
	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//电源控制模块
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);		//备份寄存器模块
	//设置PWR_CR的DBP，使能对BKP和RTC的访问
	PWR_BackupAccessCmd(ENABLE);

	
	while(1)
	{
		//按键读取
		KeyNum = Key_GetNum();
		//按键按下，读写 BKP 的数据寄存器
		if (KeyNum == 1)
		{
			ArrayWrite[0] ++;
			ArrayWrite[1] ++;
			
			BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]);
			BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]);
			
			OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);
			OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
		}
		
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		
		OLED_ShowHexNum(2, 3, ArrayRead[0], 4);
		OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
		
	}
}





