#include "encode.h"

void TIM4_ENCODE_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* One output-shaft revolution produces about 1560 encoder counts. */
    TIM_TimeBaseStructure.TIM_Period = 1560 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM4,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_SetCounter(TIM4, 0);
    TIM_Cmd(TIM4, ENABLE);
}

int circle_count = 0;

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
    {
        if (((TIM4->CR1 >> 4) & 0x01) == 0)
        {
            circle_count++;
        }
        else if (((TIM4->CR1 >> 4) & 0x01) == 1)
        {
            circle_count--;
        }

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
