#include "encoder.h"
#include "delay.h"
#include "math.h"
//#define SDA_IN()  {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=(u32)8<<20;} //SDA切换为输入模式 
//#define SDA_OUT() {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=(u32)3<<20;} //SDA切换为输出模式 
#define SDA_IN()  { GPIOA->CRL &= 0x0FFFFFFFU; GPIOA->CRL |= (8U << 28); }
#define SDA_OUT() { GPIOA->CRL &= 0x0FFFFFFFU; GPIOA->CRL |= (3U << 28); }
 
 
//IO操作函数	 
#define IIC_SCL    PAout(6) //SCL输出
#define IIC_SDA    PAout(7) //SDA输出	 
#define READ_SDA   PAin(7)  //SDA输入
 
u16 raw_num = 0;
u16 JIAODUnum = 0;

void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//ê1?üGPIOBê±?ó
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //í?íìê?3?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_7); 	//
}
//2úéúIIC?eê?D?o?
void IIC_Start_c(void)
{
	SDA_OUT();     //sda??ê?3?
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//?ˉ×?I2C×ü??￡?×?±?·￠?í?ò?óê?êy?Y 
}	  
//2úéúIICí￡?1D?o?
void IIC_Stop_c(void)
{
	SDA_OUT();//sda??ê?3?
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//·￠?íI2C×ü???áê?D?o?
	delay_us(4);
}
//μè′yó|′eD?o?μ?à′
//·μ???μ￡o1￡??óê?ó|′eê§°ü
//        0￡??óê?ó|′e3é1|
u8 IIC_Wait_Ack_c(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDAéè???aê?è?  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_c();
			return 1;
		}
	}
	IIC_SCL=0;//ê±?óê?3?0 	   
	return 0;  
} 
//2úéúACKó|′e
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//2?2úéúACKó|′e		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC·￠?íò???×??ú
//·μ??′ó?úóD?Tó|′e
//1￡?óDó|′e
//0￡??Tó|′e			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//à-μíê±?ó?aê?êy?Y′?ê?
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //??TEA5767?aèy???óê±??ê?±?D?μ?
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//?á1??×??ú￡?ack=1ê±￡?·￠?íACK￡?ack=0￡?·￠?ínACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDAéè???aê?è?
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//·￠?ínACK
    else
        IIC_Ack(); //·￠?íACK   
    return receive;
}
 
//?úAS5600???¨μ??·?á3?ò???êy?Y
u8 AS5600_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=-1;		  	    																 
  IIC_Start_c();  
	IIC_Send_Byte((0X36<<1)|0x00);	   //·￠?íD′?üá?
	IIC_Wait_Ack_c(); 
  IIC_Send_Byte(ReadAddr);   //·￠?íμ??·
	IIC_Wait_Ack_c();	    
	IIC_Start_c();  	 	   
	IIC_Send_Byte((0X36<<1)|0x01);           //??è??óê??￡ê?			   
	IIC_Wait_Ack_c();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop_c();//2úéúò???í￡?1ì??t	    
	return temp;
}
 
//?úAS5600???¨μ??··￠?í3?ò???êy?Y
void AS5600_WriteOneByte(u16 WriteAddr,u8 WriteData)
{				  	  	    																 
  IIC_Start_c();  
	IIC_Send_Byte((0X36<<1)|0x00);	   //·￠?íD′?üá?
	IIC_Wait_Ack_c(); 
  IIC_Send_Byte(WriteAddr);   	//·￠?íμ??·
	IIC_Wait_Ack_c();	    
	IIC_Start_c();  	 	   
	IIC_Send_Byte(WriteData);          //·￠?íêy?Y		   
	IIC_Wait_Ack_c();	 	   
  IIC_Stop_c();//2úéúò???í￡?1ì??t	    
	delay_ms(10);
}
 
//?áè?á???êy?Y
u16 AS5600_ReadTwoByte(u16 ReadAddr_hi,u16 ReadAddr_lo)
{
	u16 TwoByte_Data=-1;
	u8 hi_Data=0,lo_Data=0;
	hi_Data=AS5600_ReadOneByte(ReadAddr_hi);
	lo_Data=AS5600_ReadOneByte(ReadAddr_lo);
	TwoByte_Data = (hi_Data<<8)|lo_Data;
	return TwoByte_Data;
}


float angle_prev=0,full_rotation_offset=0;

float getAngle(void)
{
	float angle,d_angle;
	
	angle = AS5600_ReadTwoByte(_raw_ang_hi,_raw_ang_lo);
	d_angle = angle - angle_prev;
	if(fabs(d_angle) > (0.8*4096) ) full_rotation_offset += d_angle > 0 ? -_2PI : _2PI;
	angle_prev = angle;

	return  ((full_rotation_offset + ( angle * 1.0 / 4096.0) * _2PI)*7) ;
}

