#include "led.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "stdio.h"
#include "i2c.h"

u8 a =-1;
char aa =-1;
int main(void)
{	
	delay_init();
	LED_Init();
	OLED_Init();
	OLED_Clear();	//«Â∆¡
	IIC_Init();
	a=0;
	

	while(1)     
	{
		delay_ms(10);a--;aa--;
	}
}
