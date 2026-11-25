#include "led.h"
#include "delay.h"
#include "sys.h"
#include "FOC.h"
#include "FOC_utils.h"
#include "encoder.h"
#include "motor.h"

float angle_now=0,angle_ori=0;

int main(void)
{	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	FOC_init();
	IIC_Init();
	MotorAngle_Init();
	
	while(1)
	{
		angle_now=getAngle()-angle_ori;
		FOC_SVPWM(2, 0, angle_now);//2极限1.5正好
	}
}

