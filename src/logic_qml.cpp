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


logic_qml::logic_qml(){
    //Initialiser
    proximity_logic = new proximity(1,0,250,0,250); ///Sample values currently //Input pin. ALS low threshold, ALS high threshold, PS low threshold, PS high threshold
    if(proximity_logic->interruptmode){
        if (proximity_logic->configinterrupt(proxdetection)) goto proxintfail;
    }else{
        proxintfail: std::cout << "Initiating non interruptmethod" << std::endl;
        //System restart most or polling code
    }

    DIR *dr; //Section scans directory for tours
    struct dirent *en;
    dr = opendir("."); //open all or present directory
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            if (!strcmp(en->d_name, "*.csv")){ //literal string match, no wildcards, does not work
                qInfo("CSV file found");
            }
            if (strstr(en->d_name, ".csv") != NULL){ //search for .csv in file name
                mapList[totalTourCount] = new char[260];
                strcpy(mapList[totalTourCount], en->d_name); //places name of found map in mapList
                totalTourCount++;
            }
        }
        closedir(dr); //close all directory
    }
}


void logic_qml::proxdetection(int gpio, int level, uint32_t tick){
    std::cout << "Object detected, stopping" << std::endl;
    m_stop();
    //Further stopping logic
}


void logic_qml::callHelp() {
    mystring = strdup("Calling help desk function called"); //configures string to be printed to GUI
    //stop moving, movement module
    //send signal to help desk
    //bring 
    std::cout << "Calling helpdesk" << std::endl;
}


void logic_qml::doTour(int tourID){ ///need to find a way to get tourname, currently using tourID
    num_waypoints = dev_read_csv(&tour, tourname);//Tour coordinates in 'tour' array
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
}


void logic_qml::emergencyStop(){
    //call movement stopping method
    //await UI confirmation 
    std::cout << "Executing emergency stop" << std::endl; //Placeholder to info programmer of exection and robots intentions
}


void logic_qml::giveInfo(int locationID){//TODO - Need to fix/test this ///Need to add location ID parameter and check this with logic_qml_test
    //Checks current location internally
    //call audio out module with this input
    if (tour[locationID].sound_name == "N/A"){
        return 0; //indicates no sound file at location
    } else {
        sndcon(tour[locationID].sound_name);//TODO - check this
        std::cout << "Giving tour information" << std::endl; //Placeholder to info programmer of exection and robots intentions
        return 1; //returns 1 to indicate there was information to provide to user
    }
    mystring = strdup("Information stored");
}


void logic_qml::goHome(int locationID){ //Possibly delete and append to stoptour
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
    if(locationID > num_waypoints/2){
        for(int i = locationID; i < num_waypoints; i++){
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
        for(int i = locationID; i >= 0; --i){
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


void logic_qml::goNextTourPoint(){
    qInfo("Next tour point called");
}


void logic_qml::stopTour(){ //Stops the tour
    isTour = false;
    std::cout << "Stopping tour" << std::endl; //Placeholder to info programmer of exection and robots intentions
}


int logic_qml::getlocation(){
    return current_location;
}


QString logic_qml::speak(){
    QString wordy;
    wordy = mystring;
    return wordy;
}


QString logic_qml::getTourName(int tourID){ //Method to get names of tours
    qInfo("Getting tour name");
    QString tourname;
    if (tourID < totalTourCount){
        tourname = mapList[tourID];
    }else{
        tourname = QString("Tour %1").arg(tourID);
    }
    return tourname;
}