#include <coordinate.h>
#include <math.h>

tdh nav(coords start, coords end, double velocity){
    tdh result;
    result.heading = atan((end.y - start.y)/(end.x - start.x));
    result.distance = sqrt(pow((end.x - start.x), 2.0) + pow((end.y - start.y), 2.0));
    result.time = (0.8 * result.distance/velocity) + (2 * 0.1 * result.distance/velocity) * 2; // The second term is to account for accelerating and de-accelerating at the start and end

    return result;
}

//WARNING THIS FILE IS REDUNDANT NOW, PENDING DELETION