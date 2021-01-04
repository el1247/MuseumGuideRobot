#ifdef COORDINATE_H
#define COORDINATE_H

typedef struct Coordinates coords;
typedef struct Time_Distance_Heading tdh;
tdh nav(coords, coords, double);

#endif