#ifndef __TIM_H
#define __TIM_H

#include "sys.h"

void TIM2_Init(u16 arr, u16 psc);
void TIM3_Init(u16 arr, u16 psc);

extern float speed;

#endif
