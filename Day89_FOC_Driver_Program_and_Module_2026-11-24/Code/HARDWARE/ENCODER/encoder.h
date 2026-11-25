#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#include "FOC.h"
#include "FOC_conf.h"
#include "FOC_utils.h"

#define	_raw_ang_hi 0x0c
#define	_raw_ang_lo 0x0d
 
extern u16 raw_num;
extern u16 JIAODUnum;
//IIC?¨´¨®D2¨´¡Á¡Âo¡¥¨ºy
void IIC_Init(void);                //3?¨º??¡¥IIC¦Ì?IO?¨²				 
void IIC_Start_c(void);				//¡¤¡é?¨ªIIC?a¨º?D?o?
void IIC_Stop_c(void);	  			//¡¤¡é?¨ªIIC¨ª¡ê?1D?o?
void IIC_Send_Byte(u8 txd);			//IIC¡¤¡é?¨ª¨°???¡Á??¨²
u8 IIC_Read_Byte(unsigned char ack);//IIC?¨¢¨¨?¨°???¡Á??¨²
u8 IIC_Wait_Ack_c(void); 				//IIC¦Ì¨¨¡äyACKD?o?
void IIC_Ack(void);					//IIC¡¤¡é?¨ªACKD?o?
void IIC_NAck(void);				//IIC2?¡¤¡é?¨ªACKD?o?
 
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
 
float getAngle(void);
u8 AS5600_ReadOneByte(u16 ReadAddr);
void AS5600_WriteOneByte(u16 WriteAddr,u8 WriteData);
u16 AS5600_ReadTwoByte(u16 ReadAddr_hi,u16 ReadAddr_lo);

 
#endif
