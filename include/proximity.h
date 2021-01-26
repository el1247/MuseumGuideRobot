#pragma once
#include <stdio.h>
#include <iostream>
#include <pigpio.h>

#define proxstart = 0b01000010 //integration time 50ms, channel interrupt, interrupt persistance setting 1, interrupt enabled, ALS power on


class proximity{
	private:
		int distance; //Distance to object measured from proximity sensor
		proximity(); //Initialiser
	public:
		void measuredist(); //Method for measuring distance
};