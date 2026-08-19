#ifndef __TIM_H
#define __TIM_H
#include "sys.h"
  
extern float speed;
extern float Pitch,Roll,Yaw;
extern float pidp,pidv;
extern volatile u32 control_tick_ms;
extern volatile u8 motor_control_enabled;
extern volatile u8 dmp_miss_count;
extern volatile u8 attitude_valid;

void TIM1_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);
void Balance_Control_SetEnabled(u8 enabled);


#endif
