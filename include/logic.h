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


#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <string>
#include "movement.h"
//#include "navigation.h"

class logic {
private:
	bool isTour = false; //Tracks if a tour is in progress
	//instance of movement class
	//instance of navigation class
	//instance of QR scanning class
	
public:
	void callHelp();
	void doTour(int tourID);
	void emergencyStop();
	int giveInfo(int locationID);
	void goHome();
	void stopTour();
};

#endif