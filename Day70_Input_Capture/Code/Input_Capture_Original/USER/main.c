#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"
#include "capture.h"

int main(void)
{	
	delay_init();	      
	LED_Init();
	uart_init(9600);
	TIM3_PWM_Init(20000-1,72-1); 
	TIM2_Cap_Init(0XFFFF,72-1);		//以1Mhz的频率计数
	TIM_SetCompare1(TIM3,10000);
	
	while(1)     
	{
		delay_ms(100);
		calculate_fre();
	}
}
