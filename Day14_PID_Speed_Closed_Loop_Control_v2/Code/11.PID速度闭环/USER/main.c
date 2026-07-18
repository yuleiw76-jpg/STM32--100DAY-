#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "key.h"
#include "tim.h"
#include "encode.h"
#include "oled.h"
#include "stdio.h"


extern float speed;
extern float aim_v;
extern int circle_count;
int total_count;

int main(void)
{	
	char text[20];
	delay_init();
	LED_Init();
	TIM1_PWM_Init(3000-1,0); //PWM频率为24KHz
	TIM4_ENCODE_Init();		
	TIM3_Init(10000-1,720-1); //100ms一次定时器中断，用于计算电机转速
	TIM2_Init(10000-1,720-1); //100ms一次定时器中断，用于PID计算
	OLED_Init();
	OLED_Clear();
	//TIM_SetCompare1(TIM1, 800);
  //TIM_SetCompare2(TIM1, 0);
	

	
	while(1)     
	{
	u8 a_level;
  u8 b_level;

while (1)
{
    sprintf(text, "now:%.2f    ", speed);
    OLED_ShowString(8, 4, (u8 *)text, 16);

    delay_ms(50);
}
		
	}
}
