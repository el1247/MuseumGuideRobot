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
#include <pthread.h>
#include <unistd.h> 

#include "gpio_assign.h"
#include "gsound.h"
#include "movement.h"
#include "proximity.h"
#include "mapstruct.h"
#include "nav.h"
#include "qr.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio/videoio_c.h"
#include "zbar.h"

#include<QDebug>

class logic_qml : public QObject{
    Q_OBJECT
    private:
        proximity* proximity_logic; ///Commented out for frontend testings

        struct tourUpdateDataStruct{
            volatile int tourConfirms; //Marker for UI update when modifying tours
            char *message;
        };
        struct tourDataStruct{
            char *tourList[3]; //List of tours
            int current_location; //Current point in the tour ///set to 7 for test purposes, set to 0
            int currentTourID; //TourID passed from GUI
            int num_waypoints; //Number of waypoints that are in the loaded tour  ///set to 10 for test purposes, set to 0
            int totalTourCount; //Count of total tours
            bool isTour = false; //Tracks if a tour is in progress
            qr_Code qrcode;//instance of QR scanning class ///Commented out for frontend testings
            cv::Mat frame;//Instance of image object ///Commented out for frontend testings
            Waypoint *tour;//Instance of tour array pointer ///Commented out for frontend testings
            cv::VideoCapture* vCap;
        };

        tourUpdateDataStruct tourUpdateData;
        tourDataStruct tourData;

    public:
        char* stringOut = strdup("Hello. I am your museum guide robot."); //String storage to be output on the GUI

        logic_qml(); //Initialiser
        static void proxdetection(int gpio, int level, uint32_t tick);
        Q_INVOKABLE void callHelp();
        Q_INVOKABLE void emergencyStop();
        Q_INVOKABLE int giveInfo();
        Q_INVOKABLE void giveInfoAbout();
        Q_INVOKABLE void goNextTourPoint();
        Q_INVOKABLE void resumeMoving();
        Q_INVOKABLE void startTour(int tourID);
        Q_INVOKABLE void stopTour();
        Q_INVOKABLE void tourConfirmGUI();
        Q_INVOKABLE void tourUpdate();
        Q_INVOKABLE void tourWrite();
        Q_INVOKABLE int getlocation();
        Q_INVOKABLE int getTotalTourCount();
        Q_INVOKABLE QString speak();
        Q_INVOKABLE QString speakTour();
        Q_INVOKABLE QString getTourName(int tourID);

        //static void *goNextTourPointWork(void *tourDataIn);
        static void *goNextTourPointWork(void *tourDataIn);
        static void *stopTourWork(void *tourDataIn);
        static void *tourUpdateWork(void *tourUpdateDataIn);
};
extern void nav_callback();
#endif
