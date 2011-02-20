#ifndef TMATRIX2D_H
#define TMATRIX2D_H

/*   TMatrix2D.h
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

#include <cmath>


class TVector2D;

class TMatrix2D
{

public:

    float e11, e12, e21, e22;

    TMatrix2D(float _e11, float _e12, float _e21, float _e22) :
        e11(_e11),
        e12(_e12),
        e21(_e21),
        e22(_e22)
    {}

    TMatrix2D() : e11(0), e12(0), e21(0), e22(0) {}

    // macierz obrotu
    TMatrix2D(float angle) : e11(cos(angle)), e12(sin(angle)), e21(-e12), e22(e11) {}

    // macierz jednostkowa
    static TMatrix2D Identity()
    {
        return TMatrix2D(1.0f, 0.0f, 0.0f, 1.0f);
    }

    TMatrix2D operator*(const TMatrix2D& M) const;
    TMatrix2D operator^(const TMatrix2D& M) const;
    TMatrix2D operator*(float s) const;

};

#endif
