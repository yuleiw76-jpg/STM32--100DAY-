#include "track.h"

void track_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 

	//A4口，循迹OUT1口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//A5口，循迹OUT2口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//A6口，循迹OUT3口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//A7口，循迹OUT4口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//B0口，循迹OUT5口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
int track_error=0;
void track_scan()
{
	if	   (OUT1==0 && OUT2==1 && OUT3==1 && OUT4==1 && OUT5==1) track_error=-2;	 //01111
	else if(OUT1==1 && OUT2==0 && OUT3==1 && OUT4==1 && OUT5==1) track_error=-1; 	 //10111
	else if(OUT1==1 && OUT2==1 && OUT3==0 && OUT4==1 && OUT5==1) track_error= 0; 	 //11011
	else if(OUT1==1 && OUT2==1 && OUT3==1 && OUT4==0 && OUT5==1) track_error= 1; 	 //11101
	else if(OUT1==1 && OUT2==1 && OUT3==1 && OUT4==1 && OUT5==0) track_error= 2; 	 //11110
	else
	return;

}

void servo_control(int t_error)
{
	TIM_SetCompare1(TIM1,1500+500*t_error);
}

