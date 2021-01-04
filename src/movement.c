#include <pigpio.h>
#include "gpio_assign.h"
#include "movement.h"
#include "sensors.h"

#define MASK_MOTOR_L ((1<<PIN_M_L_FWD)|(1<<PIN_M_L_REV)|(1<<PIN_M_L_PWM))
#define MASK_MOTOR_R ((1<<PIN_M_R_FWD)|(1<<PIN_M_R_REV)|(1<<PIN_M_R_PWM))
#define MASK_MOTOR_ALL (MASK_MOTOR_L|MASK_MOTOR_R)

/* TODO values for PWM_RNG, PWM_FREQ, and PID constants */

#define PID_DECLS(id) static double id##_err, id##_sum; double id##_last
#define PID(id,ecalc,kp,ki,kd) (id##_last = id##_err, id##_sum += (id##_err = (ecalc)), \
		                    (kp)*id##_err + (ki)*id##_sum + (kd)*(id##_err-id##_last))

void m_init(void) {
    gpioSetMode(PIN_M_L_FWD, PI_OUTPUT);
    gpioSetMode(PIN_M_L_REV, PI_OUTPUT);
    gpioSetMode(PIN_M_L_PWM, PI_OUTPUT);
    gpioSetPWMrange(PIN_M_L_PWM, PWM_RNG);
    gpioSetPWMfrequency(PIN_M_L_PWM, PWM_FREQ); 
    
    gpioSetMode(PIN_M_R_FWD, PI_OUTPUT);
    gpioSetMode(PIN_M_R_REV, PI_OUTPUT);
    gpioSetMode(PIN_M_R_PWM, PI_OUTPUT);
    gpioSetPWMrange(PIN_M_R_PWM, PWM_RNG);
    gpioSetPWMfrequency(PIN_M_R_PWM, PWM_FREQ); 
}


int m_drive(double velocity, unsigned heading) {
    double cm, diff;
    int left, right;
    PID_DECLS(vel);
    PID_DECLS(hdg);

    cm = PID(vel, velocity - get_current_velocity(), VEL_KP, VEL_KI, VEL_KD);
    diff = PID(hdg, (int)((540 + heading - get_current_heading()) % 360) - 180, HDG_KP, HDG_KI, HDG_KD);

    left = (int) ((cm - diff) * PWM_RNG);
    right = (int) ((cm + diff) * PWM_RNG);
    if(left > PWM_RNG) {
        if(right > left) { left -= (right - PWM_RNG); right = PWM_RNG; } 
	else { right -= (left - PWM_RNG); left = PWM_RNG; }
    } else if(right > PWM_RNG) { left -= (right - PWM_RNG); right = PWM_RNG; }
    if(left < -PWM_RNG) {
        if(right < left) { left -= (right + PWM_RNG); right = -PWM_RNG; } 
	else { right -= (left + PWM_RNG); left = -PWM_RNG; }
    } else if(right < -PWM_RNG) { left -= (right + PWM_RNG); right = -PWM_RNG; }

    gpioWrite(PIN_M_L_FWD, left > 0);
    gpioWrite(PIN_M_L_REV, left < 0);
    gpioPWM(PIN_M_L_PWM, abs(left));
    gpioWrite(PIN_M_R_FWD, right > 0)
    gpioWrite(PIN_M_R_REV, right < 0);
    gpioPWM(PIN_M_R_PWM, abs(right));
}

void m_stop(void) {
    gpioWrite_Bits_0_31_Set(MASK_MOTOR_ALL);
}

void m_fini(void) {
    gpioWrite_Bits_0_31_Clear(MASK_MOTOR_ALL);
}

