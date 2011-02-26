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


#include "SpatialManager.h"
#include "PhysicsManager.h"
#include "Map.h"
#include "Body.h"
#include "WorldMap.h"



SpatialManager::SpatialManager() :
    m_numSectors(world.map->numSectors*2 + 1),
    m_numSectors2(m_numSectors * m_numSectors)
{

    std::cout << "Starting SpatialManager ..." << std::flush;

    m_hashTableStatic.resize(m_numSectors2);
    m_hashTableMoving.resize(m_numSectors2);

    std::cout << " DONE" << std::endl;
}


SpatialManager::~SpatialManager()
{

    std::cout << "Removing SpatialManager ..." << std::flush;

    for (unsigned int i = 0; i < m_hashTableMoving.size(); ++i)
    {
        m_hashTableMoving[i].clear();
    }
    m_hashTableMoving.clear();


    for (unsigned int i = 0; i < m_hashTableStatic.size(); ++i)
    {
        m_hashTableStatic[i].clear();
    }
    m_hashTableStatic.clear();

    std::cout << " DONE" << std::endl;

}


Body* SpatialManager::WhichBody(unsigned int number) const
{
    std::list<Body *>::iterator i = Physics.m_staticObj.begin();
    std::advance(i, number);

    return *i;
}


// wstawia trojkaty do odpowiednich sektorow
void SpatialManager::AddStatic()
{

    for (int i = 0; i < m_numSectors2; ++i)
        for (unsigned int j = 0; j < world.map->sector[i].polyCount; ++j)
        {
            m_hashTableStatic[i].push_back(WhichBody(world.map->sector[i].polys[j]-1));
        }

}

int SpatialManager::FindIndex(float x, float y) const
{
    int indexX = -1, indexY = -1;

    for (float i = world.map->leftoffs; i <= world.map->rightoffs; i+=world.map->sectorDivisions)
    {
        if (x < i)
        {
            // znaleziono X, teraz Y

            for (float j = world.map->topoffs; j <= world.map->bottomoffs; j+=world.map->sectorDivisions)
            {
                if (y < j)
                {
                    return indexY + indexX * m_numSectors;
                }
                //     if (y == j)
                //   std::cout << "DDDDDDDDDDDDDDDDDDDD" << std::endl;
                ++indexY;
            }
            return -1;
        }
        /*if (x == i)
        {
            std::cout << "DDDDDDDDDDDDDDDDDDDD" << std::endl;
        }*/
        ++indexX;
    }
    return -1;
}


void SpatialManager::ReAddMovable()
{

    // wyczysc poprzednie informacje o ruchomych obiektach

    for (int i = 0; i < m_numSectors2; ++i)
    {
        m_hashTableMoving[i].clear();
    }

    int index[4];
    std::list<Body *>::iterator it = Physics.m_movingObj.begin();
    for (it = Physics.m_movingObj.begin(); it != Physics.m_movingObj.end(); ++it)
    {

        (*it)->FindAABB();

        index[0] = FindIndex((*it)->min.x, (*it)->min.y);
        index[1] = FindIndex((*it)->min.x, (*it)->max.y);
        index[2] = FindIndex((*it)->max.x, (*it)->min.y);
        index[3] = FindIndex((*it)->max.x, (*it)->max.y);

        if (index[0] < 0 || index[1] < 0 || index[2] < 0 || index[3] < 0 ||
            index[0] > m_numSectors2 || index[1] > m_numSectors2 || index[2] > m_numSectors2 || index[3] > m_numSectors2)
        {
          //  delete (*it);
            //it++;
         //   std::cout <<"ups"<<std::endl;
            continue;
        }


        m_hashTableMoving[index[0]].push_back(*it);
        m_hashTableMoving[index[1]].push_back(*it);
        m_hashTableMoving[index[2]].push_back(*it);
        m_hashTableMoving[index[3]].push_back(*it);

    }

}
