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
    qInfo("Initialiser");

    tourUpdateData.tourConfirms = 1;
    //sprintf(tourUpdateData.message, "Message");

    tourData.current_location = 7;
    tourData.num_waypoints = 10;
    tourData.totalTourCount = 0;
    tourData.currentTourID = 0;

    DIR *dr; //Section scans directory for tours
    struct dirent *en;
    dr = opendir("."); //open all or present directory
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            if (!strcmp(en->d_name, "*.csv")){ //literal string match, no wildcards, does not work
                qInfo("CSV file found");
            }
            if (strstr(en->d_name, ".csv") != NULL){ //search for .csv in file name
                tourData.tourList[tourData.totalTourCount] = new char[260];
                strncpy(tourData.tourList[tourData.totalTourCount], en->d_name, strlen(en->d_name)-4); //places name of found map in tourList
                tourData.totalTourCount++;
                qInfo("%s",tourData.tourList[tourData.totalTourCount - 1]);
            }
        }
        closedir(dr); //close all directory
    }
}


void logic_qml::proxdetection(int gpio, int level, uint32_t tick){
    qInfo("%d %d %d", gpio, level, tick);
}


void logic_qml::callHelp() {
    stringOut = strdup("Please go fetch a member of staff from the front desk.");
    qInfo("Calling helpdesk called");
}


void logic_qml::doTour(int tourID){
    tourData.currentTourID = tourID;
    pthread_t dotourThread;
    pthread_create(&dotourThread,NULL,doTourWork,(void *)&tourData);
}


void *logic_qml::doTourWork(void *tourDataIn){
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;
    qInfo("Do tour called, tourID  %d", tourDataStore->currentTourID);
    pthread_exit(NULL);
}


void logic_qml::emergencyStop(){
    stringOut = strdup("Stopping robot.");
    qInfo("Emergency stop called");
}


void logic_qml::giveInfo(){//TODO - Need to fix/test this
    //Checks current location internally
    qInfo("Give info called");
    sprintf(stringOut, "Information stored in location %d", tourData.current_location);
    //stringOut = strdup("Information stored " + locationID);
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


void *logic_qml::goNextTourPointWork(void *tourDataIn){
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;
    tourDataStore->current_location++;
    qInfo("Next tour point called");
    pthread_exit(NULL);
}


void logic_qml::resumeMoving(){ //Resumes the movement of the robot
    qInfo("ResumeMoving called");
}


void logic_qml::startTour(int tourID){
    tourData.currentTourID = tourID;
    qInfo("Starttour called, tourID %d", tourID);
    logic_qml::goNextTourPoint();
}


void logic_qml::stopTour(){ //Stops the tour
    pthread_t stopTourThread;
    pthread_create(&stopTourThread,NULL,stopTourWork,(void *)&tourData);
}

void *logic_qml::stopTourWork(void *tourDataIn){
    struct tourDataStruct *tourDataStore;
    tourDataStore = (struct tourDataStruct *) tourDataIn;
    qInfo("Stop tour called. TourID %d",tourDataStore->currentTourID);
    pthread_exit(NULL);
}


void logic_qml::tourConfirmGUI(){
    tourUpdateData.tourConfirms = 0;
    qInfo("tourConfirmGUI sent");
}


void logic_qml::tourUpdate(){
    pthread_t tourUpdateThread;
    tourUpdateData.tourConfirms = 1;
    pthread_create(&tourUpdateThread,NULL,tourUpdateWork,(void *)&tourUpdateData);
}

void *logic_qml::tourUpdateWork(void *tourUpdateDataIn){
    struct tourUpdateDataStruct *tourUpdateDataStore;
    tourUpdateDataStore = (struct tourUpdateDataStruct *) tourUpdateDataIn;
    qInfo("Tour Update called");
    while(tourUpdateDataStore->tourConfirms) usleep(50000);
    qInfo("Post sleep");
    //sprintf(tourUpdateDataStore->message, "JJJJJJ");
    qInfo("Successful update");
    pthread_exit(NULL);
}


void logic_qml::tourWrite(){
    qInfo("Tour Write called");
}


int logic_qml::getlocation(){
    qInfo("Location point %d",tourData.current_location);
    if (tourData.current_location < tourData.num_waypoints) {
        return tourData.current_location;
    } else {
        return 255; //Indicates that the robot is on the last tour point
    }
}


int logic_qml::getTotalTourCount(){
    return tourData.totalTourCount;
}


QString logic_qml::speak(){
    QString wordy;
    wordy = stringOut;
    return wordy;
}


QString logic_qml::speakTour(){
    QString wordy;
    wordy = tourUpdateData.message;
    return wordy;
}


QString logic_qml::getTourName(int tourID){ //Method to get names of tours
    qInfo("Getting tour name");
    QString tourname;
    if (tourID < tourData.totalTourCount){
        tourname = tourData.tourList[tourID];
    }else{
        tourname = QString("Tour %1").arg(tourID);
    }
    return tourname;
}
