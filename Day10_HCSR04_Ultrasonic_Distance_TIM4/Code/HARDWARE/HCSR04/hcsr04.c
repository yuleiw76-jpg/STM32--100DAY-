#include "hcsr04.h"
#include "delay.h"

//测距
void HC_SR04_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	
}

extern int overcount;
float Senor_Using(void)
{
	float length = 0,sum = 0;
	u16 tim;
	unsigned int i = 0;
	//测五次数据计算一次平均值
	while(i != 5)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_11); //拉高信号，作为触发信号
		delay_us(20); //高电平信号超过10us
		GPIO_ResetBits(GPIOA,GPIO_Pin_11); //等待回响信号
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == RESET);
		//回响信号到来，开启定时器计数
		overcount = 0;
		TIM4->CNT = 0;
		TIM_Cmd(TIM4,ENABLE);
		i += 1; //每收到一次回响信号+1,收到5次就计算均值
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == SET);//回响信号消失
		TIM_Cmd(TIM4,DISABLE);//关闭定时器
		tim = TIM_GetCounter(TIM4); //获取计TIM2数寄存器中的计数值
		length = 100*(tim+overcount*63336)*(1.0/72000000.0)*340.0/2; //通过回响信号计算距离
		sum = length+sum;
		TIM4->CNT = 0; //将TIM2计数寄存器的计数值清零
		overcount = 0; //中断溢出次数清零
		delay_ms(100);
	}
	length = sum/5;
	return length; //距离作为函数返回值
}
