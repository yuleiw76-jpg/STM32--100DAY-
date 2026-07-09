#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "key.h"
#include "servo.h"

int main(void)
{	
	u8 a;
	delay_init();
	LED_Init();
	KEY_Init();
	TIM1_PWM_Init(20000-1,72-1); //舵机的arr值为500-2500
	Servo_SetAngle(90);
  delay_ms(1000);
	Servo_SetAngle(0);
    delay_ms(1000);
   Servo_SetAngle(45);
   delay_ms(1000);
   Servo_SetAngle(90);
   delay_ms(1000);
   Servo_SetAngle(135);
   delay_ms(1000);
   Servo_SetAngle(180);
   delay_ms(1000);
  u16 angle = 90;
	Servo_SetAngle(angle);
	while(1)     
	{
		a=KEY_Scan(0);
		if(a==KEY1_PRESS)
		{
			if(angle>=15)
			{
				angle-=15;
			}
			Servo_SetAngle(angle);
		}
		else if(a==KEY2_PRESS)
		{
			if(angle<=165)
			{
				angle+=15;
			}
			Servo_SetAngle(angle);
		}
    delay_ms(20);
   
		
	}
}
