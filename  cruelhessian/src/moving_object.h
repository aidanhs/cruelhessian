/*   moving_objects.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawe³ Konieczny <konp84 at gmail.com>
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

#ifndef MOV_OBJ_H
#define MOV_OBJ_H

#include "globals.h"


class MovingObject
{
public:
    float w;                 // szerokosc
    float h;                 // wysokosc
    float mass;              // masa
    float massInv;           // odwrotnosc masy
    float maxSpeedX;          // predkosc max po X
    float maxSpeedY;          // predkosc max po Y
    TVector2D velocity;      // predkosc chwilowa obiektu
    TVector2D forces;        // sila wypadkowa obiektu
    TVector2D position;      // polozenie obiektu

    int collision_det_with_wall(float dx, float dy);

private:
    bool rzuty_bots(int tr_number, int vec_nr, float dx, float dy);
};



#endif
