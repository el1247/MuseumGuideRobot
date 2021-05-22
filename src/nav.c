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
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "movement.h"
#include "sensors.h"
#include "nav.h"
#include "MadgwickAHRS.h"

#define PID_DECLS(id) static float id##_err, id##_sum; float id##_last
#define PID(id,ecalc,kp,ki,kd) (id##_last = id##_err, id##_sum += (id##_err = (ecalc)), \
                                    (kp)*id##_err + (ki)*id##_sum + (kd)*(id##_err-id##_last))


static timer_t ticker;
static struct sigevent tickevt;
static struct itimerspec tickspec = { .it_interval = { .tv_sec = 0, .tv_nsec = SAMPLE_TIME_NS },
                               .it_value = { .tv_sec = 0, .tv_nsec = 500000000 }};

static enum {
    NAV_STOP, NAV_TURN, NAV_TRAVEL
} state = NAV_STOP;

static float sx, sy, tx, ty, course;
static volatile int en = 0;
static void (*on_complete)(void);
static void (*nxt_cbk)(void);
void nav_tick(sigval_t);

void nav_init(void) {
    tickevt.sigev_notify = SIGEV_THREAD;
    tickevt.sigev_notify_function = &nav_tick;
    tickevt.sigev_notify_attributes = NULL; /* XXX Possibly increase priority? */

    if(timer_create(CLOCK_MONOTONIC, &tickevt, &ticker)
        || timer_settime(ticker, 0, &tickspec, NULL)) {
        perror("Error starting nav update timer: ");
    }
    atexit(nav_fini);
}

void nav_set_turn(float hdg, void (*callback)(void)) {
    state = NAV_STOP;
    course = hdg;
    on_complete = callback;
    state = NAV_TURN;
}

static void resume(void) {
    on_complete = nxt_cbk;
    state = NAV_TRAVEL;
}

/* XXX TODO determine */
#define HTOLW (M_PI/4.0)
#define HTOLN (M_PI/16.0)
#define DSQTOL 0.1
#define MOV_SPD 1.0
#define PATH_KP 0.0
#define PATH_KI 0.0
#define PATH_KD 0.0



void nav_set_travel(float x, float y, void (*callback)(void)) {
    state = NAV_STOP;
    tx = x, ty = y;
    get_current_position(&sx,&sy);
    course = atan2f(ty - sy, tx - sx);
    if(fabs(get_current_heading() - course) > HTOLW) {
	on_complete = &resume;
        nxt_cbk = callback;
	state = NAV_TURN;
    } else {
        on_complete = callback;
        state = NAV_TRAVEL;
    }
}

void nav_tick(sigval_t _) {
    PID_DECLS(path);
    float x, y, ex, ey, esq, off_angle, correction;
    switch(state) {
      case NAV_TURN:
        if(fabsf(get_current_heading() - course) < HTOLN) {
	    state = NAV_STOP;
            m_stop();
	    on_complete();
	} else m_drive(0.0, course);
	return;
      case NAV_TRAVEL:
        get_current_position(&x,&y);
	ex = tx - x, ey = ty - y;
	esq = ex*ex + ey*ey;
	if(esq < DSQTOL) {
	    state = NAV_STOP;
	    m_stop();
	    on_complete();
	} else {
	    off_angle = sqrtf(ex*ex + ey*ey) * sinf(atan2f(ey,ex) - course);
	    correction = PID(path,off_angle,PATH_KP,PATH_KI,PATH_KD);
	    m_drive(MOV_SPD, course + correction);
	}
	return;
      default:
	m_stop();
    }
}

void nav_cancel(void) {
    state = NAV_STOP;
    m_stop();
}

void nav_fini(void) {
    timer_delete(ticker);
    nav_cancel();
}

