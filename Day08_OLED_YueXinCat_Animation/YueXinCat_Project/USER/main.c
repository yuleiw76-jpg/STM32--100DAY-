#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "oled.h"
#include "stdio.h"
#include "yuexinmiao_oled_frames.h"

int main(void)
{	
	u8 a;
	int your_money=100;
	char text[20];
	delay_init();
	LED_Init();
	KEY_Init();
	OLED_Init(); 
	OLED_Clear();	//«Â∆¡
	unsigned char frame_index;

	
	while(1)
{
    for(frame_index = 0;
        frame_index < YUEXINMIAO_FRAME_COUNT;
        frame_index++)
    {
        OLED_ShowFrame(yuexinmiao_frames[frame_index]);
        delay_ms(YUEXINMIAO_FRAME_DELAY_MS);
    }
}
}
