#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "oled.h"
#include "stdio.h"

int main(void)
{	
	u8 a;
	int your_money=100;
	char text[20];
	delay_init();
	LED_Init();
	KEY_Init();
	OLED_Init();
	OLED_Clear();	//«Â∆¡
	while(1)     
	{
		delay_ms(10);
		a=KEY_Scan(0);
		
		if(a==KEY1_PRESS)
		{
			OLED_Clear();//«Â∆¡
			sprintf(text,"you have %d yuan!",your_money);
			OLED_ShowString(8,2,(u8 *)text,8);
		}
		else if(a==KEY2_PRESS)
		{
			OLED_Clear();//«Â∆¡
			OLED_ShowString(8,0,(u8 *)"Hello!",16);
			OLED_ShowString(8,2,(u8 *)"This is small size.",8);
			OLED_ShowString(8,3,(u8 *)"HAHAHA!!!",8);
			OLED_ShowString(8,4,(u8 *)"The big size.",16);
			OLED_ShowString(8,6,(u8 *)"last row",16);
		}
	}
}
