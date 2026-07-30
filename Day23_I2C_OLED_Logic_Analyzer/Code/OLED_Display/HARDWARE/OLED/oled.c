//////////////////////////////////////////////////////////////////////////////////
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   :
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑÝÊ¾Àý³Ì(STM32F103ZEÏµÁÐIIC)
//              ËµÃ÷:
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPD6£¨SCL£©
//              SDA   ½ÓPD7£¨SDA£©
//              ----------------------------------------------------------------
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
    OLED_SCLK_Set() ;
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
    OLED_SCLK_Set() ;
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

void IIC_Wait_Ack(void)
{
    OLED_SCLK_Set() ;
    OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m, da;
    da = IIC_Byte;
    OLED_SCLK_Clr();

    for(i = 0; i < 8; i++)
    {
        m = da;
        m = m & 0x80;

        if(m == 0x80)
        {
            OLED_SDIN_Set();
        }
        else OLED_SDIN_Clr();

        da = da << 1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78);            //Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00);			       //write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);			//write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

/***********************Delay****************************************/
//×ø±êÉèÖÃ

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    Write_IIC_Command(0xb0 + y);
    Write_IIC_Command(((x & 0xf0) >> 4) | 0x10);
    Write_IIC_Command((x & 0x0f) | 0x01);
}
void OLED_Fill(unsigned char fill_Data)//È«ÆÁÌî³ä
{
    unsigned char m, n;

    for(m = 0; m < 8; m++)
    {
        Write_IIC_Command(0xb0 + m);		//page0-page1
        Write_IIC_Command(0x02);		//low column start address
        Write_IIC_Command(0x10);		//high column start address

        for(n = 0; n < 128; n++)
        {
            Write_IIC_Data(fill_Data);
        }
    }
}
//ÇåÆÁº¯Êý,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!
void OLED_Clear(void)
{
    OLED_Fill(0x00);
}
//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : ½«OLED´ÓÐÝÃßÖÐ»½ÐÑ
//--------------------------------------------------------------
void OLED_On(void)
{
    Write_IIC_Command(0X8D);  //ÉèÖÃµçºÉ±Ã
    Write_IIC_Command(0X14);  //¿ªÆôµçºÉ±Ã
    Write_IIC_Command(0XAF);  //OLED»½ÐÑ
}
//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : ÈÃOLEDÐÝÃß -- ÐÝÃßÄ£Ê½ÏÂ,OLED¹¦ºÄ²»µ½10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
    Write_IIC_Command(0X8D);  //ÉèÖÃµçºÉ±Ã
    Write_IIC_Command(0X10);  //¹Ø±ÕµçºÉ±Ã
    Write_IIC_Command(0XAE);  //OLEDÐÝÃß
}
//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ÆðÊ¼µã×ø±ê(x:0~127, y:0~7); ch[] -- ÒªÏÔÊ¾µÄ×Ö·û´®; TextSize -- ×Ö·û´óÐ¡(1:6*8 ; 2:8*16)
// Description    : ÏÔÊ¾codetab.hÖÐµÄASCII×Ö·û,ÓÐ6*8ºÍ8*16¿ÉÑ¡Ôñ
//--------------------------------------------------------------
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 8:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 122)
						return;

				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
					Write_IIC_Data(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 16:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
						return;

				OLED_Set_Pos(x,y);
				for(i=0;i<8;i++)
					Write_IIC_Data(F8X16[c*16+i]);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++)
					Write_IIC_Data(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}
//³õÊ¼»¯OLEDÆÁÄ»
void OLED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //Ê¹ÄÜPC,D,G¶Ë¿ÚÊ±ÖÓ

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;	 //PB5,PB6ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ËÙ¶È50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);	  //³õÊ¼»¯GPIOD3,6

    delay_ms(100); //ÕâÀïµÄÑÓÊ±ºÜÖØÒª

    Write_IIC_Command(0xAE); //display off
    Write_IIC_Command(0x20);	//Set Memory Addressing Mode
    Write_IIC_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    Write_IIC_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    Write_IIC_Command(0xc8);	//Set COM Output Scan Direction
    Write_IIC_Command(0x00); //---set low column address
    Write_IIC_Command(0x10); //---set high column address
    Write_IIC_Command(0x40); //--set start line address
    Write_IIC_Command(0x81); //--set contrast control register
    Write_IIC_Command(0xff); //ÁÁ¶Èµ÷½Ú 0x00~0xff
    Write_IIC_Command(0xa1); //--set segment re-map 0 to 127
    Write_IIC_Command(0xa6); //--set normal display
    Write_IIC_Command(0xa8); //--set multiplex ratio(1 to 64)
    Write_IIC_Command(0x3F); //
    Write_IIC_Command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    Write_IIC_Command(0xd3); //-set display offset
    Write_IIC_Command(0x00); //-not offset
    Write_IIC_Command(0xd5); //--set display clock divide ratio/oscillator frequency
    Write_IIC_Command(0xf0); //--set divide ratio
    Write_IIC_Command(0xd9); //--set pre-charge period
    Write_IIC_Command(0x22); //
    Write_IIC_Command(0xda); //--set com pins hardware configuration
    Write_IIC_Command(0x12);
    Write_IIC_Command(0xdb); //--set vcomh
    Write_IIC_Command(0x20); //0x20,0.77xVcc
    Write_IIC_Command(0x8d); //--set DC-DC enable
    Write_IIC_Command(0x14); //
    Write_IIC_Command(0xaf); //--turn on oled panel
    OLED_Clear();
}





























