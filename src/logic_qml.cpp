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
#include "nav.h"


logic_qml::logic_qml(){ //Initialiser
    tourUpdateData.tourConfirms = 1;
    //sprintf(tourUpdateData.message, "Message");

    tourData.current_location = 0;
    tourData.num_waypoints = 0;
    tourData.totalTourCount = 0;
    tourData.currentTourID = 0;

    proximity_logic = new proximity(5,25,1537,30,1500); ///Sample values currently //Input pin. ALS low threshold, ALS high threshold, PS low threshold, PS high threshold
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
                tourData.tourList[tourData.totalTourCount] = new char[260];
                strncpy(tourData.tourList[tourData.totalTourCount], en->d_name, strlen(en->d_name)-4); //places name of found map in tourList, removes last 4 characters (.csv)
                tourData.totalTourCount++;
            }
        }
        closedir(dr); //closes directory
    }

    tourData.vCap = new cv::VideoCapture(0);

    if(!tourData.vCap->isOpened()){//Error call in case camera does not open
        std::cout << "ERROR! Unable to open Camera" << std::endl;
    }else {
        tourData.vCap->set(CV_CAP_PROP_FRAME_WIDTH, 2592);
        tourData.vCap->set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
        tourData.vCap->set(CV_CAP_PROP_FPS, 10);
    }

    std::cout << "Logic_qml init finished" << std::endl;
}


void logic_qml::proxdetection(int gpio, int level, uint32_t tick){ //Interrupt function to be called by proximity sensor
    //nav_cancel();
    std::cout << "Object detected, stopping movement" << std::endl;
    sleep(1);
    //nav_resume();
}


void logic_qml::callHelp() { //Stops the robot and informs user to fetch a member of staff
    //nav_cancel();
    stringOut = strdup("Kindly go fetch a member of staff from the help desk. I will be waiting here."); //configures string to be printed to GUI
}

static int waiting = 0;
static void wait_cb(void) { waiting = 0; }
static int dtwi = 0;


void logic_qml::emergencyStop(){ //Stops robot from moving. Awaits robot to continue moving via UI prompt calling resumeMoving
    //nav_cancel(); ///Cancel
}


int logic_qml::giveInfo(){//Gives information about tour point
    //call audio out module with this input
#if 1
    if (!strncmp(tourData.tour[tourData.current_location].sound_name, "N/A", 3)){
        stringOut = strdup("No information available for this point"); ///Better output available? This will be printed to GUI
        return 0; //indicates no sound file at location
    } else {
        sndcon(tourData.tour[tourData.current_location].sound_name);
        sprintf(stringOut, "Information at point %d",tourData.current_location);
        return 1; //returns 1 to indicate there was information to provide to user
    }
#endif
    sprintf(stringOut, "Information at point %d", tourData.current_location);
    return 0;
}


void logic_qml::giveInfoAbout(){
    stringOut = strdup("To start a tour please press the start tours button and then choose a tour.\n"
                    " I will guide you to a point in the tour.\nTo move to the next tour point, select next tour point.\n"
                    "We hope you enjoy your visit.");
}


void logic_qml::goNextTourPoint(){ 
    pthread_t goNextTourPointThread;
    pthread_create(&goNextTourPointThread,NULL,goNextTourPointWork,(void *)&tourData);
}


void *logic_qml::goNextTourPointWork(void *tourDataIn){ //Moves the robot to the next tour point
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;

    tourDataStore->isTour = true;

    int confirmer = 0; //Stores if QR adjust has been executed
    int info = 0; //Stores if there is information for this tour point to be given
    int qr_data;
    float error_x, error_y;

    if (tourDataStore->current_location < tourDataStore->num_waypoints){
        //nav_set_travel(tourDataStore->tour[tourDataStore->current_location + 1].dx, tourDataStore->tour[tourDataStore->current_location + 1].dy, &nav_callback);

        tourDataStore->current_location++; //Updates current_location

        tourDataStore->vCap->read(tourDataStore->frame);
        if (tourDataStore->frame.empty()){//In case camera doesn't work
            //cerr<<"ERROR! Blank frame grabbed!\n";
        }else if(tourDataStore->tour[tourDataStore->current_location].qr == 1){//Checking the QR location if QR exists at the location
            while(confirmer == 0){
                decode(tourDataStore->frame, tourDataStore->qrcode);//Reading the image and getting the coordinates
                error_x = 0.1*tourDataStore->tour[tourDataStore->current_location].dx_qr;//Permissable errors in the x and y deviations
                error_y = 0.1*tourDataStore->tour[tourDataStore->current_location].dy_qr;
                qr_data = std::stoi(tourDataStore->qrcode.data);
                if(qr_data == tourDataStore->tour[tourDataStore->current_location].data){//Checking if the QR code is correct and at correct coordinates
                    if((tourDataStore->tour[tourDataStore->current_location].dx_qr + error_x) > tourDataStore->qrcode.dx && tourDataStore->qrcode.dx > (tourDataStore->tour[tourDataStore->current_location].dx_qr - error_x)){//TODO - make a robot move a bit if not at correct coordinates
                        if((tourDataStore->tour[tourDataStore->current_location].dy_qr + error_y) > tourDataStore->qrcode.dy && tourDataStore->qrcode.dy > (tourDataStore->tour[tourDataStore->current_location].dy_qr - error_y)){
                            confirmer = 1;//Destination reached
                            //m_stop();
                        }
                    }
                }
            }
            //Provide information about the tour point if it exists
        }
    }else{
        stopTourWork(tourDataIn);
    }
}


