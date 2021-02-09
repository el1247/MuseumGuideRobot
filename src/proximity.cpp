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

/*Current issues: 
	verify configuration
		cancellation levels?
	Develop	interrupt functionality
	Extracting distances?
	Use for ALS?
*/

#include "proximity.h"


//Proximity sensor I2C addressing
#define PROX_I2CBUSNO 0 //I2C bus number 					///check
#define PROX_ADDR 0x51 //7 bit slave address (lsb 0=r, 1=w)

//Proximity sensor Registers
#define PROX_ALS_CONF 0x00 //Low
#define PROX_ALS_THRESH_HIGH 0x01 //Low + High (16 bit)
#define PROX_ALS_THRESH_LOW 0x02 //Low + High (16 bit)
#define PROX_PS_CONF1 0x03 //Low
#define PROX_PS_CONF2 0x03 //High
#define PROX_PS_CONF3 0x04 //Low
#define PROX_PS_MS 0x04 //High
#define PROX_PS_CANC 0x05 //Low + High (16 bit)
#define PROX_PS_THRESH_HIGH 0x06 //Low + High (16 bit)
#define PROX_PS_THRESH_LOW 0x07 //Low + High (16 bit)
#define PROX_PS_DATA 0x08 //Low + High (16 bit)
#define PROX_ALS_DATA 0x09 //Low + High (16 bit)

//Proximity sensor comand code values
#define CMD_PROX_START_ALS 0b01000110 //integration time 50ms, channel interrupt, interrupt persistance setting 2, interrupt enabled, ALS power on
#define CMD_PROX_START_PS1 0b01010000 //on/off duty ratio of 1/320, interrupt persistance setting 2, integration time 1T, PS power on
#define CMD_PROX_START_PS2 0b00001001 //reserved, PS output 16 bit, reserved, interrupt triggered by closing
#define CMD_PROX_START_PS3 0b00000001 //reserved, mulit pulse number 1, smart persistence disabled, active force mode disabled, no PS active force mode trigger
									  // typical sunlight immunity, sunlight cancellation function enabled
#define CMD_PROX_START_PSMS 0b00000010 //reserved, proximity normal operation /w interrupt function, typical sunlight capability, 00h in sunlihgt protect mode, LED current 100ma ///LEDcheck
#define CMD_PROX_START_PS12 ((CMD_PROX_START_PS2 << 8) | CMD_PROX_START_PS1)
#define CMD_PROX_START_PS3MS ((CMD_PROX_START_PSMS << 8) | CMD_PROX_START_PS3)
#define CMD_PROX_CANC 0x0000 //PS cancellation level set to 0

//Constants
#define ALS_SENS 0.024 //based on selected integration time, check table 14 of data sheet
#define ALS_MAX 1573 //based on selected integration time, check table 14 of data sheet


proximity::proximity(uint16_t ALSint_thresh_lowini, uint16_t ALSint_thresh_highini, uint16_t PSint_thresh_lowini, uint16_t PSint_thresh_highini){
	//Initialises GPIO libary and establishes I2C connection to proximity sensor
	int temp;
	if (gpioInitialise() < 0){ ///Potentially redundant
		std::cout << "Pigpio failed to initialise" << std::endl; //Possibile duplicate command, may already be initialised
		error = 1; //marks failure for gpio initialisation
	}else if ((temp = i2cOpen(PROX_I2CBUSNO, PROX_ADDR, 0)) < 0) {
		std::cout << "Proximity I2C read connection failed" << std::endl;
		error = 2; //marks failure for i2c connection failing
	}
	prox_2c = (unsigned) temp;
	if (writeLSB_Prox(PROX_ALS_CONF, CMD_PROX_START_ALS)){
		std::cout << "Proximity ALS config failure" << std::endl;
		error = 3; //marks failure for i2c configuration
	}else if (configALSthresh(ALSint_thresh_lowini, ALSint_thresh_highini)) {

	}else if (i2cWriteWordData(prox_i2c, PROX_PS_CONF1, CMD_PROX_START_PS12)){
		std::cout << "Proximity PS config failure 1-2" << std::endl;
		error = 6; //marks failure for PS configuration 1-2
	}else if (i2cWriteWordData(prox_i2c, PROX_PS_CONF3, CMD_PROX_START_PS3MS)){
		std::cout << "Proximty PS config failure 3-MS" << std::endl;
		error = 7; //marks failure for PS configuration 3-mode select
	}else if (i2cWriteWordData(prox_i2c, PROX_PS_CANC, CMD_PROX_CANC)){
		std::cout << "Proximity PS cancellation setting failure" << std::endl;
		error = 8; //marks failure for PS cancellation settings
	}else if (configPSthresh(PSint_thresh_lowini, PSint_thresh_highini)){

	}else error = 0; //marks not failure
}


