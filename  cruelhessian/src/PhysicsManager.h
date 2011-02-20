#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H


/*   PhysicsManager.cpp
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



#include "Singleton.h"
#include <list>

class Body;


class PhysicsManager : public Singleton<PhysicsManager>
{
    //int FindBody(Body* pxBody);
    void UpdatePlayerMovement(float dt);
    void DestroyBody(Body* body);
    void UpdatePlayer(float dt);
    bool m_bShouldCollide[11][11];
    void CreateCollisionTable();

public:
    float fPlayerAirbornTimer;
   // float fCoF;
   // float fCoR;
   // float fCoS;
    std::list<Body *> m_movingObj;
    std::list<Body *> m_staticObj;

    PhysicsManager();
    ~PhysicsManager();

    void  GameUpdate			(float dt);
//    void  GameRender			(void);

};



#define Physics PhysicsManager::GetSingleton()

#endif
