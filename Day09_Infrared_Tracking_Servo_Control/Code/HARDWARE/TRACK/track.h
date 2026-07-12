#ifndef __TRACK_H
#define __TRACK_H	 
#include "sys.h"

#define OUT1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define OUT2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define OUT3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define OUT4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define OUT5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

void track_Init(void);
void track_scan(void);
void servo_control(int t_error);
		 				    
#endif
