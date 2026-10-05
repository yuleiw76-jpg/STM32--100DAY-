#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"	 

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	      
	LED_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	KEY_Init();	 //按键初始化
	EXTIX_Init();
	uart_init(9600);

	while(1)     
	{
		delay_ms(100);
		//printf("ok\r\n");
	}
}

