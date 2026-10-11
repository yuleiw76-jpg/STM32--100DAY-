#include "lora.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

u8* LORA_CMD(u8 *cmd, u8 *ack)
{
    char *strx = 0;
    printf("%s\r\n", cmd);

    while(1)
    {
        if(USART_RX_STA&0x8000)		//接收到一次数据了
        {
			strx = strstr((const char*)USART_RX_BUF, (const char*)ack);//判断ack是否是USART1BUF的子串
			USART_RX_STA = 0;
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
			return (u8*)strx;
        }
    }

}

void Lora_Init()
{
	char buf[32];
	GPIO_InitTypeDef  GPIO_InitStructure;   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 

	LORA_MD0=1;//MD0引脚拉高，进入AT模式
	printf("ATE0\r\n");
	delay_ms(100);

	while(1)
    {
        if(LORA_CMD((u8 *)"AT", (u8 *)"OK") != 0)//是否检测到模块
        {
			sprintf(buf, "+WLRATE:%u,%u",0, 5); //无线速率和信道配置
            if(LORA_CMD((u8 *)"AT+WLRATE?", (u8 *)buf) == 0)
			{
				sprintf(buf, "AT+WLRATE=%u,%u",0, 5);
                LORA_CMD((u8 *)buf, (u8 *)"OK"); 
			}						
	
            if(LORA_CMD((u8 *)"AT+UART?", (u8 *)"+UART:7,0") == 0) //比特率
			{
				LORA_CMD((u8 *)"AT+UART=7,0", (u8 *)"OK"); //设置115200比特率,无校验
			}

            if(LORA_CMD((u8 *)"AT+WLTIME?", (u8 *)"+WLTIME:0") == 0) //休眠时间
			{
				LORA_CMD((u8 *)"AT+WLTIME=0", (u8 *)"OK"); //设置1秒
			}
						
            if(LORA_CMD((u8 *)"AT+TPOWER?", (u8 *)"+TPOWER:3") == 0) //发射功率
			{
				LORA_CMD((u8 *)"AT+WLTIME=0", (u8 *)"OK"); //设置20dbm
			}

            if(LORA_CMD((u8 *)"AT+CWMODE?", (u8 *)"+CWMODE:0") == 0) //工作模式
			{
				LORA_CMD((u8 *)"AT+CWMODE=0", (u8 *)"OK"); //设置一般模式
			}
									
			if(LORA_CMD((u8 *)"AT+ADDR=? ", (u8 *)"+ADDR:00,01") == 0) //工作模式
			{
				LORA_CMD((u8 *)"AT+ADDR=00,01", (u8 *)"OK"); //设置一般模式
			}
			
            if(LORA_CMD((u8 *)"AT+TMODE?", (u8 *)"+TMODE:0") == 0) //发送状态
			{
				LORA_CMD((u8 *)"AT+TMODE=0", (u8 *)"OK"); //透明传输
			}
						
			sprintf(buf, "+ADDR:%02x,%02x", 0, 1); //设备地址
            if(LORA_CMD((u8 *)"AT+ADDR?", (u8 *)buf) == 0)
			{
				sprintf(buf, "AT+ADDR=%02x,%02x", 0, 1);
				LORA_CMD((u8 *)buf, (u8 *)"OK"); 
			}		
            break;
        }
    }
	printf("AT+FLASH=1\r\n");
	LORA_MD0=0;
	PCout(13)=!PCout(13);
}