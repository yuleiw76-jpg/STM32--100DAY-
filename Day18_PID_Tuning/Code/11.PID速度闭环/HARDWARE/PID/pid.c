#include "pid.h"

static float err_last_last = 0.0f;
static float err_last = 0.0f;
static float err = 0.0f;

/* Final Day18 gains after the P, I, and D comparison tests. */
static float P = 100.0f;
static float I = 30.0f;
static float D = 5.0f;

static float off_cal = 0.0f;

float a_m = 0.0f;
float aim_v = 0.5f;

float pid_control(float speed)
{
    float new_output;

    err_last_last = err_last;
    err_last = err;
    err = aim_v - speed;

    off_cal = P * (err - err_last)
            + D * (err - 2.0f * err_last + err_last_last);

    /* Stop integration only when it would drive the output farther into saturation. */
    if (!((a_m >= 1000.0f && err > 0.0f) ||
          (a_m <= 0.0f && err < 0.0f)))
    {
        off_cal += I * err;
    }

    /* Desired incremental PID output before engineering limits. */
    new_output = a_m + off_cal;

    /* Limit each control period to +30 when rising and -200 when falling. */
    if (new_output - a_m > 30.0f)
    {
        new_output = a_m + 30.0f;
    }
    else if (new_output - a_m < -200.0f)
    {
        new_output = a_m - 200.0f;
    }

    a_m = new_output;

    /* Clamp the final PWM command. */
    if (a_m > 1000.0f)
    {
        a_m = 1000.0f;
    }
    else if (a_m < 0.0f)
    {
        a_m = 0.0f;
    }

    return a_m;
}
