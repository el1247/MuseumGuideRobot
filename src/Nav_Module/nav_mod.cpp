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

#include <string.h>
#include <math.h>
#include "coordinate.h"
#include <stdio.h>

int heading(double start_x, double start_y, double end_x, double end_y) {
	int head = atan((end_y - start_y) / (end_x - start_x));
	return head;
}

int dist_to_object(double start_x, double start_y, double end_x, double end_y){
	int dist = sqrt(pow((end_x - start_x), 2) + pow((end_y - start_y), 2));
	return dist;
}

int time_to_object(double start_x, double start_y, double end_x, double end_y, double velocity){
	int dist = sqrt(pow((end_x - start_x), 2) + pow((end_y - start_y), 2));
	int time = (0.8 * dist / velocity) + (2 * 0.1 * dist / velocity) * 2; // The second term is to account for accelerating and de-accelerating at the start and end
	return time;
}

/*int main() {
	coords a, b;
	time_dist_head tdh;

	a.x = 0;
	a.y = 0;
	b.x = 1;
	b.y = 1;
	tdh = nav(a, b, 1);
	printf("%f, %f", tdh.time, tdh.distance);
}*/