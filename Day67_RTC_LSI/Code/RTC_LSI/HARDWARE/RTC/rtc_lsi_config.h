#ifndef RTC_LSI_CONFIG_H
#define RTC_LSI_CONFIG_H

#include "sys.h"

/* Nominal frequency only: the actual LSI frequency is not calibrated. */
#define RTC_LSI_NOMINAL_HZ 40000UL
#define RTC_LSI_PRESCALER (RTC_LSI_NOMINAL_HZ - 1UL)
#define RTC_LSI_BACKUP_MARKER ((u16)0x4C53)
#define RTC_LSI_TIMEOUT_MS 1000U

u16 RTC_LSI_GetMilliseconds(void);

#endif

