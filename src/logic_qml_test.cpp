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


void logic_qml::giveInfo(){//TODO - Need to fix/test this
    //Checks current location internally
    qInfo("Give info called");
    mystring = strdup("Information stored");
}


void logic_qml::goHome(int locationID){ //Possibly delete and append to stoptour
    qInfo("Go home called");
}


void logic_qml::goNextTourPoint(){
    qInfo("Next tour point called");
}

void logic_qml::stopTour(){ //Stops the tour
   qInfo("Stoptour called");
}

QString logic_qml::speak(){
    QString wordy;
    wordy = mystring;
    return wordy;
}

QString logic_qml::getTourName(int tourID){ //Method to get names of tours
    qInfo("Getting tour name");
    QString tourname;
    //get tourname
    tourname = QString("Tour %1").arg(tourID);
    return tourname;
}
