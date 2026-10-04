#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"



int main(void)
{	
	u8 a;
	u16 i;
	char text[20];
	delay_init();	      
	LED_Init();
	KEY_Init();	 //按键初始化
	uart_init(9600);
	OLED_Init();

	while(1)     
	{
		a=KEY_Scan(0);
		if(a==1)
		{
			LED=0;
			printf("Hello!\r\n");
			printf("%d\r\n",a);
			OLED_Clear();
			OLED_ShowString(0,2,(u8*)"Sessage sent.",16);
		}
		
		else if(a==2)
		{
				for(i=0;i<1000;i++)
				{
					printf("(%d)\r\n",i);
				}
				
				for(i=1000;i>0;i--)
				{
					printf("(%d)\r\n",i);
				}
		}
			
		
		if((USART_RX_STA>>15)==1)
		{
			strcpy(text,(char*)USART_RX_BUF);
			LED=!LED;			//信号

			OLED_Clear();
			OLED_ShowString(0,0,(u8*)text,16);
			
			USART_RX_STA=0;
			memset(&text,0,sizeof(text));
			memset(&USART_RX_BUF,0,sizeof(USART_RX_BUF));
		}
	}
}

