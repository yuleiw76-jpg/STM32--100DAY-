#include "pid.h"
 
static float err_last_last=0;	//上上次的误差
static float err_last=0;  		//上次的误差
static float err=0;			//本次的误差


static float P=100;			//参数P
static float I=30; 		//参数I
static float D=5;	 		//参数D
 
static float off_cal = 0;

float a_m=0.0;				//输出值

float aim_v=0.5;				//目标速度：2r/s

float pid_control(float speed)
 {	 
		err_last_last=err_last;	//更新上上次的误差
		err_last=err;			//更新上次的误差
		err=(aim_v - speed);		//更新本次误差
		
		off_cal=P*(err-err_last)+I*err+D*(err+err_last_last-2*err_last); //增量式PID的公式

		a_m+= off_cal; 			//得到输出值

		if (a_m > 1000)
    a_m = 1000;

    if (a_m < 0)
    a_m = 0;
	 return a_m;				//输出a_m
 }
 
