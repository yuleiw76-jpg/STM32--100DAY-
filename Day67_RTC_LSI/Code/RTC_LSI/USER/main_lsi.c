#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "rtc.h"
#include "rtc_lsi_config.h"

int main(void)
{
    u8 rtc_status;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    LED_Init();
    /* Enable AFIO before KEY_Init disables JTAG for PB4. */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    KEY_Init();
    EXTIX_Init();
    uart_init(115200);

    printf("\r\nRTC LSI DEMO: nominal %lu Hz, PRL=%lu\r\n",
           RTC_LSI_NOMINAL_HZ, RTC_LSI_PRESCALER);
    printf("RTC LSI init...\r\n");
    rtc_status = RTC_Init();
    if (rtc_status != 0)
    {
        printf("RTC LSI init failed: code=%u\r\n", (unsigned)rtc_status);
        while (1)
        {
            delay_ms(1000);
        }
    }

    printf("RTC LSI init OK; powered-on test only, LSI uncalibrated\r\n");
    printf("year month day week hour min sec approx_ms\r\n");

    while (1)
    {
        delay_ms(100);
        calendar.m_sec = RTC_LSI_GetMilliseconds();
        /* One printf evaluates all values before serial transmission.
           It still does not promise an atomic calendar/divider snapshot. */
        printf("%u %u %u %u %u %u %u %u\r\n",
               (unsigned)calendar.w_year, (unsigned)calendar.w_month,
               (unsigned)calendar.w_date, (unsigned)calendar.week,
               (unsigned)calendar.hour, (unsigned)calendar.min,
               (unsigned)calendar.sec, (unsigned)calendar.m_sec);
    }
}

