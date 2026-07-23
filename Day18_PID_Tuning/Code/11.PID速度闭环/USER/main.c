#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "tim.h"
#include "encode.h"
#include "oled.h"
#include "stdio.h"
#include "pid.h"

int main(void)
{
    char text[20];

    delay_init();
    LED_Init();
    TIM1_PWM_Init(3000 - 1, 0);
    TIM4_ENCODE_Init();
    TIM3_Init(10000 - 1, 720 - 1);
    TIM2_Init(10000 - 1, 720 - 1);
    OLED_Init();
    OLED_Clear();

    while (1)
    {
        sprintf(text, "now:%.2f   ", speed);
        OLED_ShowString(8, 4, (u8 *)text, 16);

        sprintf(text, "PWM:%.0f   ", a_m);
        OLED_ShowString(8, 6, (u8 *)text, 16);

        delay_ms(50);
    }
}
