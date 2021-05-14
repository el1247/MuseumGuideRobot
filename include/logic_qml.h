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
#include <QTextStream>

#include <iostream>
#include <string>
#include <filesystem>
#include <string.h>
#include "dirent.h"

//#include "gpio_assign.h"
//#include "gsound.h"
//#include "movement.h"
//#include "proximity.h"
//#include "mapstruct.h"
//#include "qr.hpp"
//#include "opencv2/core.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/videoio/videoio_c.h"
//#include "zbar.h"
//#include "navigation.h"

#include<QDebug>

class logic_qml : public QObject{
    Q_OBJECT
    private:
        bool isTour = false; //Tracks if a tour is in progress
        //Commented out for frontend testings
        //proximity* proximity_logic;
        //instance of movement class
        //instance of navigation class
        //qr_Code qrcode;//instance of QR scanning class
        //Mat frame;//Instance of image object
        //Waypoint *tour;//Instance of tour array pointer
        int current_location = 7; //set to 7 for test purposes, set to zero
        int num_waypoints;//Number of waypoints that are in a specific tour
        char *mapList[3]; //List of maps/tours 
        int totalTourCount = 0; //Count of total tours

    public:
        char* mystring = strdup("abc");
        logic_qml(); //Initialiser
        static void proxdetection(int gpio, int level, uint32_t tick);
        Q_INVOKABLE void callHelp();
        Q_INVOKABLE void doTour(int tourID);
        Q_INVOKABLE void emergencyStop();
        Q_INVOKABLE void giveInfo(int locationID);
        Q_INVOKABLE void goHome(int locationID);
        Q_INVOKABLE void goNextTourPoint();
        Q_INVOKABLE void stopTour();
        Q_INVOKABLE int getlocation();
        Q_INVOKABLE QString speak();
        Q_INVOKABLE QString getTourName(int tourID);
};

#endif
