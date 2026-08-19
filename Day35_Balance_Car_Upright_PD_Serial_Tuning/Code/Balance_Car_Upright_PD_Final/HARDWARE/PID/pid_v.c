#include "pid_v.h"
#include "tim.h"

static float err_last_last1=0;	//上上次的误差
static float err_last1=0;  		//上次的误差
float err1=0;			//本次的误差
static float sum_err1=0;			//本次的误差

#define V_P -900
#define SUM_MAX 90000


static float P=V_P;			//参数P
static float I=V_P/200.0; 		//参数I
static float D=0; 		//参数I
 
static float off_cal1 = 0;

float a_m1=0.0;				//输出值

float aim_v=0;


float pid_control_v(float speed)
{	 
	if(speed>9||speed<=-9)
		return a_m1;
	err_last_last1=err_last1;	//更新上上次的误差
	err_last1=err1;			//更新上次的误差
	err1=(aim_v - speed);		//更新本次误差
	sum_err1+=err1;
	sum_err1 = sum_err1<SUM_MAX?sum_err1:SUM_MAX;
	sum_err1 = sum_err1>-SUM_MAX?sum_err1:-SUM_MAX;
	
	off_cal1=P*err1+I*sum_err1+D*(err1-err_last1); //位置式PID的公式

	a_m1 = off_cal1; 			//得到输出值

	a_m1 = a_m1<2000?a_m1:2000;	//对输出值进行限幅
	a_m1 = a_m1>-2000?a_m1:-2000;
 
 return a_m1;				//输出a_m
}

 