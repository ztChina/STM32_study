实验器材:
	精英STM32F103开发板V1版本
	
实验目的:
	学习STM32 SPI接口的使用,驱动W25Q128实现SPI FLASH数据读写.
	
硬件资源:
	1,DS0(连接在PB5)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A10接RS) 
	4,按键KEY0(PE4)/KEY1(PE3)
	5,SPI2(PB12/13/14/15)
	6,W25Q128(SPI FLASH芯片,连接在SPI2上)
	
实验现象:
	本实验通过KEY1按键来控制W25Q128的写入，通过另外一个按键KEY0来控制W25Q128的读取。并在LCD模块
	上面显示相关信息。DS0提示程序正在运行。同时，我们可以通过USMART控制读取W25QXX的ID或者整片擦除。
  
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,W25Q128和NRF24L01接口共用SPI2,所以他们必须分时复用SPI2,使用一个的时候,必须禁止另外一个的片选.


			正点原子@ALIENTEK 
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://eboard.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com



































