#include <coordinate.h>
#include <math.h>
//#include <DUNCANS MODULE GOES HERE>

int nav(coords start, coords end, double velocity){
    double heading = atan((end.y - start.y)/(end.x - start.x));
    double distance = sqrt(pow((end.x - start.x), 2.0) + pow((end.y - start.y), 2.0));
    double time_needed = distance/velocity;
    
    //Function to drive from start to end
}