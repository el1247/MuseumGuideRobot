/* Copyright (C) 2021 Gautam Gupta
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

#ifndef MAPSTRUCT_H
#define MAPSTRUCT_H
#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
    float dx, dy;
    unsigned qr; //0 -> no QR at location, 1 -> QR is present
    char *sound_name;
} Waypoint;

typedef struct{
	char *data;
	float dx, dy, face;
} qr_Code;

#ifdef __cplusplus
}
#endif
#endif