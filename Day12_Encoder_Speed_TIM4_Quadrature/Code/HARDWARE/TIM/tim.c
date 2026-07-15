#include "tim.h"
#include "led.h"

//arr：自动重装值。
//psc：时钟预分频数

void TIM3_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 			//自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 		
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);        				//定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//抢占占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  			//子优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 

	TIM_Cmd(TIM3, ENABLE);  										//使能TIM3外设
							 
}

extern int circle_count;
float speed;
int last_count=0;
int last_num=0;

void TIM3_IRQHandler(void)   //TIM3中断服务函数
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //中断为定时器更新中断 
		{
			speed=((float)((circle_count-last_count)*1560+((int)(TIM4->CNT)-last_num)))/1560.0/0.1;
			last_count=circle_count;
			last_num=TIM4->CNT;
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除标志位 
		}
}