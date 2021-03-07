/* Copyright (C) 2021 Gautam Gupta
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

#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "mapstruct.h"

#define FILE_ERROR "Error opening file.\n"
#define EMPTY_WAY "No waypoints found\n"

int read_csv(Waypoint **result){
	/*Multiple tours can be created by creating different csv files*/
	char tour_name[100];
	printf("Enter tour name (Please separate words using _):\n");
	scanf("%s", &tour_name);
	strcat(tour_name, ".csv");
	FILE* fp = fopen(tour_name, "r");

	int struct_size;
	if (!fp){/*Error call in case file does not open*/
		printf(FILE_ERROR);
		return 0;
	}
	else{
		char buffer[1024];
		int row = 0;
		int column = 0;

		fgets(buffer, 1024, fp);
		struct_size = atoi(buffer);/*Sets the size of the struct array*/
		if(struct_size == 0){/*Error call for catching 0 waypoints*/
			printf(EMPTY_WAY);
		}

		Waypoint *output = (Waypoint*) malloc(struct_size * sizeof(Waypoint));

		while (fgets(buffer, 1024, fp)) {
			column = 0;
			char* value = strtok(buffer, ", ");

			/*Looping over the various columns*/
			while (value) {
				if (column == 0){
					output[row].dx = atof(value);
				}
				else{
					if (column == 1){
						output[row].dy = atof(value);
					}
					else{
						if (column == 2){
							output[row].sound_name = strdup(value);
						}
						else{
							if (column == 3){
								output[row].qr = atoi(value);
							}
						}
					}
				}
				value = strtok(NULL, ", ");
				column++;
			}
			row++;
		}
		*result = output;
		//free(output);
                fclose(fp);
		return 1;
	}
}

int write_csv(){
	FILE *fp;
	int i, num_way, qr;
	int confirmer = 0;
	float dx, dy;
	char sound[100], tour_name[50];
	/*Enables multiple tours by allowing multiple csv files to be created*/
	while(confirmer == 0){
		printf("Enter tour name (words separated by _):\n");
		scanf("%s", tour_name);
		if(strlen(tour_name) == 0){
			printf("Error!Enter a valid name for the tour!\n");
		}
		else {
			printf("Are you sure that the tour name is %s? Enter 1 to confirm or 0 to retry:\n", tour_name);
			scanf("%i", &confirmer);
			if(confirmer == 1){
				strcat(tour_name, ".csv");
			}
		}
	}
	confirmer = 0;
	fp = fopen(tour_name, "w+");
	if(!fp){/*Error calls in case file does not open*/
		printf(FILE_ERROR);
		return 0;
	}
	else{
		while(confirmer == 0){
			printf("How many waypoints do you want to enter?\n");
			scanf("%d", &num_way);
			printf("Are you sure the number of waypoints are %d? Enter 1 to confirm or 0 to retry:\n", num_way);
			scanf("%d", &confirmer);
			if(confirmer==1){
				fprintf(fp, "%d\n", num_way); /*First line of all csv files is the number of waypoints*/
			}
		}

		confirmer = 0;
		for(i=1; i<=num_way; i++){
			while(confirmer == 0){
				printf("%d -> dx:\n", i);
				scanf("%f", &dx);
				printf("Are you sure dx%d is %f? Press 1 to confirm or 0 to retry:\n", i, dx);
				scanf("%d", &confirmer);
			}
			confirmer = 0;
			while(confirmer == 0){
				printf("%d -> dy:\n", i);
				scanf("%f", &dy);
				printf("Are you sure dy%d is %f? Press 1 to confirm or 0 to retry:\n",i,dy);
				scanf("%d", &confirmer);
			}
			confirmer = 0;
			while(confirmer == 0){
				printf("%d -> sound file name:\n", i);
				scanf("%s", sound);
				printf("Are you sure sound file %d is called %s? Press 1 to confirm or 0 to retry:\n", i, sound);
				scanf("%d", &confirmer);
			}
			confirmer = 0;
			while(confirmer == 0){
				printf("%d -> Does a QR code exist at this location? 1 for yes, 0 for no:\n", i);
				scanf("%d", &qr);
				printf("Are you sure? Press 1 to confirm or 0 to retry:\n");
				scanf("%d", &confirmer);
			}
			confirmer = 0;
			fprintf(fp, "%f, %f, %s, %d\n", dx, dy, sound, qr);
		}
		fclose(fp);
		printf("All the waypoints have been recorded!\n");
		return 1;
	}
}

/*int update_csv(){
	char tour_name[50];
	printf("Enter the name of the tour to update:\n")
	scanf("%s", tour_name);
}*/

/*Test program for these functions, can be compiled separately*/
#ifdef FILEREADER_STANDALONE
int main(){
	Waypoint *output = NULL;
	int i = 0;

	write_csv();
	read_csv(&output);
	for(i = 0; i<3; i++){
		printf("dx -> %f, dy -> %f, sound file name -> %s \n", output[i].dx, output[i].dy, output[i].sound_name);
	}
}
#endif

