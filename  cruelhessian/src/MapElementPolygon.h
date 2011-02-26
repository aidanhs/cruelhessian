#ifndef MAP_ELEMENT_POLYGON_H
#define MAP_ELEMENT_POLYGON_H

/*   MapElementPolygon.h
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



class MapElementPolygon : public MapElement
{
public:
    typedef enum
    {
        ptNORMAL = 0,
        ptONLY_BULLETS_COLLIDE,
        ptONLY_PLAYERS_COLLIDE,
        ptNO_COLLIDE,
        ptICE,
        ptDEADLY,
        ptBLOODY_DEADLY,
        ptHURTS,
        ptREGENERATES,
        ptLAVA,
		ptRED_BULLETS_COLLIDE,
		ptRED_PLAYERS_COLLIDE,
		ptBLUE_BULLETS_COLLIDE,
		ptBLUE_PLAYERS_COLLIDE,
		ptYELLOW_BULLETS_COLLIDE,
		ptYELLOW_PLAYERS_COLLIDE,
		ptGREEN_BULLETS_COLLIDE,
		ptGREEN_PLAYERS_COLLIDE,
		ptONLY_FLAGGERS_COLLIDE,
		ptEXPLOSIVE,
		ptHURT_FLAGGERS
    } PMS_POLYTYPE;

    PMS_VERTEX vertex[3];
    PMS_VECTOR perpendicular[3];
    PMS_POLYTYPE polyType;

    MapElementPolygon() {};
    void Draw() const;
};



#endif
