/*   TMatrix2D.cpp
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

#include "TMatrix2D.h"



TMatrix2D TMatrix2D::operator*(const TMatrix2D& M) const
{

    return TMatrix2D(e11 * M.e11 + e12 * M.e21,
                     e11 * M.e12 + e12 * M.e22,
                     e21 * M.e11 + e22 * M.e21,
                     e21 * M.e12 + e22 * M.e22);
}

TMatrix2D TMatrix2D::operator^(const TMatrix2D& M) const
{

    return TMatrix2D(e11 * M.e11 + e12 * M.e12,
                     e11 * M.e21 + e12 * M.e22,
                     e21 * M.e11 + e22 * M.e12,
                     e21 * M.e21 + e22 * M.e22);
}

TMatrix2D TMatrix2D::operator*(float s) const
{

    return TMatrix2D(e11 * s,
                     e12 * s,
                     e21 * s,
                     e22 * s);
}
