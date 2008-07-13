/*   gun.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef GUN_H
#define GUN_H

#include <math.h>

const char *gun_name(int model);
const char *gun_reload(int model);
const char *gun_fire(int model);
int gun_ammos(int model);
float gun_mass(int model);
void gun_menu_select(int x, int y);
void gun_menu();
void exit_menu_select(int x, int y);
void exit_menu();
Uint32 gun_reload_time(int model);
Uint32 gun_interval_time(int model);
float gun_speed(int model);
int reload_gun(void *data);

#endif
