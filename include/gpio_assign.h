#ifndef GPIO_ASSIGN_H
#define GPIO_ASSIGN_H

/* Enum instead of #defines causes conflicting assignments to error at compile-time */
enum {
    /* Motor controllers (L and R) */
    PIN_M_L_FWD = 22,
    PIN_M_L_REV = 23,
    PIN_M_L_PWM = 12,
    PIN_M_R_FWD = 24,
    PIN_M_R_REV = 25,
    PIN_M_R_PWM = 13,
};

#endif

