#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取按键2
 

#define KEY1_PRESS	1		//KEY1  
#define KEY2_PRESS	2		//KEY2  

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
