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

int dev_read_csv(Waypoint **result, char *tour_name){
	FILE* fp = fopen(tour_name, "r+");
	int struct_size;

	if (!fp){/*Error call in case file does not open*/
		printf(FILE_ERROR);
		return 1;
	}
	else{
		char buffer[1024];
		int row = 0;
		int column = 0;

		fgets(buffer, 1024, fp);
		struct_size = atoi(buffer);/*Sets the size of the struct array*/
		if(struct_size == 0){/*Error call for catching 0 waypoints*/
			printf(EMPTY_WAY);
			return 1;
		}

		Waypoint *output = (Waypoint*) malloc(struct_size * sizeof(Waypoint));

		while (fgets(buffer, 1024, fp)) {
			column = 0;
			char* value = strtok(buffer, ", ");

			/*Looping over the various columns*/
			while (value) {
				switch(column){
					case 0:
						output[row].dx = atof(value);
						break;
					case 1:
						output[row].dy = atof(value);
						break;
					case 2:
						output[row].sound_name = strdup(value);
						break;
					case 3:
						output[row].qr = atoi(value);
						break;
				}
				value = strtok(NULL, ", ");
				column++;
			}
			row++;
		}
		*result = output;
		//free(output);
                fclose(fp);
		//free(output);
		return struct_size; /*Easy way to find the number of elements of the output array pointer*/
	}
}

int read_csv(Waypoint **result){
	/*Multiple tours can be created by creating different csv files*/
	char tour_name[100];
	int confirmer = 0;
	Waypoint *output = NULL;

	while(confirmer == 0){
		printf("Enter tour name (Please separate words using _):\n");
		scanf("%s", tour_name);
		if(strlen(tour_name) == 0){
			printf("ERROR! Enter a valid tour name:\n");
		}
		else{
			printf("Are you sure that the tour name is %s? Enter 1 to confirm or 0 to retry:\n", &tour_name);
			scanf("%d", &confirmer);
			if(confirmer != 0 && confirmer != 1){ /*Error call to catch invalid input*/
				printf("Enter a valid value next time!");
				confirmer = 0;
			}
		}
	}
	strcat(tour_name, ".csv");
	int struct_size = dev_read_csv(&output, tour_name);
	*result = output;
	return struct_size; /*Easy way to find the number of elements of the output array pointer*/
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
			if(confirmer != 0 && confirmer != 1){
                                printf("Enter a valid value next time!\n");
                                confirmer = 0;
                        }
		}
	}
	strcat(tour_name, ".csv");
	confirmer = 0;
	fp = fopen(tour_name, "w+");

	if(!fp){/*Error calls in case file does not open*/
		printf(FILE_ERROR);
		return 1;
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

				printf("%d -> dy:\n", i);
				scanf("%f", &dy);
			
				printf("%d -> sound file name:\n", i);
				scanf("%s", sound);
			
				printf("%d -> Does a QR code exist at this location? 1 for yes, 0 for no:\n", i);
				scanf("%d", &qr);
			
				printf("Are you sure the values of dx, dy, sound file, and qr location are %f, %f, %s, %d? Press 1 to confirm or 0 to retry:\n", dx, dy, sound, qr);
				scanf("%d", &confirmer);

				if(confirmer != 0 && confirmer != 1){
                                	printf("Enter a valid value next time!\n");
                                	confirmer = 0;
                        	}
			}
			confirmer = 0;
			fprintf(fp, "%f, %f, %s, %d\n", dx, dy, sound, qr);
		}
		fclose(fp);
		printf("All the waypoints have been recorded!\n");
		return 0;
	}
}

int update_csv(){
	int way_num, qr_update;
	int confirmer = 0;
	float dx_update, dy_update;
	char sound_update[100];
	char tour_name[100];
	Waypoint *output = NULL;
	
	/*Reading the csv file to be updated into a Waypoint array*/
	while(confirmer == 0){ /*To confirm the tour name from the user*/
		printf("Enter tour name to be updated(Please separate words using _):\n");
		scanf("%s", tour_name);
		if(strlen(tour_name) == 0){
			printf("Error!Enter a valid name for the tour!\n");
		}
		else{
			printf("Are you sure that the tour name is %s? Press 1 to confirm or 0 to retry:\n", tour_name);
			scanf("%i", &confirmer);
			if(confirmer != 0 && confirmer != 1){
				printf("Enter a valid value next time!\n");
				confirmer = 0;
			}
		}
	}
	strcat(tour_name, ".csv");
	confirmer = 0;
	int struct_size = dev_read_csv(&output, tour_name); /*Reading the array from the csv file*/
	
	/*Getting the waypoint number and values to be edited*/
	while(confirmer == 0){
		printf("Enter the waypoint number to be edited:\n");
		scanf("%d", &way_num);
		printf("Are you sure the waypoint to be updated is number %d? Press 1 to confirm or 0 to retry:\n", way_num);
		scanf("%d", &confirmer);
		if(confirmer != 0 && confirmer != 1){
			printf("Enter a valid value next time!\n");
			confirmer = 0;
		}
	}
	confirmer = 0;
	while(confirmer == 0){
		printf("Enter the new value of dx:\n");
		scanf("%f", &dx_update);

		printf("Enter the new value of dy:\n");
		scanf("%f", &dy_update);

		printf("Enter the new sound file name:\n");
		scanf("%s", sound_update);

		printf("Does a QR code exist at this location?:\n");
		scanf("%d", &qr_update);

		printf("Are you sure the new values of dx, dy, sound file, and qr location are %f, %f, %s, %d? Press 1 to confirm or 0 to retry:\n", dx_update, dy_update, sound_update, qr_update);
		scanf("%d", &confirmer);

		if(confirmer != 0 && confirmer != 1){
			printf("Enter a valid value next time!\n");
			confirmer = 0;
		}
	}
	confirmer = 0;

	/*Updating the values in the array*/
	output[way_num - 1].dx = dx_update;
	output[way_num - 1].dy = dy_update;
	output[way_num - 1].sound_name = strdup(sound_update);
	output[way_num - 1].qr = qr_update;

	/*Writing the new array into a temporary file*/
	FILE *fpw;
	fpw = fopen("temp.csv", "w+");
	fprintf(fpw, "%d\n", struct_size); /*Rewriting the size of the array which will be read*/
	for(int i = 0; i < struct_size; i++){
		fprintf(fpw, "%f, %f, %s, %d\n", output[i].dx, output[i].dy, output[i].sound_name, output[i].qr);
	}
	printf("Successfully recorded update\n");
	/*Deleting original file and renaming temp.csv*/
	if(remove(tour_name) == 0){
		printf("Original deleted!\n");
		if(rename("temp.csv", tour_name) == 0){
			printf("File renamed successfully. Update success!\n");
			fclose(fpw);
			free(output);
			return 0;
		}
		else{
			printf("Update unsuccessful. Unable to rename file.\n");
			fclose(fpw);
			free(output);
			return 1;
				}
	}
	else{
		printf("Unable to delete original!\n");
		free(output);
		fclose(fpw);
		return 1;
	}
}

