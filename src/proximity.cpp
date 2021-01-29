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

void proximity::measuredist(){
	//Function to capture and store latest value from proximity sensor
	//measure distance
}

proximity::proximity(){
	if (gpioInitialise() < 0){
		std::cout << "Pigpio failed to initiaise" << std::endl; //Possibile duplicate command, may already be initialised
	}
	//Start pigpio
	//Identify I2C connection
	//Start I2C connection

}