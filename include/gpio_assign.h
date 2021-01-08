#ifndef GPIO_ASSIGN_H
#define GPIO_ASSIGN_H

/* Enum instead of #defines causes conflicting assignments to error at compile-time */
/* XXX Unassigned pins left with implicit value -- TODO assign actual pins with explicit enum constant value */
enum {
    /* Motor controllers (L and R) */
    PIN_M_L_FWD,
    PIN_M_L_REV,
    PIN_M_L_PWM,
    PIN_M_R_FWD,
    PIN_M_R_REV,
    PIN_M_R_PWM,
};

#endif

