#pragma once
#include <string>
#include "movement.h"
#include "navigation.h"


class logic {
private:
	bool isTour = false; //Tracks if a tour is in progress
	string pointInfo; //Container for information about a display // format may need to change
	//instance of movement class
	//instance of navigation class
	//instance of QR scanning class
	
public:
	void callHelp();
	void doTour(int tourID);
	void emergencyStop();
	int giveInfo(int locationID);
	void gohome();
	void stopTour();
};