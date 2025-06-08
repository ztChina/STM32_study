//主板GPIOA连接CH340串口转换器的USRAT引脚
//PA9作为STM32硬件USART1的TX脚
//PA10作为STM32硬件USART1的RX脚

#include "stm32f10x.h"                  // Device header
#include "Serial.h"

uint8_t Serial_RxData = 0;	//储存中断读取的数据
uint8_t Serial_RxFlag = 0;	//作为串口读取中断标志位


void Serial_Init(void)
{
	//开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//初始化PA9和PA10
	GPIO_InitTypeDef gpio_initStructure;
	gpio_initStructure.GPIO_Mode=GPIO_Mode_IPU;//PA10是上拉输入（PA10作为输入，也可以是浮空输入）
	gpio_initStructure.GPIO_Pin=GPIO_Pin_10;
	gpio_initStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initStructure);
	gpio_initStructure.GPIO_Mode=GPIO_Mode_AF_PP;//PA9是推挽输出
	gpio_initStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOA,&gpio_initStructure);

	//打开USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//初始化UASRT
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//配置波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//使用流控选择
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//模式选择
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位选择
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位长度选择
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//数据帧长度
	USART_Init(USART1,&USART_InitStructure);
	
	//USART中断方式读取
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//USATR中断源选择
	//中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级组别选择
	NVIC_InitTypeDef NVIC_InitStructure;//初始化结构体
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	//启动USART1
	USART_Cmd(USART1,ENABLE);
}

//判断串口读取中断是否发生
uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

//取出串口中断时的读取数据
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}


//NVIC中断函数
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET )	//判断读取寄存器为空
	{
		Serial_RxData = USART_ReceiveData(USART1); //读取数据
		Serial_RxFlag = 1;
		
		//手动清除中断标志位
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}


//串口发送函数
void Serial_SendByte(uint8_t data)
{
	//发送数据，函数为内置
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//等待发送完成，此时寄存器标志位为空
}


//串口发送数组函数
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	for(uint16_t i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

//串口发送字符串
void Serial_SendString(char *String)
{
	for(uint16_t i = 0; String[i] != '\0'; i++)		//在c语言中，字符串以 '\0' 结尾
	{
		Serial_SendByte(String[i]);
	}
}

//串口用次方计算函数
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
		uint32_t Result = 1;
		while(Y--)
		{
			Result*=X;
		}
		return Result;
}

//串口发送十进制数
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	for(uint8_t i = 0; i < Length; i++)
	{
		//从个位开始发送
		Serial_SendByte((uint8_t)(Number / Serial_Pow(10, Length - (i+1) ) ) + '0');
		//Serial_SendByte(Number / Serial_Pow(10, Length - (i+1) ) % 10  + '0');	// +'0' 是因为要对照编码标准进行偏移
	}
}

//将 printf 函数重定向到 Serial 输出
//fputc 本来是 printf 函数的最底层函数，但 printf 只能输出到屏幕，不能串口输出
//这样重定向底层函数的话，可以将 printf 输出到串口
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

//封装 sprintf，可同时发送多个String
void Serial_Printf(char *format,...)		//	...	表示可变参数
{
	char String[100];
	va_list arg;	//（可变参数列表） 参数
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
