/*   MapElementPolygon.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2011 by Paweł Konieczny <konp84 at gmail.com>
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


#include "MapElementPolygon.h"


void MapElementPolygon::Draw() const
{
    glColor4ub(vertex[0].color.red, vertex[0].color.green, vertex[0].color.blue, vertex[0].color.alpha);
    glTexCoord3f(vertex[0].tu, -vertex[0].tv, 1.0f / vertex[0].rhw);
    glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);

    glColor4ub(vertex[1].color.red, vertex[1].color.green, vertex[1].color.blue, vertex[1].color.alpha);
    glTexCoord3f(vertex[1].tu, -vertex[1].tv, 1.0f / vertex[1].rhw);
    glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);

    glColor4ub(vertex[2].color.red, vertex[2].color.green, vertex[2].color.blue, vertex[2].color.alpha);
    glTexCoord3f(vertex[2].tu, -vertex[2].tv, 1.0f / vertex[2].rhw);
    glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
}
