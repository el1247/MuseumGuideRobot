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
	//lookup location information with locationID
	pointInfo = "This is information about a location"; //Temporary placeholder, will replace with lookup on in memory
	if pointInfo.empty(){
		return 0;
	}
	//call audio out module with this input
	cout << "Giving tour information" << endl; //Placeholder to info programmer of exection and robots intentions
	return 1;
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