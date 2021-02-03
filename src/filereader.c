#include <stdio.h>
#include <stdlib.h>
#include "mapstruct.h"

void writemap(){
	int num_waypoints;
	int i;
	FILE *fp;

	printf("Enter the number of waypoints in the tour:");
	scanf("%d", &num_waypoints);
	waypoint waypoints[num_waypoints];

	if ((fp = fopen("/home/pi/MuesumGuideRobot/bin/map.bin","wb")) == NULL){
		printf("Error opening file");
		exit(1);
	}

	for(i = 0; i < num_waypoints; i++){
		printf("Enter waypoint no. %d x and y-coordinates, whether a QR code exists there or not, and the sound file to be played there:", i);
		scanf("%f %f %u %s", &waypoints[i].dx, &waypoints[i].dy, &waypoints[i].qr, &waypoints[i].sound_name);
	}

	fwrite(waypoints, sizeof(waypoints), 1, fp);
	fclose(fp);
}