int proximity::configALSthresh(uint16_t ALSint_thresh_low, uint16_t ALSint_thresh_high){
	if (i2cWriteWordData(prox_i2c, PROX_ALS_THRESH_HIGH, ALSint_thresh_high) < 0){
		std::cout << "Failure to update ALS interrupt high threshold" << std::endl;
		error = 4; //marks failure for setting ALS interrupt high threshold
		return 1;
	}else if (i2cWriteWordData(prox_i2c, PROX_ALS_THRESH_LOW, ALSint_thresh_low) < 0){
		std::cout << "Failure to update ALS interrupt low threshold" << std::endl;
		error = 5; //marks failure for setting ALS interrupt low threshold
		return 1;
	}
	return 0;
}


int proximity::configPSthresh(uint16_t PSint_thresh_low, uint16_t PSint_thresh_high){
	if (i2cWriteWordData(prox_i2c, PROX_PS_THRESH_HIGH, PSint_thresh_high) < 0){
		std::cout << "Failure to update PS interrupt high threshold" << std::endl;
		error = 10; //marks failure for setting PS interrupt high threshold
		return 1;
	}else if (i2cWriteWordData(prox_i2c, PROX_PS_THRESH_LOW, PSint_thresh_low) < 0){
		std::cout << "Failure to update PS interrupt low threshold" << std::endl;
		error = 11; //marks failure for setting PS interrupt low threshold
		return 1;
	}
	return 0;
}


int proximity::measureALS(){
	//Function to capture and store latest value of the ALS
	ALSval = i2cReadWordData(prox_i2c, PROX_ALS_DATA);
	if (!ALSval) {
		std::cout << "Failure to read ALS value" << std::endl;
		error = 12; //marks failure to read ALS DATA 
		return 1;
	}
	return 0;
}


int proximity::measurePS(){
	//Function to capture and store latest value of the PS
	PSval = i2cReadWordData(prox_i2c, PROX_PS_DATA);
	if (!PSval) {
		std::cout << "Failure to read PS value" << std::endl;
		error = 13; //marks failure to read PS DATA 
		return 1;
	}
	return 0;
}


int proximity::writeMSB_Prox(uint8_t reg, uint8_t MSB){
	//method to write to MSB of a register
	uint16_t temp;
	if ((temp = i2cReadWordData(prox_i2c, reg)) < 0){
		error = 96;
		return 1;
	}
	uint8_t LSB = temp & 0xFF;
	temp = (MSB << 8) | LSB;
	if (i2cWriteWordData(prox_i2c, reg, temp)){
		error = 97;
		return 2;
	}
	return 0;
}

int proximity::writeLSB_Prox(uint8_t reg, uint8_t LSB){
	//method to write to LSB of a register
	uint16_t temp;
	if ((temp = i2cReadWordData(prox_i2c, reg)) < 0){
		error = 98;
		return 1;
	}
	uint8_t MSB = temp >> 8;
	temp = (MSB << 8) | LSB;
	if (i2cWriteWordData(prox_i2c, reg, temp)){
		error = 99;
		return 2;
	}
	return 0;
}


proximity::~proximity(){
	//Closes GPIO libary and I2C connection to proximity sensor
	gpioTerminate();
	if (i2cClose(prox_i2c)) std::cout << "Error closing i2c read connection" << std::endl;
	if (i2cClose(prox_i2c)) std::cout << "Error closing i2c write connection" << std::endl;
}


/*
uint16_t proximity::read16(uint16_t comcode){ ///WRONG AS this is not 16 bit memory address
	//Code to read 16 bits from 16 bit memory address ///https://github.com/joan2937/pigpio/issues/301
	char buf[2];
	buf[0] = comcode >> 8;
	buf[1] = comcode & 0xFF;
	i2cWriteDevice(prox_i2c_r, buf, 2); ///uses read address to write, potential issue?
	i2cReadDevice(prox_i2c_r, buf, 2);
	return buf[0] << 8 | buf[1];
}
*/