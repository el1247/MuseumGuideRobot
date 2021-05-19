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

#include "logic_qml.h"


logic_qml::logic_qml(){ //Initialiser
    tourUpdateData.tourConfirms = 1;
    //sprintf(tourUpdateData.message, "Message");

    tourData.current_location = 0;
    tourData.num_waypoints = 0;
    tourData.totalTourCount = 0;
    tourData.currentTourID = 0;

    proximity_logic = new proximity(1,0,250,0,250); ///Sample values currently //Input pin. ALS low threshold, ALS high threshold, PS low threshold, PS high threshold
    if(proximity_logic->interruptmode){
        if (proximity_logic->configinterrupt(proxdetection)) goto proxintfail;
    }else{
        proxintfail: std::cout << "Initiating non interruptmethod" << std::endl;
        //System restart most or polling code
    }

    DIR *dr; //Section scans directory for tours
    struct dirent *en;
    dr = opendir("."); //opens directory
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            if (strstr(en->d_name, ".csv") != NULL){ //search for .csv in file name
                tourList[totalTourCount] = new char[260];
                strncpy(tourList[totalTourCount], en->d_name, strlen(en->d_name)-4); //places name of found map in tourList, removes last 4 characters (.csv)
                totalTourCount++;
            }
        }
        closedir(dr); //closes directory
    }
}


void logic_qml::proxdetection(int gpio, int level, uint32_t tick){ ///TODO - test and decide upon further logic
    ///Nav pause?
    m_stop();
    //Further stopping logic
    //Reverse robot slightly?
    //Check sensors until it is safe, or wait a certain time period
    //If robot has steered too near an object 
    //resumeMoving(); //Commented for testing
    stringOut = "Object detected, stopping movement";
    //UI alertable?
}


void logic_qml::callHelp() { ///TODO - write
    stringOut = strdup("Kindly go fetch a member of staff from the help desk. I will be waiting here."); //configures string to be printed to GUI
    nav_cancel(); ///Cancel or pause? should call help pause the robot from moving?
}

static int waiting = 0;
static void wait_cb(void) { waiting = 0; }

void logic_qml::doTour(int tourID){ 
    tourData.currentTourID = tourID;
    pthread_t dotourThread;
    pthread_create(&dotourThread,NULL,doTourWork,(void *)&tourData);
}


