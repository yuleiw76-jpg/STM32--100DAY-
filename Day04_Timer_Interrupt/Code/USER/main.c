#include "led.h"
#include "delay.h"
#include "sys.h"
#include "tim.h"



 int main(void)
 {	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	  
	LED_Init();		  	
	TIM3_Init(20000-1,720-1);
	while(1)     
	{
		delay_ms(100);	 						//延时100ms
	}
 }
 

