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
#ifndef NAV_H
#define NAV_H

#ifdef __cplusplus
extern "C" {
#endif

extern void nav_init(void);
extern void nav_set_turn(float hdg, void (*callback)(void));
extern void nav_set_travel(float x, float y, void (*callback)(void));
extern void nav_fini(void);
extern void nav_cancel(void);

#ifdef __cplusplus
}
#endif
#endif

