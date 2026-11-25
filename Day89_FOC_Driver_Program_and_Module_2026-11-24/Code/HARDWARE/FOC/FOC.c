#include <math.h>
#include "FOC.h"
#include "FOC_conf.h"
#include "FOC_utils.h"
#include "hardware_api.h"
#include "tim1.h"

void FOC_init() {
	TIM1_Init(4096-1,0);   //ÖÜÆÚÎª0.000114s
}

u16 a=0;
void FOC_SVPWM(float Uq, float Ud, float angle) {

    int sector;
    float Uout = _sqrt(Ud * Ud + Uq * Uq) / VOLTAGE_LIMIT; 
    angle = _normalizeAngle(angle +  atan2(Uq, Ud));   

    sector = floor(angle / _PI_3) + 1;
	
    float T1 = _SQRT3 * _sin(sector * _PI_3 - angle) * Uout;   
    float T2 = _SQRT3 * _sin(angle - (sector - 1.0f) * _PI_3) * Uout;
    float T0 = 1 - T1 - T2; 
	
    float Ta, Tb, Tc;
    switch (sector) {
        case 1:
            Ta = T1 + T2 + T0 / 2;
            Tb = T2 + T0 / 2;
            Tc = T0 / 2;
            break;
        case 2:
            Ta = T1 + T0 / 2;
            Tb = T1 + T2 + T0 / 2;
            Tc = T0 / 2;
            break;
        case 3:
            Ta = T0 / 2;
            Tb = T1 + T2 + T0 / 2;
            Tc = T2 + T0 / 2;
            break;
        case 4:
            Ta = T0 / 2;
            Tb = T1 + T0 / 2;
            Tc = T1 + T2 + T0 / 2;
            break;
        case 5:
            Ta = T2 + T0 / 2;
            Tb = T0 / 2;
            Tc = T1 + T2 + T0 / 2;
            break;
        case 6:
            Ta = T1 + T2 + T0 / 2;
            Tb = T0 / 2;
            Tc = T1 + T0 / 2;
            break;
        default:
            // possible error state
            Ta = 0;
            Tb = 0;
            Tc = 0;
    }

    // Ta, Tb, Tc range [0,1]

	// transform duty cycle from [0,1] to [0,_PWM_RANGE]
	TIM_SetCompare1(TIM1,_PWM_RANGE*Ta);
	TIM_SetCompare2(TIM1,_PWM_RANGE*Tb);
	TIM_SetCompare3(TIM1,_PWM_RANGE*Tc);
	
}
