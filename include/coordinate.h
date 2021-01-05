typedef struct Coordinates{
    double x, y;  //x is x-coordinate, y is y-coordinate
    char audio[50]; //name of audio file
} coords;

typedef struct Time_Distance_Heading{
    double time, distance, heading;
} time_dist_head;

time_dist_head nav(coords, coords, double);