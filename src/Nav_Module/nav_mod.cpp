#include <string.h>
#include <math.h>
#include "coordinate.h"
#include <stdio.h>

int head_dist_time(double start_x, double start_y, double end_x, double end_y, double velocity, int select) {
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