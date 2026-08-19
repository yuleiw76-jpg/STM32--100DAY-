#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "tim.h"
#include "encode.h"
#include "motor.h"
#include "mpu6050.h"
#include "pid.h"
#include "usart.h"

static u8 String_Equals(const char *left,const char *right)
{
	while((*left!='\0')&&(*right!='\0'))
	{
		if(*left!=*right)
			return 0;
		left++;
		right++;
	}
	return ((*left=='\0')&&(*right=='\0'))?1:0;
}

static u8 Parse_Float(const char *text,float *value)
{
	float result=0.0f;
	float decimal=0.1f;
	float sign=1.0f;
	u8 digit_seen=0;

	if(*text=='-')
	{
		sign=-1.0f;
		text++;
	}
	else if(*text=='+')
	{
		text++;
	}

	while((*text>='0')&&(*text<='9'))
	{
		digit_seen=1;
		result=result*10.0f+(float)(*text-'0');
		text++;
	}

	if(*text=='.')
	{
		text++;
		while((*text>='0')&&(*text<='9'))
		{
			digit_seen=1;
			result+=(float)(*text-'0')*decimal;
			decimal*=0.1f;
			text++;
		}
	}

	if((!digit_seen)||(*text!='\0'))
		return 0;

	*value=result*sign;
	return 1;
}

static void Print_Tuning_Help(void)
{
	printf("CMD: P=0..2000, D=0..20, A=-20..20, ZERO, RUN, STOP, STATUS, HELP\r\n");
}

static void Process_Tuning_Command(void)
{
	char command[32];
	u16 length;
	u16 index;
	float value;

	if((USART_RX_STA&0x8000)==0)
		return;

	length=USART_RX_STA&0x3FFF;
	if(length>(sizeof(command)-1))
		length=sizeof(command)-1;
	for(index=0;index<length;index++)
		command[index]=(char)USART_RX_BUF[index];
	command[length]='\0';
	USART_RX_STA=0;

	if(String_Equals(command,"ZERO"))
	{
		Balance_Control_SetEnabled(0);
		aim_pitch=Pitch;
		printf("ACK,ZERO,A=%.2f,MOTOR_DISABLED\r\n",aim_pitch);
	}
	else if(String_Equals(command,"STOP"))
	{
		Balance_Control_SetEnabled(0);
		printf("ACK,STOP\r\n");
	}
	else if(String_Equals(command,"RUN"))
	{
		if(attitude_valid&&(Pitch<(aim_pitch+10.0f))&&(Pitch>(aim_pitch-10.0f))&&(dmp_miss_count==0))
		{
			Balance_Control_SetEnabled(1);
			printf("ACK,RUN\r\n");
		}
		else
		{
			Balance_Control_SetEnabled(0);
			printf("ERR,RUN_REJECTED,pitch=%.2f,aim=%.2f,valid=%u,miss=%u\r\n",Pitch,aim_pitch,attitude_valid,dmp_miss_count);
		}
	}
	else if(String_Equals(command,"STATUS"))
	{
		printf("STATUS,P=%.2f,D=%.3f,A=%.2f,RUN=%u\r\n",P,D,aim_pitch,motor_control_enabled);
	}
	else if(String_Equals(command,"HELP"))
	{
		Print_Tuning_Help();
	}
	else if((command[0]=='P')&&(command[1]=='=')&&Parse_Float(&command[2],&value))
	{
		if((value>=0.0f)&&(value<=2000.0f))
		{
			Balance_Control_SetEnabled(0);
			P=value;
			printf("ACK,P=%.2f,MOTOR_DISABLED\r\n",P);
		}
		else printf("ERR,P_RANGE\r\n");
	}
	else if((command[0]=='D')&&(command[1]=='=')&&Parse_Float(&command[2],&value))
	{
		if((value>=0.0f)&&(value<=20.0f))
		{
			Balance_Control_SetEnabled(0);
			D=value;
			printf("ACK,D=%.3f,MOTOR_DISABLED\r\n",D);
		}
		else printf("ERR,D_RANGE\r\n");
	}
	else if((command[0]=='A')&&(command[1]=='=')&&Parse_Float(&command[2],&value))
	{
		if((value>=-20.0f)&&(value<=20.0f))
		{
			Balance_Control_SetEnabled(0);
			aim_pitch=value;
			printf("ACK,A=%.2f,MOTOR_DISABLED\r\n",aim_pitch);
		}
		else printf("ERR,A_RANGE\r\n");
	}
	else
	{
		printf("ERR,UNKNOWN_CMD,%s\r\n",command);
		Print_Tuning_Help();
	}
}

static void Init_Failed(const char *stage,u8 code)
{
	printf("%s ERROR, CODE=%u\r\n",stage,(unsigned int)code);
	while(1)
	{
		PCout(13)=!PCout(13);
		delay_ms(200);
	}
}

int main(void)
{
	u8 mpu_status;
	u8 dmp_status;

	uart_init(115200);
	delay_init();
	LED_Init();
	IIC_Init();
	printf("WHO_AM_I=0x%02X\r\n",MPU6050_getDeviceID());

	mpu_status=MPU6050_initialize();     //=====MPU初始化并验证WHO_AM_I
	if(mpu_status!=MPU6050_INIT_OK)
		Init_Failed("MPU INIT",mpu_status);
	printf("MPU INIT OK\r\n");

	dmp_status=DMP_Init();
	if(dmp_status!=DMP_INIT_OK)
		Init_Failed("DMP INIT",dmp_status);
	printf("DMP INIT OK, 200HZ\r\n");

	TIM4_PWM_Init(3000-1,0); //PWM频率为24KHz
	
	delay_ms(500);
	
	TIM2_ENCODE_Init();	
	
	delay_ms(500);
	
	TIM1_Init(1000-1,72-1); //1ms一次定时器中断，多线程任务
	printf("TUNING READY, MOTOR DISABLED\r\n");
	Print_Tuning_Help();
	
	while(1)     
	{
		static u32 last_telemetry_ms=0;
		Process_Tuning_Command();
		if((control_tick_ms-last_telemetry_ms)>=50)
		{
			last_telemetry_ms=control_tick_ms;
			printf("DATA,t=%lu,pitch=%.3f,gyro=%d,pid=%.1f,ccr=%u/%u/%u/%u,P=%.2f,D=%.3f,A=%.2f,run=%u,valid=%u,miss=%u\r\n",
				(unsigned long)control_tick_ms,Pitch,(int)gyro[1],pidp,
				(unsigned int)TIM4->CCR1,(unsigned int)TIM4->CCR2,
				(unsigned int)TIM4->CCR3,(unsigned int)TIM4->CCR4,
				P,D,aim_pitch,motor_control_enabled,attitude_valid,dmp_miss_count);
		}
		delay_ms(1);
	}
}
