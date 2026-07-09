#include "servo.h"
#include "pwm.h"

#define SERVO_MIN_CCR    500
#define SERVO_MAX_CCR    2500
#define SERVO_MAX_ANGLE  180

void Servo_SetAngle(u16 angle)
{
    u16 ccr;

    if(angle > SERVO_MAX_ANGLE)
    {
        angle = SERVO_MAX_ANGLE;
    }

    ccr = SERVO_MIN_CCR + (u32)angle * (SERVO_MAX_CCR - SERVO_MIN_CCR) / SERVO_MAX_ANGLE;

    TIM_SetCompare1(TIM1, ccr);
}