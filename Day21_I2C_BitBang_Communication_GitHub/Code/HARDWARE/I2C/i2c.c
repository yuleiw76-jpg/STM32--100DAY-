#include "i2c.h"

void IIC_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC,D,G端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	 //PB5,PB6推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6

    delay_ms(100); //这里的延时很重要
	SCL=1;
	SDA=1;
}

void IIC_Start(void)
{
	SDA_OUT();
	SDA=1;
	SCL=1;
	delay_us(2);
	SDA=0;
	delay_us(2);
	SCL=0;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
	SDA_OUT();
	SCL=0;
	SDA=0;
	delay_us(2);
	SCL=1;
	SDA=1;
	delay_us(2);
}

//以下函数SCL最后都是0
void IIC_Ack(void)
{
	SCL=0;   //先拉低SCL，使SDA可改变
	SDA_OUT();
	SDA=0;   
	delay_us(2);
	SCL=1;
	delay_us(2);
	SCL=0;
}
void IIC_NAck(void)
{
	SCL=0;   //先拉低SCL，使SDA可改变
	SDA_OUT();
	SDA=1;   //拉高SDA，不产生应答信号
	delay_us(2);
	SCL=1;
	delay_us(2);
	SCL=0;
}



u8 IIC_Wait_Ack(void)
{
	u8 Ack_Error_Time=0;
	SDA_IN();//原来代码没加
	SDA=1;	   
	delay_us(2);
	SCL=1;
	delay_us(2);
	while(READ_SDA)
	{
		Ack_Error_Time++;
		if(Ack_Error_Time>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	//delay_us(3);
	SCL=0;//时钟输出0 
	return 0;  
}

void IIC_Send_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m, da;
	SDA_OUT();
    da = IIC_Byte;
    SCL=0;

    for(i = 0; i < 8; i++)
    {
        m = da;
        m = m & 0x80;

        if(m == 0x80)
        {
            SDA=1;
        }
        else SDA=0;

        da = da << 1;
        SCL=1;	//拉高一次，采完样就低回去。
		delay_us(2);
        SCL=0;
		delay_us(2);
    }
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();        //SDA设置为输入
    for(i=0;i<8;i++)
	{
        SCL=0;
        delay_us(2);
		SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(2); 
    }					 
    if (!ack)				//有点绕
        IIC_NAck();        //发送nACK
    else
        IIC_Ack();         //发送ACK   
    return receive;
}
