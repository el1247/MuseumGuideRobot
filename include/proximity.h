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

enum errorcode {
	noerror,
	i2cconnection_fail,
	ALSconfig_fail,
	PS12config_fail,
	PS3MSconfig_fail,
	PScanc_fail,
	ALSthreshconfig_low_fail,
	ALSthreshconfig_high_fail,
	PSthreshconfig_low_fail,
	PSthreshconfig_high_fail,
	ALSread_fail,
	ALSmaxvalue_exceed_fail,
	PSread_fail,
    PSmaxvalue_exceed_fail,
	MSBwrite_read_fail,
	MSBwrite_write_fail,
	LSBwrite_read_fail,
	LSBwrite_write_fail
};

class proximity{
	private:
		int prox_i2c; //Stores the handle for the i2c connections
		
		uint16_t ALS_thresh_low, ALS_thresh_high; //Stores the latest ALS threshold limits
		uint16_t PS_thresh_low, PS_thresh_high; //Stores the latest PS threshold limits

		int writeMSB_Prox(uint16_t reg, uint8_t MSB);
		int writeLSB_Prox(uint16_t reg, uint8_t LSB);

	public:
		errorcode error = noerror; //Record of any errors
	
		uint8_t interruptpin, interruptmode; //interrupt gpio number and tracker if interrupts are enabled
		uint16_t ALSval, PSval; //ALS value and PS value

		proximity(uint8_t inputpin, uint16_t ALS_thresh_low_ini, uint16_t ALS_thresh_high_ini, uint16_t PS_thresh_low_ini, uint16_t PS_thresh_high_ini); //Initialiser
		~proximity(); //Destructor
		int configALSthresh(uint16_t ALSint_thresh_low, uint16_t ALSint_thresh_high);
		int configPSthresh(uint16_t PSint_thresh_low, uint16_t PSint_thresh_high);
        int configinterrupt(void(*methodcall)(int, int, uint32_t));
		int measureALS(); //Method for measuring ALS value
		int measurePS(); //Method for measuring PS value
};

#endif
