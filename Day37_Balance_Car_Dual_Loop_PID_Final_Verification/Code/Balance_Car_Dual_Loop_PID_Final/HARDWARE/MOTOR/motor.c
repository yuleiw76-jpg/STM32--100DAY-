#include "motor.h"
#define ax 0.05 //速度环先以5%权重接入，验证方向后再逐步增加
#define bx 1.0

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
