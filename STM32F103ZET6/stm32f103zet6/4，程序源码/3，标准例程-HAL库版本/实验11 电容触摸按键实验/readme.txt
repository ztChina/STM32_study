实验器材:
	精英STM32F103开发板
	
实验目的:
	进一步学习定时器的输入捕获功能
	
硬件资源:
	1,DS0(连接在PB5)
	2,TPAD电容触摸按键(右下角LOGO,即TPAD,连接在PA1)
	3,定时器5(TIM5),TIM5的通道2(TIM5_CH2,连接在PA1上面).
	
实验现象:
	本实验,利用开发板板载的电容触摸按键(右下角白色LOGO,即TPAD)，通过TIM5_CH2（PA1）对
	电容触摸按键的检测，实现对DS1的控制,下载本代码后，我们通过按开发板右下角的TPAD按钮
	，就可以控制DS1的亮灭了。
	
注意事项:
	1,本实验需要将P10端子用跳线帽连接ADC&TPAD. 

			正点原子@ALIENTEK 
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://eboard.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com
