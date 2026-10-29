#include "my.lcd.h"
#include "spi.h"
#include "lcdfont.h"

void LCD_WR_DATA8(uint8_t dat){
    LCD_DC_Set();
	  HAL_SPI_Transmit(&hspi1,&dat,1,10);
}

void LCD_WR_DATA(uint16_t dat){
   uint8_t dataBuf[2];
	 dataBuf[0] = dat >> 8 ;
	 dataBuf[1] = dat & 0xFF ;
	 LCD_DC_Set() ;
	 HAL_SPI_Transmit(&hspi1,dataBuf,2,10);
	 
}

void LCD_WR_REG(uint8_t dat){
   LCD_DC_Clr();
	 HAL_SPI_Transmit(&hspi1,&dat,1,10);
}

void LCD_Init(void)
{
  LCD_RES_Clr();
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(200);
	
	LCD_WR_REG(0x11);
	HAL_Delay(120);
	
	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0xC3);
	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0x96);
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x48);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0x88);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x28);
	else LCD_WR_DATA8(0xE8);
	
	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);
	
	LCD_WR_REG(0Xe8);
	LCD_WR_DATA8(0x40);
	LCD_WR_DATA8(0x82);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x0a);
	LCD_WR_DATA8(0xb6);
	LCD_WR_DATA8(0x33);
	
	LCD_WR_REG(0Xc5);
	LCD_WR_DATA8(0x27);
	
	LCD_WR_REG(0Xc2);
	LCD_WR_DATA8(0xa7);
	
	LCD_WR_REG(0Xe0);
	LCD_WR_DATA8(0xf0);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0f);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x1d);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x0c);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x15);
	
	LCD_WR_REG(0Xe1);
	LCD_WR_DATA8(0xf0);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x0a);
	LCD_WR_DATA8(0x0b);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x0c);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x17);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x16);
	
	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0x3c);
	
	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0x69);
	
	LCD_WR_REG(0X29);	 
}

/******************************************************************************
      ????:?????????
      ????:x1,x2 ???????????
                y1,y2 ???????????
      ???:  ?
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
        LCD_WR_REG(0x2a);//?????
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//?????
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//????
}

/******************************************************************************
      ????:?????????
      ????:xsta,ysta   ????
               xend,yend   ????
               color       ??????
      ???:  ?
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
        uint16_t i,j; 
        LCD_Address_Set(xsta,ysta,xend-1,yend-1);//??????
        for(i=ysta;i<yend;i++)
        {                                                                                                                            
                for(j=xsta;j<xend;j++)
                {
                        LCD_WR_DATA(color);
                }
        }                                                       
}