void *logic_qml::doTourWork(void *tourDataIn){ //Legacy code, starts and loops through tour points
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;

    num_waypoints = dev_read_csv(&tour, tourList[tourDataStore.currentTourID]); //Tour coordinates in 'tour' array
    int confirmer = 0;
    int info, qr_data;
    float error_x, error_y;

    VideoCapture cap(0);

    if(!cap.isOpened()){//Error call in case camera does not open
        cerr<< "ERROR! Unable to open Camera\n";
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
    cap.set(CV_CAP_PROP_FPS, 10);

    //get/calculate route, nav module
    for(int i = 0; i < num_waypoints; i++){
	waiting = 1;
	nav_set_travel(tour[i].dx, tour[i].dy, &wait_cb);
	while(waiting) sleep(1);

        cap.read(frame);
        if (frame.empty()){//In case camera doesn't work
            cerr<<"ERROR! Blank frame grabbed!\n";
            break;
        }

        tourDataStore.current_location = i; //Updates internally stored location in tour point

        if(tour[i].qr == 1){//Checking the QR location if QR exists at the location
            while(confirmer == 0){
                decode(frame, qrcode);//Reading the image and getting the coordinates 
                error_x = 0.1*tour[i].dx_qr;//Permissable errors in the x and y deviations
                error_y = 0.1*tour[i].dy_qr;
                qr_data = stoi(qrcode.data);
                if(qr_data == tour[i].data){//Checking if the QR code is correct and at correct coordinates
                    if((tour[i].dx_qr + error_x) > qrcode.dx && qrcode.dx > (tour[i].dx_qr - error_x)){//TODO - make a robot move a bit if not at correct coordinates
                        if((tour[i].dy_qr + error_y) > qrcode.dy && qrcode.dy > (tour[i].dy_qr - error_y)){
                            confirmer = 1;//Destination reached
                            //TODO - Add stopping mechanisms
                        }
                    }
                }
            }
            //Provide information about the tour point if it exists
            info = giveInfo(i);//This will be 1 if there is info to be given out here, and 0 if no info
        }
        std::cout << "Moving onto next tour point" << std::endl; //Placeholder to info programmer of exection and robots intentions
    }
    pthread_exit(NULL);
}


void logic_qml::emergencyStop(){ //Stops robot from moving. Awaits robot to continue moving via UI prompt calling resumeMoving
    nav_cancel(); ///Cancel or pause?
    m_stop();
}


void logic_qml::giveInfo(){///TODO - Need to fix/test this 
    //Checks current location internally
    //call audio out module with this input
    if (tour[current_location].sound_name == "N/A"){
        stringOut = strdup("No information available for this point"); ///Better output available? This will be printed to GUI
        return 0; //indicates no sound file at location
    } else {
        sndcon(tour[current_location].sound_name);///TODO - check this
        stringOut = strdup("Giving information"); ///Better output available? This will be printed to GUI
        return 1; //returns 1 to indicate there was information to provide to user
    }
}


void logic_qml::giveInfoAbout(){
    stringOut = strdup("To start a tour please press the start tours button and then choose a tour.\n"
                    " I will guide you to a point in the tour.\nTo move to the next tour point, select next tour point.\n"
                    "We hope you enjoy your visit.");
}


void logic_qml::goNextTourPoint(){ 
    pthread_t tourThread;
    pthread_create(&tourThread,NULL,goNextTourPointWork,(void *)&tourData);
}


void *logic_qml::goNextTourPointWork(void *tourDataIn){ //Moves the robot to the next tour point
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;

    int confirmer = 0; //Stores if QR adjust has been executed
    int info = 0; //Stores if there is information for this tour point to be given
    int qr_data;
    float error_x, error_y;

    waiting = 1;
    nav_set_travel(tour[tourDataStore.current_location].dx, tour[tourDataStore.current_location.dy], &wait_cb);
    while(waiting) sleep(1);

    tourDataStore.current_location++; //Updates current_location

    cap.read(frame);
    if (frame.empty()){//In case camera doesn't work
        cerr<<"ERROR! Blank frame grabbed!\n";
        return -1; ///Should info still be given if camera doesn't function?
    }

    if(tour[current_location].qr == 1){//Checking the QR location if QR exists at the location
        while(confirmer == 0){
            decode(frame, qrcode);//Reading the image and getting the coordinates 
            error_x = 0.1*tour[tourDataStore.current_location].dx_qr;//Permissable errors in the x and y deviations
            error_y = 0.1*tour[tourDataStore.current_location].dy_qr;
            qr_data = stoi(qrcode.data);
            if(qr_data == tour[tourDataStore.current_location].data){//Checking if the QR code is correct and at correct coordinates
                if((tour[tourDataStore.current_location].dx_qr + error_x) > qrcode.dx && qrcode.dx > (tour[tourDataStore.current_location].dx_qr - error_x)){//TODO - make a robot move a bit if not at correct coordinates
                    if((tour[tourDataStore.current_location].dy_qr + error_y) > qrcode.dy && qrcode.dy > (tour[tourDataStore.current_location].dy_qr - error_y)){
                        confirmer = 1;//Destination reached
                        //TODO - Add stopping mechanisms
                    }
                }
            }
        }
        //Provide information about the tour point if it exists
        info = giveInfo(tourDataStore.current_location);//This will be 1 if there is info to be given out here, and 0 if no info
    }
    pthread_exit(NULL);
}


void logic_qml::resumeMoving(){ //Resumes the movement of the robot after emergencyStop is called
    ///TODO - Movement code to continue moving the robot
}


void logic_qml::startTour(int tourID){ //Loads tour data, opens camera
    tourData.currentTourID = tourID;
    num_waypoints = dev_read_csv(&tour, tourList[tourID]); //Tour coordinates in 'tour' array

    VideoCapture cap(0);

    if(!cap.isOpened()){//Error call in case camera does not open
        cerr<< "ERROR! Unable to open Camera\n";
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
    cap.set(CV_CAP_PROP_FPS, 10);

    tourData.current_location = 0; //Ensures current location is set to start of tour

    goNextTourPoint(); //Moves the robot to the first tour point
}


void logic_qml::stopTour(){ 
    pthread_t stopTourThread;
    pthread_create(&stopTourThread,NULL,stopTourWork,(void *)&tourData);
}

void *logic_qml::stopTourWork(void *tourDataIn){ //Stops the tour and returns the robot home
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;

    tourDataStore.isTour = false;

    //calculate route to go home, nav module
    int confirmer = 0;
    int info, qr_data;
    float error_x, error_y;

    VideoCapture cap(0);

    if(!cap.isOpened()){//Error call in case camera does not open
        cerr<< "ERROR! Unable to open Camera\n";
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
    cap.set(CV_CAP_PROP_FPS, 10);

    //get/calculate route, nav module
    if(tourDataStore.current_location > tourDataStore.num_waypoints/2){
        for(int i = tourDataStore.current_location; i < tourDataStore.num_waypoints; i++){
            //TODO - move robot to tour point indicated by tour[i].dx and tour[i].dy
            //TODO - wait to reach waypoint
            cap.read(frame);
            if (frame.empty()){//In case camera doesn't work
                cerr<<"ERROR! Blank frame grabbed!\n";
                break;
            }

            if(tour[i].qr == 1){//Checking the QR location if QR exists at the location
                while(confirmer == 0){
                    decode(frame, qrcode);//Reading the image and getting the coordinates 
                    error_x = 0.1*tour[i].dx_qr;//Permissible errors for the x and y coordinates
                    error_y = 0.1*tour[i].dy_qr;
                    qr_data = stoi(qrcode.data);
                    if(qr_data == tour[i].data){//Checking whether QR code data is correct
                        if((tour[i].dx_qr + error_x) > qrcode.dx && qrcode.dx > (tour[i].dx_qr - error_x)){
                            if((tour[i].dy_qr + error_y) > qrcode.dy && qrcode.dy > (tour[i].dy_qr - error_y)){
                                confirmer = 1;//Destination reached
                                //TODO - Add stopping mechanisms
                            }
                        }
                    }
                }
            }
        }
    } else {
        for(int i = tourDataStore.current_location; i >= 0; --i){
            //TODO - move robot to tour point indicated by tour[i].dx and tour[i].dy
            //TODO - wait to reach waypoint
            cap.read(frame);
            if (frame.empty()){//In case camera doesn't work
                cerr<<"ERROR! Blank frame grabbed!\n";
                break;
            }

            if(tour[i].qr == 1){//Checking the QR location if QR exists at the location
                while(confirmer == 0){
                    decode(frame, qrcode);//Reading the image and getting the coordinates 
                    error_x = 0.1*tour[i].dx_qr;
                    error_y = 0.1*tour[i].dy_qr;
                    qr_data = stoi(qrcode.data);
                    if(qr_data == tour[i].data){
                        if((tour[i].dx_qr + error_x) > qrcode.dx && qrcode.dx > (tour[i].dx_qr - error_x)){
                            if((tour[i].dy_qr + error_y) > qrcode.dy && qrcode.dy > (tour[i].dy_qr - error_y)){
                                confirmer = 1;//Destination reached
                                //TODO - Add stopping mechanisms
                            }
                        }
                    }
                }
            }
        std::cout << "Moving onto next tour point" << std::endl; //Placeholder to info programmer of exection and robots intentions
        }
    }
    pthread_exit(NULL);
}


void logic_qml::tourConfirmGUI(){///TODO take GUI input and store
    tourUpdateData.tourConfirms = 0;
}


void logic_qml::tourUpdate(){
    pthread_t tourUpdateThread;
    tourUpdateData.tourConfirms = 1;
    pthread_create(&tourUpdateThread,NULL,tourUpdateWork,(void *)&tourUpdateData);
}


void *logic_qml::tourUpdateWork(void *tourUpdateDataIn){ ///TODO: Convert code from write_csv() into here, confirm frontend connection
    struct tourUpdateDataStruct *tourUpdateDataStore;
    tourUpdateDataStore = (struct tourUpdateDataStruct *) tourUpdateDataIn;
    while(tourUpdateDataStore->tourConfirms) usleep(50000);
    //sprintf(tourUpdateDataStore->message, "JJJJJJ");
    pthread_exit(NULL);
}

void logic_qml::tourWrite(){ ///TODO: Links to update_csv() in filereader.c, frontend support needed
    qInfo("Tour Write called");
}


int logic_qml::getlocation(){ //Returns the current location of the robot in tour index point. If the robot is on the last point of the tour 255 is returned
    if (tourData.current_location < tourData.num_waypoints) {
        return tourData.current_location;
    } else {
        return 255; //Indicates that the robot is on the last tour point
    }
}


int logic_qml::getTotalTourCount(){ //Returns the total number of unique tours identified upon initialisation
    return tourData.totalTourCount;
}


QString logic_qml::speak(){ //Converts stringOut value to Qstring for GUI  to call
    QString wordy;
    wordy = stringOut;
    return wordy;
}


QString logic_qml::speakTour(){
    QString wordy;
    wordy = tourUpdateData.message;
    return wordy;
}


QString logic_qml::getTourName(int tourID){ //Gets name of indexed tour
    qInfo("Getting tour name");
    QString tourname;
    if (tourID < totalTourCount){
        tourname = tourList[tourID];
    }else{
        tourname = QString("Tour %1").arg(tourID);
    }
    return tourname;
}
