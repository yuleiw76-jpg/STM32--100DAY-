#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "rtc.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	      
	LED_Init();
	KEY_Init();	 //按键初始化
	EXTIX_Init();
	uart_init(115200);
	RTC_Init();

	while(1)     
	{
		delay_ms(100);
		printf("%u ",calendar.w_year);
		printf("%u ",calendar.w_month);
		printf("%u ",calendar.w_date);
		printf("%u ",calendar.week);
		printf("%u ",calendar.hour);
		printf("%u ",calendar.min);
		printf("%u ",calendar.sec);
		calendar.m_sec=(32767-RTC_GetDivider())*1000/32767;
		printf("%u\r\n",calendar.m_sec);
	}
}

