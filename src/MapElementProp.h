#ifndef MAP_ELEMENT_PROP_H
#define MAP_ELEMENT_PROP_H

/*   MapElementProp.h
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

#include "MapElement.h"



class MapElementProp : public MapElement
{
public:
    typedef enum
    {
        dbBEHIND_ALL = 0,
        dbBEHIND_MAP,
        dbBEHIND_NONE
    } PMS_DRAWBEHIND;

    // typedef struct
//    {

    bool active;
    ubyte filler1;
    word style;
    long width;
    long height;
    float x;
    float y;
    float rotation;
    float scaleX;
    float scaleY;
    ubyte alpha;
    ubyte filler2[3];
    PMS_COLOR color;
    PMS_DRAWBEHIND level;
    ubyte filler3[3];

    MapElementProp() {};
    void Draw() const;
};



#endif
