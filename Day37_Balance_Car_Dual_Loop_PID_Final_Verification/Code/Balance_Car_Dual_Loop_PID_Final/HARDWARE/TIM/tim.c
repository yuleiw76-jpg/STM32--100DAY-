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
volatile u32 enc_a_edges=0;
volatile u32 enc_b_edges=0;
volatile u8 enc_a_level=0;
volatile u8 enc_b_level=0;
static u8 enc_a_last=0;
static u8 enc_b_last=0;
void TIM1_UP_IRQHandler(void)   //TIM1中断服务函数
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //中断为定时器更新中断 
		{
			/* 1ms采样编码器输入，用于不增加外部接线的诊断。 */
			enc_a_level=(u8)GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
			enc_b_level=(u8)GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
			if(enc_a_level!=enc_a_last)
			{
				enc_a_edges++;
				enc_a_last=enc_a_level;
			}
			if(enc_b_level!=enc_b_last)
			{
				enc_b_edges++;
				enc_b_last=enc_b_level;
			}

			time_count++;
			dmp_count++;
			if(time_count>=10)
			{
				/* 将整圈数先换算成编码器计数，避免CNT跨零时产生约100r/s的假速度。 */
				speed=-(float)(((circle_count1-last_count)*1560)+((int)TIM2->CNT-last_CNT))/1560.0f/0.01f;
				last_CNT=TIM2->CNT;
				last_count=circle_count1;		
				pidv=pid_control_v(speed); //开启速度环计算
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
					go((int)-pidp,(int)pidv);
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
