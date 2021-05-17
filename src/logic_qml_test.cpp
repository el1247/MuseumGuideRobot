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
                strncpy(mapList[totalTourCount], en->d_name, strlen(en->d_name)-4); //places name of found map in mapList
                totalTourCount++;
                qInfo(mapList[totalTourCount - 1]);
            }
        }
        closedir(dr); //close all directory
    }
}


void logic_qml::proxdetection(int gpio, int level, uint32_t tick){
    qInfo("");
}


void logic_qml::callHelp() {
    mystring = strdup("Calling help desk function called");
    qInfo("Calling helpdesk called");
}


void logic_qml::doTour(int tourID){
    qInfo("Do tour called");
}


void logic_qml::emergencyStop(){
    qInfo("Emergency stop called");
}


void logic_qml::giveInfo(int locationID){//TODO - Need to fix/test this
    //Checks current location internally
    qInfo("Give info called");
    sprintf(stringOut, "Information stored in location %d", locationID);
    //mystring = strdup("Information stored " + locationID);
}


void logic_qml::goNextTourPoint(){
    qInfo("Next tour point called");
}


void logic_qml::stopTour(int locationID){ //Stops the tour
   qInfo("Stoptour called");
}


int logic_qml::getlocation(){
    return current_location;
}


int logic_qml::getTotalTourCount(){
    return totalTourCount;
}


QString logic_qml::speak(){
    QString wordy;
    wordy = stringOut;
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
