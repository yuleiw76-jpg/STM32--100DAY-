#include "key.h"
#include "delay.h"

void KEY_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTB时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB5
	
} 


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY1_PRESS，KEY1按下
//KEY2_PRESS，KEY2按下 
//注意此函数有响应优先级,KEY1>KEY2

u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;        // ??????
    static u16 hold_count = 0;   // ?????

    // ??????
    if(KEY1 == 1 && KEY2 == 1)
    {
        key_up = 1;
        hold_count = 0;
        return 0;
    }

    // ?????,???
    delay_ms(10);

    // ???????,??????,?????
    if(KEY1 == 1 && KEY2 == 1)
    {
        key_up = 1;
        hold_count = 0;
        return 0;
    }

    // ?????
    if(key_up == 1)
    {
        key_up = 0;
        hold_count = 0;

        if(KEY1 == 0)
        {
            return KEY1_PRESS;
        }
        else if(KEY2 == 0)
        {
            return KEY2_PRESS;
        }
    }

    // ??????,??????
    if(mode == 1)
    {
        hold_count++;

        // ??? 20 ???????
        // ???????? 10ms,?? 20 ???? 200ms
        if(hold_count >= 20)
        {
            hold_count = 0;

            if(KEY1 == 0)
            {
                return KEY1_PRESS;
            }
            else if(KEY2 == 0)
            {
                return KEY2_PRESS;
            }
        }
    }

    return 0;
}
