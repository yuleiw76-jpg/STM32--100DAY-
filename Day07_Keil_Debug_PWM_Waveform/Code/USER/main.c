#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "key.h"

int a=0;
int main(void)
{	
	delay_init();
	LED_Init();
	KEY_Init();
	TIM1_PWM_Init(20000-1,72-1);
	TIM_SetCompare1(TIM1,5000);
	TIM_SetCompare2(TIM1,10000);
	TIM_SetCompare3(TIM1,15000);
	TIM_SetCompare4(TIM1,19000);
	if(a==1)
	{
		delay_ms(10);
	}
	else
	{
		delay_ms(20);
	}
	
	while(1)     
	{
		delay_ms(10);
		a++;
	}
}
