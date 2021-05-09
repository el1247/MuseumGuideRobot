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


#ifndef LOGIC_QML_H
#define LOGIC_QML_H

#include <QObject>

#include <iostream>
#include <string>
#include "gpio_assign.h"
#include "gsound.h"
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

class logic_qml : public QObject{
    Q_OBJECT {

private:
	bool isTour = false; //Tracks if a tour is in progress 
	//Commented out for frontend testings
	//proximity* proximity_logic;
	//instance of movement class
	//instance of navigation class
	//qr_Code qrcode;//instance of QR scanning class
	//Mat frame;//Instance of image object
	//Waypoint *tour;//Instance of tour array pointer
	int num_waypoints;//Number of waypoints that are in a specific tour

public:
    logic(); //Initialiser
    static void proxdetection(int gpio, int level, uint32_t tick);
	Q_INVOKABLE void callHelp();
	Q_INVOKABLE void doTour(char *tourname);
	Q_INVOKABLE void emergencyStop();
	Q_INVOKABLE int giveInfo(int locationID);
	Q_INVOKABLE void goHome();
	Q_INVOKABLE void stopTour();
	Q_INVOKABLE QString speak();
};

#endif