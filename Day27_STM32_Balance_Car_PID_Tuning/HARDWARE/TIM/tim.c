#include "tim.h"
#include "led.h"
#include "encode.h"
#include "pid.h"
#include "pid_v.h"
#include "motor.h"
#include "mpu6050.h"

//arr：自动重装值。
//psc：时钟预分频数

void TIM1_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 			//自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 		
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);        				//定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  				//TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//抢占占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  			//子优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 

	TIM_Cmd(TIM1, ENABLE);  										//使能TIM1外设
							 
}
float Pitch,Roll,Yaw;
float speed=0.0;
int last_count=0;
int last_CNT=0;


float pidp=0.0,pidv=0.0; 
u32 time_count=0;
u32 dmp_count=0;
u8 dmp_miss_count=0;
void TIM1_UP_IRQHandler(void)   //TIM1中断服务函数
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //中断为定时器更新中断 
		{
			time_count++;
			dmp_count++;
			if(time_count>=10)
			{
				speed=-(float)(circle_count1-last_count+TIM2->CNT-last_CNT)/1560.0/0.01;
				last_CNT=TIM2->CNT;
				last_count=circle_count1;		
				//pidv=pid_control_v(speed);gaile yuewang
				pidv=0.0f;
				time_count=0;
			}

			/* DMP输出频率为200Hz，每5ms读取和更新一次直立控制。 */
			if(dmp_count>=5)
			{
				dmp_count=0;
				if(Read_DMP(&Pitch,&Roll,&Yaw))
				{
					dmp_miss_count=0;
					pidp=pid_control(Pitch);
					//go((int)pidp,(int)pidv);gaile yuewang
					go((int)pidp,0);
					PCout(13)=!PCout(13);
				}
				else
				{
					if(dmp_miss_count<10)
						dmp_miss_count++;
					if(dmp_miss_count>=10)
						go(0,0); //连续50ms无有效姿态数据时停止电机
				}
			}
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除标志位 
		}
}
