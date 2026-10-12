#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"

int main(void)
{	
	u16 a,b;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	      
	LED_Init();
	uart_init(115200);
	Adc_Init();
	while(1)     
	{
		delay_ms(100);
		a=Get_Adc(ADC_Channel_1);
		b=Get_Adc(ADC_Channel_4);
		printf("(%u,%u)\r\n",a,b);
	}
}

