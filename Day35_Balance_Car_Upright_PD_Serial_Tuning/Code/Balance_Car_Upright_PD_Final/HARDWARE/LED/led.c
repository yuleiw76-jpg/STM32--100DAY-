#include "led.h"

void LED_Init(void)
{
 //（以下代码不需要死记硬背）
 GPIO_InitTypeDef  GPIO_InitStructure;    //初始化GPIO配置用的结构体，属于库函数
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PC.13 端口配置，13号口
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz，这个基本没改过，50MHZ是最高，低了可能会出一些问题
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.13
	
	
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PC.13 输出高
 GPIO_ResetBits(GPIOC,GPIO_Pin_13);						 //PC.13 输出低
 PCout(13)=0;                                            //PC.13 输出低（简化版库函数）
 PCout(13)=1;                                            //PC.13 输出高（简化版库函数）

}
 
