实验器材:
	精英STM32F103开发板V1版本
	
实验目的:
	学习STM32待机唤醒功能的使用
	
硬件资源:
	1,DS0(连接在PB5)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A10接RS) 
	4,KEY_UP按键(即WK_UP按键,连接在PA0)
	
实验现象:
	本实验通过长按（3秒）KEY_UP(WK_UP)按键开机，并且通过DS0的闪烁指示程序已经开始运行，再次长
	按该键，则进入待机模式，DS0关闭，程序停止运行。类似于手机的开关机。

	
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!!
	3,默认是待机状态,所以上电后必须长按KEY_UP(WK_UP)按键以开机.


			正点原子@ALIENTEK 
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://eboard.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com



































