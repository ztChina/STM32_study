//�Ա��������м�������ʾ��OLED��Ļ��
//������Ϊƽ�⳵�����ֻ���������
//���ֽ���˳��ΪAO2��5V��PB7��PB6��GND��AO1�ֱ��ʾ�����-��������5V��������A�ࡢ������B�ࡢ������GND�������+
//����ΪBO2��5V��A0��A1��GND��BO1������˳��������ͬ
//AO �� BO �� TB6612 �ϵ�����
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"

int main()
{
	OLED_Init();
	Encoder_Init();
	while(1)
	{
		OLED_ShowSignedNum(2,1,Encoder_Get(),6);		
	}
}