void logic_qml::resumeMoving(){ //Resumes the movement of the robot after emergencyStop is called
    //nav_resume();
}


void logic_qml::startTour(int tourID){ //Loads tour data, opens camera
    //nav_cancel(); //Stops call current movements
    tourData.currentTourID = tourID;
    std::cout << "Tour selected: "<< tourData.tourList[tourID] << std::endl;
    tourData.num_waypoints = dev_read_csv(&tourData.tour, tourData.tourList[tourID]); //Tour coordinates in 'tour' array

    tourData.num_waypoints = 5;
    //tourData.tour[0].qr = 0;
    tourData.tour[0].sound_name = strdup("N/A");
    tourData.tour[1].sound_name = strdup("Tourpoint1.wav");
    tourData.tour[2].sound_name = strdup("tourpoint2.wav");
    tourData.tour[3].sound_name = strdup("tourpoint3.wav");
    tourData.tour[4].sound_name = strdup("tourpoint4.wav");
    tourData.tour[5].sound_name = strdup("tourpoint5.wav");

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

    tourDataStore->isTour = false;

    int confirmer = 0;
    int info, qr_data;
    float error_x, error_y;

    if(tourDataStore->current_location > tourDataStore->num_waypoints/2){
        for(int i = tourDataStore->current_location; i < tourDataStore->num_waypoints; i++){
            //nav_set_travel(tourDataStore->tour[i].dx, tourDataStore->tour[i].dy, &nav_callback);
            //wait for arrival

            tourDataStore->vCap->read(tourDataStore->frame);
            if (tourDataStore->frame.empty()){//In case camera doesn't work
                //cerr<<"ERROR! Blank frame grabbed!\n";
                break;
            }

            if(tourDataStore->tour[i].qr == 1){//Checking the QR location if QR exists at the location
                while(confirmer == 0){
                    decode(tourDataStore->frame, tourDataStore->qrcode);//Reading the image and getting the coordinates
                    error_x = 0.1*tourDataStore->tour[i].dx_qr;//Permissible errors for the x and y coordinates
                    error_y = 0.1*tourDataStore->tour[i].dy_qr;
                    qr_data = stoi(tourDataStore->qrcode.data);
                    if(qr_data == tourDataStore->tour[i].data){//Checking whether QR code data is correct
                        if((tourDataStore->tour[i].dx_qr + error_x) > tourDataStore->qrcode.dx && tourDataStore->qrcode.dx > (tourDataStore->tour[i].dx_qr - error_x)){
                            if((tourDataStore->tour[i].dy_qr + error_y) > tourDataStore->qrcode.dy && tourDataStore->qrcode.dy > (tourDataStore->tour[i].dy_qr - error_y)){
                                confirmer = 1;//Destination reached
                                //m_stop();
                            }
                        }
                    }
                }
            }
        }
    } else {
        for(int i = tourDataStore->current_location; i >= 0; --i){
            //nav_set_travel(tourDataStore->tour[i].dx, tourDataStore->tour[i].dy, &nav_callback);
            //wait for arrival

            tourDataStore->vCap->read(tourDataStore->frame);
            if (tourDataStore->frame.empty()){//In case camera doesn't work
                //cerr<<"ERROR! Blank frame grabbed!\n";
                break;
            }

            if(tourDataStore->tour[i].qr == 1){//Checking the QR location if QR exists at the location
                while(confirmer == 0){
                    decode(tourDataStore->frame, tourDataStore->qrcode);//Reading the image and getting the coordinates
                    error_x = 0.1*tourDataStore->tour[i].dx_qr;
                    error_y = 0.1*tourDataStore->tour[i].dy_qr;
                    qr_data = stoi(tourDataStore->qrcode.data);
                    if(qr_data == tourDataStore->tour[i].data){
                        if((tourDataStore->tour[i].dx_qr + error_x) > tourDataStore->qrcode.dx && tourDataStore->qrcode.dx > (tourDataStore->tour[i].dx_qr - error_x)){
                            if((tourDataStore->tour[i].dy_qr + error_y) > tourDataStore->qrcode.dy && tourDataStore->qrcode.dy > (tourDataStore->tour[i].dy_qr - error_y)){
                                confirmer = 1;//Destination reached
                                //m_stop();
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
    //sprintf(tourUpdateDataStore->message, "Enter updated tour name");
    pthread_exit(NULL);
}

void logic_qml::tourWrite(){ ///TODO: Links to update_csv() in filereader.c, frontend support needed
    pthread_t tourWriteThread;
    tourUpdateData.tourConfirms = 1;
    pthread_create(&tourWriteThread,NULL,tourWriteWork,(void *)&tourUpdateData);
}


void *logic_qml::tourWriteWork(void *tourUpdateDataIn){ ///TODO: Convert code from write_csv() into here, confirm frontend connection
    struct tourUpdateDataStruct *tourUpdateDataStore;
    tourUpdateDataStore = (struct tourUpdateDataStruct *) tourUpdateDataIn;
    while(tourUpdateDataStore->tourConfirms) usleep(50000);
    //sprintf(tourUpdateDataStore->message, "Enter new tour name");
    pthread_exit(NULL);
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
    QString tourname;
    if (tourID < tourData.totalTourCount){
        tourname = tourData.tourList[tourID];
    }else{
        tourname = QString("Tour %1").arg(tourID);
    }
    return tourname;
}

void nav_callback(){
    //empty function for nav to callback
}
