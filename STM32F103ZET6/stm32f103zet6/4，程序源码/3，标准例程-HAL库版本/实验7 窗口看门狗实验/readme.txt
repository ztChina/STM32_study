实验器材:
	精英STM32F103开发板
	
实验目的:
	学习窗口看门狗的使用
	
硬件资源:
	1,DS0(连接在PB5)
	2,DS1(连接在PE5)
	3,窗口看门狗(WWDG)
	
实验现象:
	本实验, 程序一运行则开启WWDG,并使得DS0亮300ms后关闭，进入死循环。等待WWDG中断的
	到来，在中断里面，喂狗，并执行DS1的翻转操作。所以可以看到DS1不停的闪烁，而DS0只在
	刚启动的时候闪一下。  
	
注意事项:
	无.


			正点原子@ALIENTEK 
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://eboard.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com
