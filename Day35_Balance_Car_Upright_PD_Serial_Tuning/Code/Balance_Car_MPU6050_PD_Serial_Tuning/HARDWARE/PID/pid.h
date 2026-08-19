#ifndef __PID_H
#define __PID_H	 
#include "sys.h"

extern float P;
extern float D;
extern float aim_pitch;

float pid_control(float PITCH);
		 				    
#endif
