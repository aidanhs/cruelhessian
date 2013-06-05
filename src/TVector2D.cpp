/*   TVector2D.cpp
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


#include "TVector2D.h"
#include "TMatrix2D.h"
#include <cmath>


TVector2D TVector2D::operator * (const TMatrix2D& M) const
{

    return TVector2D(x * M.e11 + y * M.e12,
                     x * M.e21 + y * M.e22);
}

TVector2D TVector2D::operator ^ (const TMatrix2D& M) const
{

    return TVector2D(x * M.e11 + y * M.e21,
                     x * M.e12 + y * M.e22);
}

TVector2D& TVector2D::operator *=(const TMatrix2D& M)
{
    TVector2D T = *this;
    x = T.x * M.e11 + T.y * M.e12;
    y = T.x * M.e21 + T.y * M.e22;
    return *this;
}

TVector2D& TVector2D::operator ^=(const TMatrix2D& M)
{
    TVector2D T = *this;
    x = T.x * M.e11 + T.y * M.e21;
    y = T.x * M.e12 + T.y * M.e22;
    return *this;
}


float TVector2D::normalize()
{

    float fLength = sqrt(x*x + y*y);

    if (fLength == 0.0f)
        return 0.0f;

    (*this) *= (1.0f / fLength);

    return fLength;
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

    return TVector2D(x + b.x, y + b.y);
}

TVector2D TVector2D::operator-(const TVector2D& b) const
{

    return TVector2D(x - b.x, y - b.y);
}

TVector2D TVector2D::operator-() const
{
    return TVector2D(-x, -y);
}

// iloczyn skalarny
float TVector2D::operator*(const TVector2D& b) const
{
    return x * b.x + y * b.y;
}

// iloczyn wektorowy
float TVector2D::operator^(const TVector2D& b) const
{
    return x * b.y - y * b.x;
}



TVector2D TVector2D::operator*(float a) const
{

    return TVector2D(x * a, y * a);
}


TVector2D TVector2D::operator/(float a) const
{

    return TVector2D(x / a, y / a);
}

TVector2D TVector2D::operator-(float a) const
{

    return TVector2D(x - a, y - a);
}


TVector2D TVector2D::Direction(void) const
{
    TVector2D Temp(*this);
    Temp.normalize();
    return Temp;
}

TVector2D& TVector2D::Rotate(float angle)
{
    float tx = x;
    x =  x * cos(angle) - y * sin(angle);
    y = tx * sin(angle) + y * cos(angle);
    return *this;
}

TVector2D& TVector2D::Rotate(const TVector2D& xCentre, float fAngle)
{
    TVector2D D = *this - xCentre;
    D.Rotate(fAngle);
    *this = xCentre + D;
    return *this;
}


TVector2D operator*(float a, const TVector2D& b)
{

    return TVector2D(b.x * a, b.y * a);
}

TVector2D operator/(float a, const TVector2D& b)
{

    return TVector2D(b.x / a, b.y / a);
}
