#ifndef TVECTOR2D_H
#define TVECTOR2D_H

/*   TVector2D.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
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


#include <stdlib.h>
#include <cmath>
//#include "Fixed.h"

//#define _USE_MATH_DEFINES


const float RAD_TO_DEG = 180.0f / static_cast<float>(M_PI);
const float DEG_TO_RAD = static_cast<float>(M_PI) / 180.0f;



inline float RadiansToDegrees(float rad)
{
    return rad * RAD_TO_DEG;
    //return rad * 57.324;
}
/*
inline float DegreesToRadians(float deg)
{
	return deg * M_PI / 180.0f;
	//return deg * 0,01745;
}
*/


class TMatrix2D;

class TVector2D
{
public:
    float x;
    float y;

//  -------------------              constructors

    TVector2D() : x(0), y(0) {};
    TVector2D(float a, float b) : x(a), y(b) {};


// ---------------------         operatory przypisania

    TVector2D& operator=(const TVector2D&);
    TVector2D& operator=(float);


// ---------------------         operatory porownania

    bool operator==(const TVector2D&) const;
    bool operator!=(const TVector2D&) const;


// ---------------------         operatory dodawania

    TVector2D operator+(const TVector2D&) const;
    TVector2D& operator+=(const TVector2D&);


// ---------------------         operatory odejmowania i odwracania znaku

    TVector2D operator-(const TVector2D&) const;
    TVector2D operator-(float) const;
    TVector2D operator-() const;
    TVector2D& operator-=(const TVector2D&);


// ---------------------         operatory mnozenia

    friend TVector2D operator*(float, const TVector2D&);
    TVector2D operator*(float) const;
    TVector2D operator*(const TMatrix2D& M) const;
    TVector2D& operator*=(float b);
    TVector2D& operator*=(const TMatrix2D& M);


// ---------------------         operatory dzielenia

    friend TVector2D operator/(float, const TVector2D&);
    TVector2D operator/(float) const;
    TVector2D& operator/=(float b);


// ---------------------         operatory iloczynu skalarnego

    float operator*(const TVector2D&) const;


// ---------------------         operatory iloczynu wektorowego

    float operator^(const TVector2D&) const;
    TVector2D operator^(const TMatrix2D& M) const;
    TVector2D& operator^=(const TMatrix2D& M);


    float normalize();
    TVector2D Direction(void) const;
    TVector2D& Rotate(float angle);
    TVector2D& Rotate(const TVector2D& xCentre, float fAngle);

};



#endif
