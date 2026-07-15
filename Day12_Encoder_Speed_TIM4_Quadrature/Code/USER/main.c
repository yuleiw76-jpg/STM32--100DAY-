#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "key.h"
#include "tim.h"
#include "encode.h"
#include "oled.h"
#include "stdio.h"


extern float speed;
int main(void)
{	
	u8 a;
	char text[20];
	delay_init();
	LED_Init();
	TIM1_PWM_Init(3000-1,0); //PWM频率为24KHz
	TIM4_ENCODE_Init();
	TIM3_Init(10000-1,720-1); //100ms一次定时器中断
	OLED_Init();
	OLED_Clear();
	TIM_SetCompare1(TIM1,1500);
	TIM_SetCompare2(TIM1,0);
	
	while(1)     
	{
		delay_ms(10);
		sprintf(text,"%.2fr/s    ",speed);
		OLED_ShowString(8,2,(u8*)text,16);
		
	}
}