int delete_csv(){ /*Function to delete an entry from array*/
	int way_num;
    	int confirmer = 0;
	char tour_name[100];
	Waypoint *output = NULL;

	/*Reading the csv file to be updated into a Waypoint array*/
	while(confirmer == 0){ /*To confirm the tour name from the user*/
		printf("Enter tour name to be updated(Please separate words using _):\n");
		scanf("%s", tour_name);
		if(strlen(tour_name) == 0){
			printf("Error!Enter a valid name for the tour!\n");
		}
		else{
			printf("Are you sure that the tour name is %s? Press 1 to confirm or 0 to retry:\n", tour_name);
			scanf("%i", &confirmer);
			if(confirmer != 0 && confirmer != 1){
				printf("Enter a valid value next time!\n");
				confirmer = 0;
			}
		}
	}
	strcat(tour_name, ".csv");
	confirmer = 0;
	int struct_size = dev_read_csv(&output, tour_name);/*Reading the csv file*/
	Waypoint *output_mod = (Waypoint *) malloc((struct_size - 1) * sizeof(Waypoint));

	while(confirmer == 0){
		printf("Enter the waypoint number to be deleted:\n");
		scanf("%d", &way_num);
		printf("Are you sure the waypoint to be deleted is number %d? Press 1 to confirm or 0 to retry:\n", way_num);
		scanf("%d", &confirmer);
		if(confirmer != 0 && confirmer != 1){
			printf("Enter a valid value next time!\n");
			confirmer = 0;
		}
	}
	way_num = way_num - 1;
	confirmer = 0;
	for(int i = 0; i < struct_size; i++){ /*Deleting the selected waypoint*/
		if(i==way_num){
			continue;
		}
		else{
			if(i < way_num){
				output_mod[i].dx = output[i].dx;
				output_mod[i].dy = output[i].dy;
				output_mod[i].sound_name = strdup(output[i].sound_name);
				output_mod[i].qr = output[i].qr;
			}
			else{
				if(i > way_num){
					output_mod[i-1].dx = output[i].dx;
					output_mod[i-1].dy = output[i].dy;
					output_mod[i-1].sound_name = strdup(output[i].sound_name);
					output_mod[i-1].qr = output[i].qr;
				}
			}
		}
	}
	free(output);

	/*Writing the new array into a temporary file*/
	FILE *fpw;
	fpw = fopen("temp.csv", "w+");
	fprintf(fpw, "%d\n", (struct_size - 1));
	for(int i = 0; i < (struct_size-1); i++){
		fprintf(fpw, "%f, %f, %s, %d\n", output_mod[i].dx, output_mod[i].dy, output_mod[i].sound_name, output_mod[i].qr);
	}
	printf("Successfully recorded update\n");

	/*Deleting original file and renaming temp.csv*/
	if(remove(tour_name) == 0){
		printf("Original deleted!\n");
		if(rename("temp.csv", tour_name) == 0){
			printf("File renamed successfully. Update success!\n");
			free(output_mod);
			fclose(fpw);
			return 0;
		}
		else{
			printf("Update unsuccessful. Unable to rename file.\n");
			fclose(fpw);
			free(output_mod);
			return 1;
		}

	}
	else {
		printf("ERROR! Unable to delete original!\n");
		return 1;
	}
}

/*Test program for these functions, can be compiled separately*/
#ifdef FILEREADER_STANDALONE
int main(){
	Waypoint *output = NULL;
	int i = 0;
	char tour[] = "tour.csv";
	char *tour_name = strdup(tour);
	
	write_csv();
	read_csv(&output);
	for(i = 0; i<3; i++){
		printf("dx -> %f, dy -> %f, sound file name -> %s \n", output[i].dx, output[i].dy, output[i].sound_name);
	}
	update_csv();
	output = NULL;
	dev_read_csv(&output, tour_name);
	for(i = 0; i<3; i++){
		printf("dx -> %f, dy -> %f, sound file name -> %s \n", output[i].dx, output[i].dy, output[i].sound_name);
	}
	delete_csv();
	output = NULL;
	dev_read_csv(&output, tour_name);
	for(i = 0; i<3; i++){
		printf("dx -> %f, dy -> %f, sound file name -> %s \n", output[i].dx, output[i].dy, output[i].sound_name);
	}
}
#endif
