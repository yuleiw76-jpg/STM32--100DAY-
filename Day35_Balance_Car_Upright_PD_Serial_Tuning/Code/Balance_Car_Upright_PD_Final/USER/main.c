#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "tim.h"
#include "encode.h"
#include "motor.h"
#include "mpu6050.h"

extern float Pitch;
extern float pidp;

static void Init_Failed(const char *stage,u8 code)
{
	printf("%s ERROR, CODE=%u\r\n",stage,(unsigned int)code);
	while(1)
	{
		PCout(13)=!PCout(13);
		delay_ms(200);
	}
}

int main(void)
{
	u8 mpu_status;
	u8 dmp_status;

	uart_init(115200);
	delay_init();
	LED_Init();
	IIC_Init();
	printf("WHO_AM_I=0x%02X\r\n",MPU6050_getDeviceID());

	mpu_status=MPU6050_initialize();     //=====MPU初始化并验证WHO_AM_I
	if(mpu_status!=MPU6050_INIT_OK)
		Init_Failed("MPU INIT",mpu_status);
	printf("MPU INIT OK\r\n");

	dmp_status=DMP_Init();
	if(dmp_status!=DMP_INIT_OK)
		Init_Failed("DMP INIT",dmp_status);
	printf("DMP INIT OK, 200HZ\r\n");

	TIM4_PWM_Init(3000-1,0); //PWM频率为24KHz
	
	delay_ms(500);
	
	TIM2_ENCODE_Init();	
	
	delay_ms(500);
	
	TIM1_Init(1000-1,72-1); //1ms一次定时器中断，多线程任务
	
	while(1)     
	{
		printf("DATA,pitch=%.3f,gyro=%d,pid=%.1f,ccr=%u/%u/%u/%u\r\n",
			Pitch,
			(int)gyro[1],
			pidp,
			(unsigned int)TIM4->CCR1,
			(unsigned int)TIM4->CCR2,
			(unsigned int)TIM4->CCR3,
			(unsigned int)TIM4->CCR4);
		delay_ms(50);
	}
}
