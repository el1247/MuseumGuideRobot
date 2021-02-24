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

#include "logic.h"


logic::logic(){
	//Initialiser
	proximity_logic = new proximity(1,1,1,250,1,250); ///Sample values currently
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


void logic::doTour(int tourID){
	//lookup tour using tourID
	//get/calculate route, nav module
	while (isTour){
		//move robot to next tour point
		//say tour point
		//get information for next tour point
		std::cout << "Moving onto next tour point" << std::endl; //Placeholder to info programmer of exection and robots intentions
	} 
}


void logic::emergencyStop(){
	//call movement stopping method
	//await UI confirmation 
	std::cout << "Executing emergency stop" << std::endl; //Placeholder to info programmer of exection and robots intentions
}


int logic::giveInfo(int locationID){
	std::string pointInfo; //Container for information about a display // format may need to change
	//lookup location information with locationID
	pointInfo = "This is information about a location"; //Temporary placeholder, will replace with lookup on in memory
	if (pointInfo.empty()) {
		return 0; //returns 0 to indicate there was no information to display
	}
	//call audio out module with this input
	std::cout << "Giving tour information" << std::endl; //Placeholder to info programmer of exection and robots intentions
	return 1; //returns 1 to indicate there was information to provide to user
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