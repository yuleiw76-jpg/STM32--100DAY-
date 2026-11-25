/**
 * @file FOC.h
 * @author Aiano_czm
 * @brief
 */

#ifndef FOC_H
#define FOC_H

#define _PWM_RANGE 4095.0 // 2^12 -1 = 4095

void FOC_init(void);
void FOC_SVPWM(float Uq, float Ud, float angle);
#endif //FOC_DRIVER_FOC_H
