实验器材:
	精英STM32F103开发板V1版本
	
实验目的:
	学习STM32的IAP功能,掌握IAP的使用.
	
硬件资源:
	1,DS0(连接在PB5)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A10接RS)
	4,按键KEY0(PE4)/KEY1(PE3)/KEY_UP(PA0,也称之为WK_UP)
	
实验现象:
	本实验开机的时候先显示提示信息，然后等待串口输入接收APP程序（无校验，一次性接收），在串口接收
	到APP程序之后，即可执行IAP。如果是SRAM APP，通过按下KEY0即可执行这个收到的SRAM APP程序。如果
	是FLASH APP，则需要先按下KEY_UP按键，将串口接收到的APP程序存放到STM32的FLASH，之后再按KEY1既
	可以执行这个FLASH APP程序。
 
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,本实验下载成功后,需用串口调试助手发送SRAM APP/FLASH APP等APP代码(.bin文件)验证IAP功能.

			正点原子@ALIENTEK 
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://eboard.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com



































