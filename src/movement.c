/* Copyright (C) 2021 Duncan Lowther
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <pigpio.h>
#include <stdlib.h>

#include "gpio_assign.h"
#include "movement.h"
#include "sensors.h"

#define MASK_MOTOR_L ((1<<PIN_M_L_FWD)|(1<<PIN_M_L_REV)|(1<<PIN_M_L_PWM))
#define MASK_MOTOR_R ((1<<PIN_M_R_FWD)|(1<<PIN_M_R_REV)|(1<<PIN_M_R_PWM))
#define MASK_MOTOR_ALL (MASK_MOTOR_L|MASK_MOTOR_R)

#define PWM_RNG PI_HW_PWM_RANGE /* Hardware PWM has set range of 1M */

/* XXX placeholder or default values for PWM_FREQ and PID constants -- TODO tune */
#define PWM_FREQ 22000
#define VEL_KP 0.0
#define VEL_KI 0.0
#define VEL_KD 0.0
#define HDG_KP 0.0
#define HDG_KI 0.0
#define HDG_KD 0.0

#define PID_DECLS(id) static double id##_err, id##_sum; double id##_last
#define PID(id,ecalc,kp,ki,kd) (id##_last = id##_err, id##_sum += (id##_err = (ecalc)), \
		                    (kp)*id##_err + (ki)*id##_sum + (kd)*(id##_err-id##_last))

void m_init(void) {
    gpioSetMode(PIN_M_L_FWD, PI_OUTPUT);
    gpioSetMode(PIN_M_L_REV, PI_OUTPUT);
    gpioSetMode(PIN_M_L_PWM, PI_OUTPUT);
    gpioSetMode(PIN_M_R_FWD, PI_OUTPUT);
    gpioSetMode(PIN_M_R_REV, PI_OUTPUT);
    gpioSetMode(PIN_M_R_PWM, PI_OUTPUT);
    atexit(m_fini);
}


int m_drive(double velocity, unsigned heading) {
    double cm, diff;
    int left, right, err0, err1, err2, err3, err4, err5;
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

    err0 = gpioWrite(PIN_M_L_FWD, left > 0);
    err1 = gpioWrite(PIN_M_L_REV, left < 0);
    err2 = gpioHardwarePWM(PIN_M_L_PWM, PWM_FREQ, abs(left));
    err3 = gpioWrite(PIN_M_R_FWD, right > 0);
    err4 = gpioWrite(PIN_M_R_REV, right < 0);
    err5 = gpioHardwarePWM(PIN_M_R_PWM, PWM_FREQ, abs(right));

    if(__builtin_expect(err0|err1|err2|err3|err4|err5, 0)) {
        /* XXX TODO Probably should log errors somehow -- for now just inform caller */
        return -1;
    } else {
        return 0;
    }
}

void m_stop(void) {
    gpioWrite_Bits_0_31_Set(MASK_MOTOR_ALL);
}

void m_fini(void) {
    gpioWrite_Bits_0_31_Clear(MASK_MOTOR_ALL);
}

