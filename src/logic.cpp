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

void logic::callHelp() {
	//stop moving, movement module
	//send signal to help desk
	//bring 
	cout << "Calling helpdesk" << endl;
}


void logic::doTour(int tourID){
	//lookup tour using tourID
	//get/calculate route, nav module
	while (isTour){
		//move robot to next tour point
		//say tour point
		//get information for next tour point
		cout << "Moving onto next tour point" << endl; //Placeholder to info programmer of exection and robots intentions
	} 
}


void logic::emergencyStop(){
	//call movement stopping method
	//await UI confirmation 
	cout << "Executing emergency stop" << endl; //Placeholder to info programmer of exection and robots intentions
}


int logic::giveInfo(int locationID){
	string pointInfo; //Container for information about a display // format may need to change
	//lookup location information with locationID
	pointInfo = "This is information about a location"; //Temporary placeholder, will replace with lookup on in memory
	if pointInfo.empty(){
		return 0; //returns 0 to indicate there was no information to display
	}
	//call audio out module with this input
	cout << "Giving tour information" << endl; //Placeholder to info programmer of exection and robots intentions
	return 1; //returns 1 to indicate there was information to provide to user
}


void logic::goHome(){
	//calcualte route to go home, nav module
	//UI display of going home
	//execute movement of robot to go home, movement module
	cout << "Going home" << endl; //Placeholder to info programmer of exection and robots intentions
}


void logic::stopTour(){
	isTour = False;
	cout << "Stopping tour" << endl; //Placeholder to info programmer of exection and robots intentions
}