#include "motor.h"
#define ax 0.3
#define bx 0.7

void go(int pid_out1,int pid_out2)
{
	int pid_out=bx*pid_out1+ax*pid_out2;

	
	
	if(pid_out>=0)
	{

		TIM_SetCompare2(TIM4,pid_out);
		TIM_SetCompare1(TIM4,0);
		
		TIM_SetCompare4(TIM4,pid_out);
		TIM_SetCompare3(TIM4,0);

	}
	else if(pid_out<0)
	{
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare1(TIM4,-pid_out);

		TIM_SetCompare4(TIM4,0);
		TIM_SetCompare3(TIM4,-pid_out);
	}
}


void motor_test(void)
{
	TIM_SetCompare1(TIM4,1000);
	TIM_SetCompare2(TIM4,0);
}
