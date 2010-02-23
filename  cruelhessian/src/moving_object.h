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
#include "tvector2d.h"


class MovingObject
{

private:
    TVector2D temp;

protected:
    //int type;               // 0 - circle, 1 - segment
    float mass;              // masa
    float massInv;           // odwrotnosc masy
    TVector2D maxSpeed;      // predkosc max
    TVector2D forces;        // sila wypadkowa obiektu

    //  TVector2D old_position;
    // TVector2D a;

public:
    OBJECT_SHAPE type;
    unsigned int owner;
    TVector2D position;      // polozenie obiektu
    TVector2D velocity;      // predkosc chwilowa obiektu
    float r;                 // radius      -          nie zawsze trzeba !!!!!!!
    float w;                 // szerokosc               niekoniecznie
    float h;                 // wysokosc                niekoniecznie
    virtual void draw() {};        // niekoniecznie
    void gravity();
};



#endif
