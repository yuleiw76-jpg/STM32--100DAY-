#include "sys.h"
#include "delay.h"
#include "mpu6050.h"
#include "oled.h"
#include "IIC.h"

float Pitch,Roll,Yaw;

static void OLED_ShowBoot(u8 *line1,u8 *line2)
{
	OLED_Clear();
	OLED_ShowString(0,0,line1,16);
	OLED_ShowString(0,16,line2,16);
	OLED_Refresh();
}

static u8 HexDigit(u8 value)
{
	value&=0x0F;
	return (value<10)?(u8)('0'+value):(u8)('A'+value-10);
}

static void OLED_ShowWhoID(u8 id)
{
	u8 text[]="READ ID 0x00";

	text[10]=HexDigit(id>>4);
	text[11]=HexDigit(id);
	OLED_ShowBoot((u8 *)"WHO ERROR",text);
}

static void OLED_ShowDmpWaiting(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,(u8 *)"Pitch: --.-",16);
	OLED_ShowString(0,16,(u8 *)"Roll : --.-",16);
	OLED_ShowString(0,32,(u8 *)"Yaw  : --.-",16);
	OLED_ShowString(0,48,(u8 *)"WAIT DMP FIFO   ",16);
	OLED_Refresh();
}

static void OLED_ShowDmpDiag(void)
{
	u8 fifo[2];
	u8 user_ctrl;
	u8 int_enable;
	u16 fifo_count;
	u8 text[]="F:000 U:00 I:00 ";

	if(i2cRead(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_FIFO_COUNTH,2,fifo) ||
		i2cRead(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_USER_CTRL,1,&user_ctrl) ||
		i2cRead(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_INT_ENABLE,1,&int_enable))
	{
		OLED_ShowString(0,48,(u8 *)"FIFO IIC ERROR  ",16);
		OLED_Refresh();
		return;
	}

	fifo_count=((u16)fifo[0]<<8)|fifo[1];
	text[2]=HexDigit((u8)(fifo_count>>8));
	text[3]=HexDigit((u8)(fifo_count>>4));
	text[4]=HexDigit((u8)fifo_count);
	text[8]=HexDigit(user_ctrl>>4);
	text[9]=HexDigit(user_ctrl);
	text[13]=HexDigit(int_enable>>4);
	text[14]=HexDigit(int_enable);
	OLED_ShowString(0,48,text,16);
	OLED_Refresh();
}

static u8 *DMP_ErrorText(u8 status)
{
	switch(status)
	{
		case DMP_INIT_IIC_ERROR:         return (u8 *)"IIC ERR";
		case DMP_INIT_WHO_ERROR:         return (u8 *)"WHO ERR";
		case DMP_INIT_DRIVER_ERROR:      return (u8 *)"MPU INIT ERR";
		case DMP_INIT_SENSOR_ERROR:      return (u8 *)"SENSOR ERR";
		case DMP_INIT_FIFO_ERROR:        return (u8 *)"FIFO ERR";
		case DMP_INIT_RATE_ERROR:        return (u8 *)"RATE ERR";
		case DMP_INIT_FIRMWARE_ERROR:    return (u8 *)"FW ERR";
		case DMP_INIT_ORIENTATION_ERROR: return (u8 *)"ORIENT ERR";
		case DMP_INIT_FEATURE_ERROR:     return (u8 *)"FEATURE ERR";
		case DMP_INIT_DMP_RATE_ERROR:    return (u8 *)"DMP RATE ERR";
		case DMP_INIT_STATE_ERROR:       return (u8 *)"DMP STATE ERR";
		default:                         return (u8 *)"UNKNOWN ERR";
	}
}

static void OLED_Halt(u8 *title,u8 *reason)
{
	OLED_ShowBoot(title,reason);
	while(1)
	{
		delay_ms(100);
	}
}

static void OLED_ShowAngle(u8 y,u8 *label,float angle)
{
	long value10;
	u32 magnitude;

	value10=(long)(angle*10.0f);
	if(value10>1999)value10=1999;
	if(value10<-1999)value10=-1999;
	magnitude=(value10<0)?(u32)(-value10):(u32)value10;

	OLED_ShowString(0,y,label,16);
	OLED_ShowChar(48,y,(value10<0)?'-':'+',16);
	OLED_ShowChar(56,y,(magnitude>=1000)?(u8)('0'+(magnitude/1000)%10):' ',16);
	OLED_ShowChar(64,y,(magnitude>=100)?(u8)('0'+(magnitude/100)%10):' ',16);
	OLED_ShowChar(72,y,(u8)('0'+(magnitude/10)%10),16);
	OLED_ShowChar(80,y,'.',16);
	OLED_ShowChar(88,y,(u8)('0'+magnitude%10),16);
}

int main(void)
{
	u8 status;
	u8 no_data_count;

	delay_init();
	OLED_Init();
	/* 上电自检：通信正常时OLED会全屏点亮约1秒。 */
	OLED_WR_Byte(0xA5,OLED_CMD);
	delay_ms(1000);
	OLED_WR_Byte(0xA4,OLED_CMD);
	OLED_ShowBoot((u8 *)"OLED READY",(u8 *)"PB6 SCL PB7 SDA");
	delay_ms(500);

	do
	{
		IIC_Init();
		delay_ms(20);
		status=MPU6050_initialize();
		if(status==MPU6050_INIT_IIC_ERROR)
			OLED_ShowBoot((u8 *)"MPU ERROR",(u8 *)"IIC ERR RETRY");
		else if(status==MPU6050_INIT_WHO_ERROR)
			OLED_ShowWhoID(MPU6050_getDeviceID());
		if(status!=MPU6050_INIT_OK)
			delay_ms(500);
	}while(status!=MPU6050_INIT_OK);
	OLED_ShowBoot((u8 *)"MPU6500 OK",(u8 *)"WHO OK 0x70");
	delay_ms(500);

	OLED_ShowBoot((u8 *)"DMP START",(u8 *)"PLEASE WAIT");
	status=DMP_Init();
	if(status!=DMP_INIT_OK)
		OLED_Halt((u8 *)"DMP ERROR",DMP_ErrorText(status));
	OLED_ShowBoot((u8 *)"DMP OK",(u8 *)"20 Hz READY");
	delay_ms(500);

	OLED_ShowDmpWaiting();
	no_data_count=0;
	while(1)
	{
		if(Read_DMP(&Pitch,&Roll,&Yaw))
		{
			OLED_ShowAngle(0,(u8 *)"Pitch:",Pitch);
			OLED_ShowAngle(16,(u8 *)"Roll :",Roll);
			OLED_ShowAngle(32,(u8 *)"Yaw  :",Yaw);
			OLED_ShowString(0,48,(u8 *)"DMP 20Hz OK     ",16);
			OLED_Refresh();
			no_data_count=0;
		}
		else if(++no_data_count>=50)
		{
			OLED_ShowDmpDiag();
			no_data_count=0;
		}
		delay_ms(5);
	}
}

