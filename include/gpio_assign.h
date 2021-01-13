/* Copyright (C) 2021 Duncan Lowther
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

