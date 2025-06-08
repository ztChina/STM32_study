#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Store.h"
#include "Key.h"

uint8_t KeyNum;					//�������ڽ��հ�������ı���

int main(void)
{
	/*ģ���ʼ��*/
	OLED_Init();				//OLED��ʼ��
	Key_Init();					//������ʼ��
	Store_Init();				//�����洢ģ���ʼ�������ϵ��ʱ����������ݼ��ػ�Store_Data��ʵ�ֵ��粻��ʧ
	
	/*��ʾ��̬�ַ���*/
	OLED_ShowString(1, 1, "Flag:");
	OLED_ShowString(2, 1, "Data:");
	
	while (1)
	{
		KeyNum = Key_GetNum();		//��ȡ��������
		
		if (KeyNum == 1)			//����1����
		{
			Store_Data[1] ++;		//�任��������
			Store_Data[2] += 2;
			Store_Data[3] += 3;
			Store_Data[4] += 4;
			Store_Save();			//��Store_Data�����ݱ��ݱ��浽���棬ʵ�ֵ��粻��ʧ
		}
		
		if (KeyNum == 2)			//����2����
		{
			Store_Clear();			//��Store_Data������ȫ����0
		}
		
		OLED_ShowHexNum(1, 6, Store_Data[0], 4);	//��ʾStore_Data�ĵ�һλ��־λ
		OLED_ShowHexNum(3, 1, Store_Data[1], 4);	//��ʾStore_Data����Ч�洢����
		OLED_ShowHexNum(3, 6, Store_Data[2], 4);
		OLED_ShowHexNum(4, 1, Store_Data[3], 4);
		OLED_ShowHexNum(4, 6, Store_Data[4], 4);
	}
}
