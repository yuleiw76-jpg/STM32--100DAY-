#include "pid.h"
#include "MPU6050.h"

extern float Pitch,Roll,Yaw;

static float err_last_last=0;	//上上次的误差
static float err_last=0;  		//上次的误差
static float err=0;			//本次的误差


float P=750;			//参数P
static float I=0; 		//参数I
float D=1.5;	 		//参数D
 
static float off_cal = 0;

float a_m=0.0;				//输出值

float aim_pitch=0.3f;	//目标俯仰角：0°

float last_a_m,last_gyro;

float pid_control(float PITCH)
 {	 
	 if(gyro[1]-last_gyro>10000.0||gyro[1]-last_gyro<=-10000.0)
		 return last_a_m;
	 
		err_last_last=err_last;	//更新上上次的误差
		err_last=err;			//更新上次的误差
		err=(aim_pitch - Pitch);		//更新本次误差
	 
		
		off_cal=P*err-D*(float)gyro[1]; //位置式PID的公式

		a_m= off_cal; 			//得到输出值

		a_m = a_m<2000?a_m:2000;	//对输出值进行限幅
		a_m = a_m>-2000?a_m:-2000;
		last_a_m=a_m;
		last_gyro=gyro[1];
	 
	 
	 return a_m;				//输出a_m
 }
 
