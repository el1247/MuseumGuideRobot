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

#include <math.h>
#include <stdlib.h>

#include "movement.h"
#include "sensors.h"

#define PID_DECLS(id) static float id##_err, id##_sum; float id##_last
#define PID(id,ecalc,kp,ki,kd) (id##_last = id##_err, id##_sum += (id##_err = (ecalc)), \
                                    (kp)*id##_err + (ki)*id##_sum + (kd)*(id##_err-id##_last))
#define PI ((float)M_PI)
#define TWOPI ((float)(2*M_PI))
#define THREEPI ((float)(3*M_PI))

static enum {
    NAV_STOP, NAV_TURN, NAV_TRAVEL
} state = NAV_STOP;

static float sx, sy, tx, ty, course;
static volatile int en = 0;
static void (*on_complete)(void);

void nav_init(void) {
    atexit(nav_fini);
}

void nav_set(float x, float y, void (*callback)(void)) {
    state = NAV_STOP;
    tx = x, ty = y;
    get_current_position(&sx,&sy);
    course = atan2f(ty - sy, tx - sx);
    on_complete = callback;
    state = NAV_TURN;
}

/* XXX TODO determine */
#define HTOL (M_PI/8.0)
#define DSQTOL 0.1
#define MOV_SPD 1.0
#define PATH_KP 0.0
#define PATH_KI 0.0
#define PATH_KD 0.0

void nav_tick() {
    PID_DECLS(path);
    switch(state) {
      case NAV_TURN:
        if(fabsf(getcurrent_heading() - course) < HTOL) {
            m_stop();
	    state = NAV_STOP;
	    on_complete();
	} else m_drive(0.0, course);
	return;
      case NAV_TRAVEL:
        float x, y, ex, ey, esq, off_angle, correction;
        get_current_position(&x,&y);
	ex = tx - x, ey = ty - y;
	esq = ex*ex + ey*ey;
	if(esq < DSQTOL) {
	    m_stop();
	    state = NAV_STOP;
	    on_complete();
	} else {
	    off_angle = sqrtf(ex*ex + ey*ey) * sinf(atan2f(ey,ex) - course);
	    correction = PID(path,off_angle,kp,ki,kd);
	    m_drive(MOV_SPD, course + correction);
	}
	return;
      default:
	m_stop();
    }
}

void nav_stop(void) {
    gpioWrite_Bits_0_31_Set(MASK_MOTOR_ALL);
}

void nav_fini(void) {
    gpioWrite_Bits_0_31_Clear(MASK_MOTOR_ALL);
}

