#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

void OLED_IIC_Start(void)
{
    OLED_SCLK_Set() ;
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void OLED_IIC_Stop(void)
{
    OLED_SCLK_Set() ;
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

void OLED_IIC_Wait_Ack(void)
{
    OLED_SCLK_Set() ;
    OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void OLED_Write_IIC_Byte(unsigned char IIC_Byte)
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
void OLED_Write_IIC_Command(unsigned char IIC_Command)
{
    OLED_IIC_Start();
    OLED_Write_IIC_Byte(0x78);            //Slave address,SA0=0
    OLED_IIC_Wait_Ack();
    OLED_Write_IIC_Byte(0x00);			       //write command
    OLED_IIC_Wait_Ack();
    OLED_Write_IIC_Byte(IIC_Command);
    OLED_IIC_Wait_Ack();
    OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void OLED_Write_IIC_Data(unsigned char IIC_Data)
{
    OLED_IIC_Start();
    OLED_Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
    OLED_IIC_Wait_Ack();
    OLED_Write_IIC_Byte(0x40);			//write data
    OLED_IIC_Wait_Ack();
    OLED_Write_IIC_Byte(IIC_Data);
    OLED_IIC_Wait_Ack();
    OLED_IIC_Stop();
}

/***********************Delay****************************************/
//坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_Write_IIC_Command(0xb0 + y);
    OLED_Write_IIC_Command(((x & 0xf0) >> 4) | 0x10);
    OLED_Write_IIC_Command((x & 0x0f) | 0x01);
}
void OLED_Fill(unsigned char fill_Data)//全屏填充
{
    unsigned char m, n;

    for(m = 0; m < 8; m++)
    {
        OLED_Write_IIC_Command(0xb0 + m);		//page0-page1
        OLED_Write_IIC_Command(0x02);		//low column start address
        OLED_Write_IIC_Command(0x10);		//high column start address

        for(n = 0; n < 128; n++)
        {
            OLED_Write_IIC_Data(fill_Data);
        }
    }
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    OLED_Fill(0x00);
}
//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_On(void)
{
    OLED_Write_IIC_Command(0X8D);  //设置电荷泵
    OLED_Write_IIC_Command(0X14);  //开启电荷泵
    OLED_Write_IIC_Command(0XAF);  //OLED唤醒
}
//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
    OLED_Write_IIC_Command(0X8D);  //设置电荷泵
    OLED_Write_IIC_Command(0X10);  //关闭电荷泵
    OLED_Write_IIC_Command(0XAE);  //OLED休眠
}
//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
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
					OLED_Write_IIC_Data(F6x8[c][i]);
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
					OLED_Write_IIC_Data(F8X16[c*16+i]);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++)
					OLED_Write_IIC_Data(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}
//初始化OLED屏幕
void OLED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC,D,G端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;	 //PB5,PB6推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6

    delay_ms(100); //这里的延时很重要

    OLED_Write_IIC_Command(0xAE); //display off
    OLED_Write_IIC_Command(0x20);	//Set Memory Addressing Mode
    OLED_Write_IIC_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_Write_IIC_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    OLED_Write_IIC_Command(0xc8);	//Set COM Output Scan Direction
    OLED_Write_IIC_Command(0x00); //---set low column address
    OLED_Write_IIC_Command(0x10); //---set high column address
    OLED_Write_IIC_Command(0x40); //--set start line address
    OLED_Write_IIC_Command(0x81); //--set contrast control register
    OLED_Write_IIC_Command(0xff); //亮度调节 0x00~0xff
    OLED_Write_IIC_Command(0xa1); //--set segment re-map 0 to 127
    OLED_Write_IIC_Command(0xa6); //--set normal display
    OLED_Write_IIC_Command(0xa8); //--set multiplex ratio(1 to 64)
    OLED_Write_IIC_Command(0x3F); //
    OLED_Write_IIC_Command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_Write_IIC_Command(0xd3); //-set display offset
    OLED_Write_IIC_Command(0x00); //-not offset
    OLED_Write_IIC_Command(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_Write_IIC_Command(0xf0); //--set divide ratio
    OLED_Write_IIC_Command(0xd9); //--set pre-charge period
    OLED_Write_IIC_Command(0x22); //
    OLED_Write_IIC_Command(0xda); //--set com pins hardware configuration
    OLED_Write_IIC_Command(0x12);
    OLED_Write_IIC_Command(0xdb); //--set vcomh
    OLED_Write_IIC_Command(0x20); //0x20,0.77xVcc
    OLED_Write_IIC_Command(0x8d); //--set DC-DC enable
    OLED_Write_IIC_Command(0x14); //
    OLED_Write_IIC_Command(0xaf); //--turn on oled panel
    OLED_Clear();
}





























