#include "led.h"
#include "delay.h"
#include "sys.h"



 int main(void)
 {	

	delay_init();	    //延时初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口

	while(1)     //loop()
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);		//PC.13 输出低
		delay_ms(100);	 						//延时100ms
		GPIO_SetBits(GPIOC,GPIO_Pin_13);		//PC.13 输出高
		delay_ms(100);							//延时100ms
	}
 }
 
 
 //注意简化版的库函数
 //u8 a=0;    a=PCin(13);   假如PC13口是某种输入模式，这两句可以检测PC13口是高电平（a=1）还是低电平（a=0）
 //PCin(13)=1  PCin(13)=0;  假如PC13口是某种输出模式，这两句可以控制PC13口是高电平（1）还是低电平（0）

