/*   tvector2d.h
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

#ifndef TVECTOR2D_H
#define TVECTOR2D_H


class TVector2D
{
public:
    float x;
    float y;

    //TVector2D(const TVector2D &);
//    TVector2D& operator+=(TVector2D &a, const TVector2D &b);
    TVector2D();
    TVector2D(float, float);
    TVector2D& operator=(const TVector2D&);
    TVector2D& operator=(float);

    bool operator==(const TVector2D&) const;
    bool operator!=(const TVector2D&) const;

    TVector2D operator+(const TVector2D&) const;
    TVector2D operator-(const TVector2D&) const;
    TVector2D operator-(float) const;
    float operator*(const TVector2D&) const;
    TVector2D operator*(float) const;
    TVector2D operator/(float) const;
    friend TVector2D operator*(float, const TVector2D&);
    friend TVector2D operator/(float, const TVector2D&);
    TVector2D& operator+=(const TVector2D&);
    TVector2D& operator-=(const TVector2D&);
    TVector2D& operator*=(float b);
    TVector2D& operator/=(float b);
    void normalize();
    float dot(const TVector2D& a, const TVector2D& b) const;

};

#endif
