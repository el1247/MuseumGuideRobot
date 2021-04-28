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


#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <iostream>
#include <string>
#include "movement.h"
#include "proximity.h"
#include "mapstruct.h"
#include "qr.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio/videoio_c.h"
#include "zbar.h"
//#include "navigation.h"

class logic {
private:
	bool isTour = false; //Tracks if a tour is in progress
	proximity* proximity_logic;
	//instance of movement class
	//instance of navigation class
	qr_Code qrcode;//instance of QR scanning class
	Mat frame;//Instance of image object
	Waypoint *tour;//Instance of tour array pointer
	logic(); //Initialiser - DOES THIS NEED TO BE PUBLIC?
	static void proxdetection(int gpio, int level, uint32_t tick);

public:
	void callHelp();
	void doTour(char *tourname);
	void emergencyStop();
	int giveInfo(int locationID);
	void goHome();
	void stopTour();
};

#endif
