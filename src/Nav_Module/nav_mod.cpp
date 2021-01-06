#include <string.h>
#include <math.h>
#include "coordinate.h"
#include <stdio.h>

time_dist_head nav(coords start, coords end, double velocity) {
	time_dist_head result;
	result.heading = atan((end.y - start.y) / (end.x - start.x));
	result.distance = sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2));
	result.time = (0.8 * result.distance / velocity) + (2 * 0.1 * result.distance / velocity) * 2; // The second term is to account for accelerating and de-accelerating at the start and end

	return result;
}

int main() {
	coords a, b;
	time_dist_head tdh;

	a.x = 0;
	a.y = 0;
	b.x = 1;
	b.y = 1;
	tdh = nav(a, b, 1);
	printf("%f, %f", tdh.time, tdh.distance);
}