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

#include "movement.h"
#include "sensors.h"

int main(int argc, char ** argv) {
    /* Initialise pigpio library */
    if(gpioInitialise() < 0) return 1;
    atexit(gpioTerminate);
    
    /* Initialise modules */
    imu_init(0.0f,0.0f,0.0f,NULL); /* TODO initial position (x,y,heading) */
    m_init();
    
    /* TODO main loop goes here */

    return 0;    
}


