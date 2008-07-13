/*   main.h
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

#ifndef MAIN_H
#define MAIN_H

#include "SDL_ttf.h"

int init_all();
int init_all2();
int free_init_all2();
void start_server();
void start_client();

extern TTF_Font *font1_16;
extern TTF_Font *font2_12;
extern TTF_Font *font2_28;
extern TTF_Font *FontWeaponMenu;
extern SDL_Color textColor, textYellow, textRed, textGunColor;

extern SDL_Thread *TH_ser;
extern SDL_Thread *TH_cli;

#endif
