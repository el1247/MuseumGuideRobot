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

class proximity{
	private:
		int distance; //Distance to object measured from proximity sensor
		int error; //Record of any errors
		int prox_i2c_r, prox_i2c_w; //Stores the handle for the i2c connections
		proximity(); //Initialiser
		~proximity(); //Destructor
		uint16_t read16(uint16_t comcode); //16 bit read method for 16 bit addr registers
	public:
		void measuredist(); //Method for measuring distance
};

#endif