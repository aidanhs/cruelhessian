#ifndef MOV_OBJ_H
#define MOV_OBJ_H

/*   MovingObject.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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


#include "Tex.h"
#include "Enums.h"
#include "TVector2D.h"


class MovingObject
{


protected:
    //int type;               // 0 - circle, 1 - segment
    float mass;              // masa
    float massInv;           // odwrotnosc masy
    TVector2D maxSpeed;      // predkosc max
    TVector2D forces;        // sila wypadkowa obiektu
    // Tex texture;

public:

    virtual ~MovingObject() {}

    OBJECT_SHAPE type;
    unsigned int owner;
    TVector2D position;      // polozenie obiektu
    TVector2D velocity;      // predkosc chwilowa obiektu
    float r;                 // radius      -          nie zawsze trzeba !!!!!!!
    float w;                 // szerokosc               niekoniecznie
    float h;                 // wysokosc                niekoniecznie
    float friction;                 // tarcie

    //TVector2D old_position;
    TVector2D old_a;
    TVector2D a;

    //virtual void draw() {};        // niekoniecznie
    //void gravity();
    //void update_s();
    void AccumulateForces();
    void Verlet();
};


#endif

