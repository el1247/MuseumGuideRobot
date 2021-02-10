/* Copyright (C) 2021 Gautam Gupta, Parewa Labs Pvt. Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include "mapstruct.h"

#define MAP_PATH "/home/pi/MuesumGuideRobot/bin/map.bin"
#define FILE_ERROR "Error opening file."
#define EMPTY_WAY "No waypoints found"

void writemap(){
	int num_waypoints = 0;
	Waypoint point;
	int i;
	FILE *fp;

	printf("Enter the number of waypoints in the tour:");
	scanf("%d", &num_waypoints);
	Waypoint *waypoints = new Waypoint[num_waypoints]; 

	if ((fp = fopen(MAP_PATH,"wb")) == NULL){
		printf(FILE_ERROR);
		exit(1);
	}

	fwrite(&num_waypoints, sizeof(num_waypoints), 1, fp);

	for(i = 0; i < num_waypoints; i++){
		printf("Enter waypoint no. %d x and y-coordinates, whether a QR code exists there or not, and the sound file to be played there:", i);
		scanf("%f %f %u %c", &waypoints[i].dx, &waypoints[i].dy, &waypoints[i].qr, &waypoints[i].sound_name);
	}
	fwrite(waypoints, sizeof(waypoints), 1, fp); 
	fclose(fp);
}

void read_map(){
	int num_waypoints_read;
	int i;
	FILE *fpt;

	if ((fpt = fopen(MAP_PATH, "rb")) == NULL){
		printf(FILE_ERROR);
		exit(1);
	}

	fread(&num_waypoints_read, sizeof(int), 1, fpt);
	Waypoint *way_in = new Waypoint[num_waypoints_read];

	if(num_waypoints_read == NULL || num_waypoints_read == 0){
		printf(EMPTY_WAY);
		exit(1);
	}

	fread(&way_in, sizeof(way_in), 1, fpt);
}
