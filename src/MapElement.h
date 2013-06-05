#ifndef MAP_ELEMENT_H
#define MAP_ELEMENT_H

/*   MapElement.h
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

#include "Body.h"
//#include "TVector2D.h"


typedef unsigned char ubyte;
typedef unsigned short word;


class MapElement : public Body
{
public:

    typedef struct
    {
        ubyte blue;
        ubyte green;
        ubyte red;
        ubyte alpha;
    } PMS_COLOR;

    typedef struct
    {
        float x;
        float y;
        float z;
    } PMS_VECTOR;

    typedef struct
    {
        float x;
        float y;
        float z;
        float rhw;
        PMS_COLOR color;
        float tu;
        float tv;
    } PMS_VERTEX;

    //static mPMS_COLOR PMS_COLOR;
};


class MapElementCollider : public MapElement
{
public:
    //typedef struct
    //  {
    bool active;
    ubyte filler[3];
    float x;
    float y;
    float radius;
    // } PMS_COLLIDER;
};

class MapElementBackground : public MapElement
{
public:
    //  MapElementBackground();

    PMS_COLOR bgColorTop;
    PMS_COLOR bgColorBottom;
};


#endif
