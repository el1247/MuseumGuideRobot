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
		uint16_t ALSval, PSval; //ALS value and PS value
		int distance; //Distance to object measured from proximity sensor
		uint8_t error; //Record of any errors
		int prox_i2c; //Stores the handle for the i2c connections

		proximity(uint16_t ALSint_thresh_lowini, uint16_t ALSint_thresh_highini, uint16_t PSint_thresh_lowini, uint16_t PSint_thresh_highini); //Initialiser
		~proximity(); //Destructor
		int writeMSB_Prox(uint8_t reg, uint8_t MSB);
		int writeLSB_Prox(uint8_t reg, uint8_t LSB);
		//uint16_t read16(uint16_t comcode); //16 bit read method for 16 bit addr registers

	public:
		int configALSthresh(uint16_t ALSint_thresh_low, uint16_t ALSint_thresh_high);
		int configPSthresh(uint16_t PSint_thresh_low, uint16_t PSint_thresh_high);
		int measureALS(); //Method for measuring ALS value
		int measurePS(); //Method for measuring PS value
};

#endif