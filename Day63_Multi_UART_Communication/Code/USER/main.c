#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"



int main(void)
{	
	u32 a;
	char text[20];
	delay_init();	      
	LED_Init();
	uart_init(115200);
	uart2_init(115200);
	uart3_init(115200);

	while(1)     
	{
		printf("Hello!%u\r\n",a);
		a++;
		delay_us(200);
		
		if((USART1_RX_STA>>15)==1)
		{
			strcpy(text,(char*)USART1_RX_BUF);
			strcat(text,"\r\n");
			LED=!LED;			//пе╨е			
			UART2_Send_String(text);
			USART1_RX_STA=0;
			memset(&text,0,sizeof(text));
			memset(&USART1_RX_BUF,0,sizeof(USART1_RX_BUF));
		}
	}
}

