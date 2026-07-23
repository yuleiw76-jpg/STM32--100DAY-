#ifndef __PID_H
#define __PID_H

#include "sys.h"

float pid_control(float speed);

extern float a_m;
extern float aim_v;

#endif
