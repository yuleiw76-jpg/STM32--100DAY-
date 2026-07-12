#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "track.h"
extern int track_error;
int main(void)
{	
	delay_init();
	LED_Init();
	TIM1_PWM_Init(20000-1,72-1); //舵机的arr值为500-2500
	track_Init();
	
	while(1)     
	{
		delay_ms(10);
		track_scan();
		servo_control(track_error);
	}
}
