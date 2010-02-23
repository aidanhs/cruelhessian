/*   tvector2d.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84 at gmail.com>
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


#include "tvector2d.h"
#include <cmath>



//TVector2D TVector2D::normalize()
void TVector2D::normalize()
{
    float len = sqrt(this->x*this->x + this->y*this->y);

    this->x = (len == 0) ? 0 : this->x / len;
    this->y = (len == 0) ? 0 : this->y / len;
    //return (len == 0) ? TVector2D(0, 0) : TVector2D(this->x / len, this->y / len);
}

TVector2D::TVector2D()
{
    x = y = 0;
}

TVector2D::TVector2D(float a, float b)
{
    x = a;
    y = b;
}

TVector2D& TVector2D::operator=(float b)
{
    x = y = b;
    return *this;
}


TVector2D& TVector2D::operator=(const TVector2D& b)
{
    x = b.x;
    y = b.y;
    return *this;
}

bool TVector2D::operator==(const TVector2D& b) const
{
    return ((x == b.x) && (y == b.y));
}

bool TVector2D::operator!=(const TVector2D& b) const
{
    return ((x != b.x) || (y != b.y));

}

TVector2D& TVector2D::operator+=(const TVector2D &b)
{
    x += b.x;
    y += b.y;
    return *this;
}

TVector2D& TVector2D::operator-=(const TVector2D &b)
{
    x -= b.x;
    y -= b.y;
    return *this;
}

TVector2D& TVector2D::operator*=(float b)
{
    x *= b;
    y *= b;
    return *this;
}

TVector2D& TVector2D::operator/=(float b)
{
    x /= b;
    y /= b;
    return *this;
}


TVector2D TVector2D::operator+(const TVector2D& b) const
{
    TVector2D tmp;

    tmp.x = x + b.x;
    tmp.y = y + b.y;

    return tmp;
}

TVector2D TVector2D::operator-(const TVector2D& b) const
{
    TVector2D tmp;

    tmp.x = x - b.x;
    tmp.y = y - b.y;

    return tmp;
}

float TVector2D::operator*(const TVector2D& b) const
{
    return x * b.x + y * b.y;
}



TVector2D TVector2D::operator*(float a) const
{
    TVector2D tmp;

    tmp.x = x * a;
    tmp.y = y * a;

    return tmp;
}

TVector2D TVector2D::operator/(float a) const
{
    TVector2D tmp;

    tmp.x = x / a;
    tmp.y = y / a;

    return tmp;
}

TVector2D TVector2D::operator-(float a) const
{
    TVector2D tmp;

    tmp.x = x - a;
    tmp.y = y - a;

    return tmp;
}



TVector2D operator*(float a, const TVector2D& b)
{
    TVector2D tmp;

    tmp.x = b.x * a;
    tmp.y = b.y * a;

    return tmp;
}

TVector2D operator/(float a, const TVector2D& b)
{
    TVector2D tmp;

    tmp.x = b.x / a;
    tmp.y = b.y / a;

    return tmp;
}

// iloczyn skalarny
float TVector2D::dot(const TVector2D& a, const TVector2D& b) const
{
    return (a.x*a.y + b.x*b.y);
}
