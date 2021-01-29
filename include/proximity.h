/* Copyright (C) 2021 Elliott Lawrence
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

#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <stdio.h>
#include <iostream>
#include <pigpio.h>

#define proxstart 0b01000010 //integration time 50ms, channel interrupt, interrupt persistance setting 1, interrupt enabled, ALS power on


class proximity{
	private:
		int distance; //Distance to object measured from proximity sensor
		proximity(); //Initialiser
	public:
		void measuredist(); //Method for measuring distance
};

#endif