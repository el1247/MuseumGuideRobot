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
		int prox_i2c; //Stores the handle for the i2c connections
		uint8_t busno, interruptpin, error; //Record of bus number, interrupt gpio number  and any errors
		uint16_t ALS_thresh_low, ALS_thresh_high; //Stores the latest ALS threshold limits
		uint16_t PS_thresh_low, PS_thresh_high; //Stores the latest PS threshold limits

		proximity(uint8_t busnumber, uint8_t inputpin, uint16_t ALS_thresh_low_ini, uint16_t ALS_thresh_high_ini, uint16_t PS_thresh_low_ini, uint16_t PS_thresh_high_ini); //Initialiser
		~proximity(); //Destructor
		static void proxdetection(int gpio, int level, uint32_t tick);
		int writeMSB_Prox(uint8_t reg, uint8_t MSB);
		int writeLSB_Prox(uint8_t reg, uint8_t LSB);

	public:
		uint16_t ALSval, PSval; //ALS value and PS value

		int configALSthresh(uint16_t ALSint_thresh_low, uint16_t ALSint_thresh_high);
		int configPSthresh(uint16_t PSint_thresh_low, uint16_t PSint_thresh_high);
		int getbusnumber(); //Method for returning the busnumber assigned to the class instance
		int measureALS(); //Method for measuring ALS value
		int measurePS(); //Method for measuring PS value
};

#endif