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

#include "logic.hpp"
#include "mapstruct.h" //For the filereader
#include "qr.hpp" //For the QR code detection
#include "gsound.h" //For sound output

logic::logic(){
	//Initialiser
	proximity_logic = new proximity(1,0,250,0,250); ///Sample values currently //Input pin. ALS low threshold, ALS high threshold, PS low threshold, PS high threshold
	if(proximity_logic->interruptmode){
		if (proximity_logic->configinterrupt(proxdetection)) goto proxintfail;
	}else{
		proxintfail: std::cout << "Initiating non interruptmethod" << std::endl;
		//System restart most or polling code
	}
	
	
}


void logic::proxdetection(int gpio, int level, uint32_t tick){
	std::cout << "Object detected, stopping" << std::endl;
	m_stop();
	//Further stopping logic
}


void logic::callHelp() {
	//stop moving, movement module
	//send signal to help desk
	//bring 
	std::cout << "Calling helpdesk" << std::endl;
}


void logic::doTour(char *tourname){
	int num_waypoints = dev_read_csv(&tour, tourname);//Tour coordinates in 'tour' array
	int confirmer = 0;
	int info;
	VideoCapture cap(0);

	if(!cap.isOpened()){//Error call in case camera does not open
		cerr<< "ERROR! Unable to open Camera\n";
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
	cap.set(CV_CAP_PROP_FPS, 10);

	//get/calculate route, nav module
	//while (isTour){
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
				if((tour[i].dx_qr + 0.025) > qrcode.dx && qrcode.dx > (tour[i].dx_qr - 0.025)){
					if((tour[i].dy_qr + 0.025) > qrcode.dy && qrcode.dy > (tour[i].dy_qr - 0.025)){
						confirmer = 1;
					}
				}
			}
		}

		//say tour point
		info = giveInfo(i);
		std::cout << "Moving onto next tour point" << std::endl; //Placeholder to info programmer of exection and robots intentions
	}
}


void logic::emergencyStop(){
	//call movement stopping method
	//await UI confirmation 
	std::cout << "Executing emergency stop" << std::endl; //Placeholder to info programmer of exection and robots intentions
}


int logic::giveInfo(int locationID){//ONLY CALL THIS FROM THE doTour FUNCTION!!!THAT INITIALISES THIS!
	std::string pointInfo; //Container for information about a display // format may need to change
	//lookup location information with locationID
	pointInfo = "This is information about a location"; //Temporary placeholder, will replace with lookup on in memory
	if (pointInfo.empty()) {
		return 0; //returns 0 to indicate there was no information to display
	}//TODO - Remove pointInfo stuff
	//call audio out module with this input
	if (tour[i].sound_name == "N/A"){
		return 0; //indicates no sound file at location
	} else {
		sndcon(tour[i].sound_name);//TODO - check this
		std::cout << "Giving tour information" << std::endl; //Placeholder to info programmer of exection and robots intentions
		return 1; //returns 1 to indicate there was information to provide to user
	}
}


void logic::goHome(){
	//calcualte route to go home, nav module
	//UI display of going home
	//execute movement of robot to go home, movement module
	std::cout << "Going home" << std::endl; //Placeholder to info programmer of exection and robots intentions
}


void logic::stopTour(){
	isTour = false;
	std::cout << "Stopping tour" << std::endl; //Placeholder to info programmer of exection and robots intentions
}
