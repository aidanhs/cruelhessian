#ifndef SPATIAL_H
#define SPATIAL_H

/*   SpatialManager.h
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



#include "Singleton.h"
#include <vector>
#include <list>

class Body;


class SpatialManager : public Singleton<SpatialManager>
{

    const int m_numSectors;
    int FindIndex(float x, float y) const;
    Body* WhichBody(unsigned int number) const;
	SpatialManager& operator=(const SpatialManager&) {return *this;}

public:
    const int m_numSectors2;

    SpatialManager();
    ~SpatialManager();

    std::vector<std::list<Body *> > m_hashTableMoving;
    std::vector<std::list<Body *> > m_hashTableStatic;
    void AddStatic();
    void ReAddMovable();

};

#define Spatial SpatialManager::GetSingleton()

#endif
