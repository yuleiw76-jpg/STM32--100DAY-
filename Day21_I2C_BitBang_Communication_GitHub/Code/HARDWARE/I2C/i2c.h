#ifndef __I2C_H
#define __I2C_H			  	 
#include "sys.h"    	
#include "delay.h"
			  
#define SDA_IN()  {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=(u32)8<<20;}//上/下拉输入
#define SDA_OUT() {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=(u32)3<<20;}//推挽输出

#define SCL PAout(4)
#define SDA PAout(5)
#define READ_SDA PAin(5)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NAck(void);
u8 IIC_Wait_Ack(void);
void IIC_Send_Byte(unsigned char IIC_Byte);  
u8 IIC_Read_Byte(unsigned char ack);

#endif  
	 
