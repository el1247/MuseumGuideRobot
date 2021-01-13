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

#ifndef COORDINATE_H
#define COORDINATE_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Coordinates {
	double x, y;  //x is x-coordinate, y is y-coordinate
	char audio[50]; //name of audio file
} coords;

typedef struct Time_Distance_Heading {
	double time, distance, heading;
} time_dist_head;

int head_obj(double, double, double, double);
int dist_obj(double, double, double, double);
int time_obj(double, double, double, double, double);

#ifdef __cplusplus
}
#endif
#endif