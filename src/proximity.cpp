#include "proximity.h"

void proximity::measuredist(){
	//Function to capture and store latest value from proximity sensor
	//measure distance
}

proximity::proximity(){
	if (gpioInitialise() < 0){
		std::cout << "Pigpio failed to initiaise" << std::endl; //Possibile duplicate command, may already be initialised
	}
	//Start pigpio
	//Identify I2C connection
	//Start I2C connection

}