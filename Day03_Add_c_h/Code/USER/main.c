#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"


int main(void)
{	
	u8 a;
	delay_init();	      
	LED_Init();
	KEY_Init();	 //°´¼ü³õÊ¼»¯

	while(1)     
	{
		a=KEY_Scan(1);
		if(a==1)
		{
			LED=0;
		}
			
		else if(a==2)
		{
			LED=1;
		}
	}
}
