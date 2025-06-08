//实现右轮测速
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Delay.h"

int16_t encoder_get_num=0;
int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	Encoder_Init();
	while(1)
	{
		OLED_ShowSignedNum(2,1,Encoder_Speed(),5);
		Delay_ms(1000);//Delay函数阻碍程序进程，可以使用另一个计时器的定时中断函数优化。
	}
}
