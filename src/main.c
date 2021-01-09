#include <pigpio.h>
#include <stdlib.h>

#include "movement.h"

int main(int argc, char ** argv) {
    /* Initialise pigpio library */
    if(gpioInitialise() < 0) return 1;
    atexit(gpioTerminate);
    
    /* Initialise modules */
    m_init();
    
    /* TODO main loop goes here */

    return 0;    
}


