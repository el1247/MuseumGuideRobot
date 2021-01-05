#ifdef COORDINATE_H
#define COORDINATE_H

typedef struct Coordinates{
    double x, y;  //x is x-coordinate, y is y-coordinate
    char audio[50]; //name of audio file
} coords;

typedef struct Time_Distance_Heading{
    double time, distance, heading;
} tdh;

tdh nav(coords, coords, double);

#endif