#ifndef SENSORS_H
#define SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* XXX TODO Implement these (and other useful) sensor capture functions (possibly with caching) in a new `sensor.c` or somesuch */
unsigned get_current_heading(void);
double get_current_velocity(void);

#ifdef __cplusplus
}
#endif

#endif

