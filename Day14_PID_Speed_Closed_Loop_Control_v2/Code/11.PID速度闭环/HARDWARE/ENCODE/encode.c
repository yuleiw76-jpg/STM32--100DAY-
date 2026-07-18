#include "encode.h"

void TIM4_ENCODE_Init()
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  
																		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //TIM4_CH1、TIM4_CH2两路浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	
	TIM_TimeBaseStructure.TIM_Period = 1560-1; //arr，与编码器线数、减速器减速比有关
	TIM_TimeBaseStructure.TIM_Prescaler =0; //psc不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	/*TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);*/

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	
 	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
   
   	TIM_SetCounter(TIM4,0); //计数器清零
	TIM_Cmd(TIM4, ENABLE);  //使能TIM1
}

int circle_count=0;
void TIM4_IRQHandler(void)
{

if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{ 
		if((TIM4->CR1>>4&0x01)==0)   //寄存器的DIR位，判断向下计数还是向上计数
			circle_count++;
		else if((TIM4->CR1>>4&0x01)==1)
			circle_count--;
	
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
	}
}
