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
#include "proximity.h"


//Proximity sensor I2C addressing
#define PROX_I2CBUSNO 0 //I2C bus number 					///check
#define PROX_ADDR 0x51 //7 bit slave address (lsb 0=r, 1=w)
#define PROX_ADDR_R PROX_ADDR << 1 //8 bit slave address read
#define PROX_ADDR_W (PROX_ADDR << 1) + 1 //8 bit slave address write

#define PROX_ALS_CONF 0x00 //Low
#define PROX_ALS_DATA 0x09 //Low + High
#define PROX_PS_CONF1 0x03 //Low
#define PROX_PS_CONF2 0x03 //High
#define PROX_PS_CONF3 0x04 //Low

//Proximity sensor comand code values
#define CMD_PROX_START 0b01000010 //integration time 50ms, channel interrupt, interrupt persistance setting 1, interrupt enabled, ALS power on


proximity::proximity(){
	//Initialises GPIO libary and establishes I2C connection to proximity sensor
	if (gpioInitialise() < 0){
		std::cout << "Pigpio failed to initialise" << std::endl; //Possibile duplicate command, may already be initialised
		error = 1; //marks failure for gpio initialisation
	}else if ((prox_i2c_r = (unsigned) i2cOpen(PROX_I2CBUSNO, PROX_ADDR_R, 0)) < 0) {
		std::cout << "Proximity I2C read connection failed" << std::endl;
		error = 2; //marks failure for i2c read connection failing
	}else if ((prox_i2c_w = (unsigned) i2cOpen(PROX_I2CBUSNO, PROX_ADDR_W, 0)) < 0) {
		std::cout << "Proximity I2C write connection failed" << std::endl;
		error = 3; //marks failure for i2c write connection failing
	}else if (i2cWriteByteData(prox_i2c_w,PROX_ALS_CONF, CMD_PROX_START)){
		std::cout << "Proximity sensor config failure" << std::endl;
		error = 4; //marks failure for i2c configuration
	}else error = 0;
}


void proximity::measuredist(){
	//Function to capture and store latest value from proximity sensor
	distance = i2cReadWordData(prox_i2c_r, PROX_ALS_DATA);
	if (!distance) {
		std::cout << "Failure to read distance" << std::endl;
		error = 11; //marks failure to read ALS DATA ///error value not currently fixed
	}
}


proximity::~proximity(){
	//Closes GPIO libary and I2C connection to proximity sensor
	gpioTerminate();
	if (i2cClose(prox_i2c_r)) std::cout << "Error closing i2c read connection" << std::endl;
	if (i2cClose(prox_i2c_w)) std::cout << "Error closing i2c write connection" << std::endl;
}