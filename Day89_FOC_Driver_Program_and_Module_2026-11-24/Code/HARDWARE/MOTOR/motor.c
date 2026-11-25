#include "motor.h"
#include "led.h"
#include "delay.h"
#include "encoder.h" 

void MotorAngle_Init()
{
	TIM_SetCompare1(TIM1,500);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);
	delay_ms(1000);
	angle_ori=getAngle();

	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);
}
