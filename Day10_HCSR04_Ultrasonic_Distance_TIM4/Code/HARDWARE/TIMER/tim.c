#include "tim.h"
#include "led.h"

//arr：自动重装值。
//psc：时钟预分频数

void TIM4_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 			//自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 		
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);        				//定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  				//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//抢占占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  			//子优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 

	TIM_Cmd(TIM4, ENABLE);  										//使能TIM4外设
							 
}

int overcount=0;
void TIM4_IRQHandler(void)   //TIM4中断服务函数
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //中断为定时器更新中断 
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除标志位 
			overcount++;
		}
}