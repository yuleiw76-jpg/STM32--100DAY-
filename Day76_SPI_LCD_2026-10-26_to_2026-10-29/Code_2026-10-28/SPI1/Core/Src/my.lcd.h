#ifndef MY_LCD_H
#define MY_LCD_H

#include "main.h"

#define LCD_RES_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)
#define LCD_RES_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)
#define LCD_DC_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET)
#define LCD_DC_Set()HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET)
#define USE_HORIZONTAL 2
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 320
#define LCD_H 480

#else
#define LCD_W 480
#define LCD_H 320
#endif



//????
#define WHITE                  0xFFFF
#define BLACK                  0x0000          
#define BLUE                   0x001F  
#define BRED                   0XF81F
#define GRED                   0XFFE0
#define GBLUE                  0X07FF
#define RED                    0xF800
#define MAGENTA                0xF81F
#define GREEN                  0x07E0
#define CYAN                   0x7FFF
#define YELLOW                 0xFFE0
#define BROWN                  0XBC40 //??
#define BRRED                  0XFC07 //???
#define GRAY                   0X8430 //??
#define DARKBLUE               0X01CF //???
#define LIGHTBLUE              0X7D7C //???  
#define GRAYBLUE               0X5458 //???
#define LIGHTGREEN             0X841F //???
#define LGRAY                  0XC618 //???(PANNEL),?????
#define LGRAYBLUE              0XA651 //????(?????)
#define LBBLUE                 0X2B12 //????(???????)
void LCD_Init(void);

void LCD_Fill(uint16_t xsta, uint16_t ysta,
              uint16_t xend, uint16_t yend,
              uint16_t color);
#endif
