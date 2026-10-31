#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"	
#include "dma.h"

#define size_buf sizeof(buf)-1
u8 buf[]={"我就让你看一下DMA一次能发多少？！而且发出的期间你可以随意干别的~"};
u8 text[(size_buf+2)*100];

int main(void)
{	
	u32 i,t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	      
	LED_Init();
	KEY_Init();	 //按键初始化
	EXTIX_Init();
	uart_init(115200);
	DMA_init(DMA1_Channel4,(u32)&USART1->DR,(u32)text,(size_buf+2)*100);//DMA1通道4,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.	
	for(i=0;i<(size_buf+2)*100;i++)
	{
		if(t>=size_buf)//加入换行符
		{ 
			text[i++]=0x0d; 
			text[i]=0x0a; 
			t=0;
		}
		else text[i]=buf[t++];//复制TEXT_TO_SEND语句   
	}
	while(1)     
	{
		delay_ms(100);
		//printf("ok\r\n");
	}
}

