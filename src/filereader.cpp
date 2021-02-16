/* Copyright (C) 2021 Gautam Gupta, GeeksforGeeks.org -> https://www.geeksforgeeks.org/csv-file-management-using-c/
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
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "mapstruct.h"

#define MAP_PATH "/home/pi/MuesumGuideRobot/bin/map.bin"
#define FILE_ERROR "Error opening file."
#define EMPTY_WAY "No waypoints found"

/*void writemap(){
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
		printf("Enter waypoint no. %d x and y-coordinates, whether a QR code exists there or not, and the path to the sound file to be played there:", i);
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
}*/

void write_csv(){
	std::fstream fout;
	fout.open("map.csv", ios::out | ios::app);
	int num_waypoints = 0;
	int i;
	float dx, dy;
	unsigned qr;
	char* sound_name;

	std::cout<< "Enter number of waypoints:" <<std::endl;
	std::cin >> num_waypoints;
	fout << num_waypoints << "\n";

	for (i = 0; i < num_waypoints; i++){
		std::cout << "Enter the x-coord, y-coord, path to sound file, qr code existence(0 or 1)" << std::endl;
		std::cin >> dx >> dy >> sound_name >> qr;

		fout << dx << ", "
		     << dy << ", "
			 << sound_name << ", "
			 << qr << "\n"; 	
	}
}

/*void read_csv(Waypoint (*point)[], int way_num){
	std::fstream fin;
	fin.open("map.csv", ios::in);

	int i = 0;
	int read_way_num;
	char *buffer;
	size_t bufsize = 32;
	std::vector<std::string> row;
	std::string line, word, temp;

	while (fin >> temp) {
		row.clear();
		getline(fin, line);
		std::stringstream s(line);
		while (getline(s, word, ', ')) {
			row.push_back(word);
		}

		read_way_num = stoi(row[0]);
		if (read_way_num == i){
			
		}
	}
}*/

void read_csv(Waypoint *output){
	FILE* fp = fopen("map.csv", "r");
	int struct_size;
	if (!fp){
		printf("Can't open file");
	}
	else{
		char buffer[1024];

		int row = 0;
		int column = 0;
		
		fgets(buffer, 1024, fp);
		struct_size = atoi(buffer);
		output = (Waypoint*) malloc(struct_size * sizeof(Waypoint));

		while (fgets(buffer, 1024, fp)) {
			column = 0;

			if (row==0){
				row++;
				continue;
			}

			char* value = strtok(buffer, ", ");

			while (value) {
				if (column == 0){
					(output+(row-1))->dx = atof(value); 
				}
				if (column == 1){
					(output+(row-1))->dy = atof(value);
				}
				if (column == 2){
					(output+(row-1))->sound_name = value;
				}
				if (column == 3){
					(output+(row-1))->qr = atoi(value);
				}

				value = strtok(NULL, ", ");
				column++;
			}
			row++;
		}

		fclose(fp);
	}
}

#ifdef FILEREADER_STANDALONE
int main(){
	Waypoint *output;
	write_csv();
	read_csv(output);
}
#endif