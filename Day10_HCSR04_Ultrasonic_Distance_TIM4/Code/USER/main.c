#include "led.h"
#include "delay.h"
#include "sys.h"
#include "tim.h"
#include "hcsr04.h"
#include "oled.h"
#include "stdio.h"

float a;
int main(void)
{	
	char text[10];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	  
	LED_Init();		  	
	TIM4_Init(65536-1,0);
	OLED_Init();
	OLED_Clear();
	HC_SR04_Init();
while(1)     
{
	a=Senor_Using();
	sprintf(text,"%.2fcm       ",a);
	OLED_ShowString(0,0,(u8*)text,16);
	delay_ms(100);	 						//延时100ms
}
}
 

