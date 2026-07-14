#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "key.h"

int main(void)
{	
	u8 a;
	delay_init();
	LED_Init();
	KEY_Init();
	TIM1_PWM_Init(3000-1,0); //PWMÆµÂÊÎª24KHz
	
	while(1)     
	{
		delay_ms(10);
		a=KEY_Scan(0);
		
		if(a==KEY1_PRESS)
		{
			TIM_SetCompare1(TIM1,1500);
			TIM_SetCompare2(TIM1,0);
		}
		else if(a==KEY2_PRESS)
		{
			TIM_SetCompare1(TIM1,0);
			TIM_SetCompare2(TIM1,1500);
		}
	}
}
