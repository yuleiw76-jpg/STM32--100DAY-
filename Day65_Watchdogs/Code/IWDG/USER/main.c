#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"	
#include "iwdg.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	      
	LED_Init();
	KEY_Init();	 //按键初始化
	EXTIX_Init();
	uart_init(9600);
	IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s

	while(1)     
	{
		delay_ms(100);
		printf("ok\r\n");
		IWDG_Feed();
	}
}

