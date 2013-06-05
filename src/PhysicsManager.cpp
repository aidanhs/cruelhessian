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


// based no Polycolly by Olivier Renault


#include "PhysicsManager.h"
#include "ParserManager.h"
#include "SpatialManager.h"
#include "physics/Material.h"
#include "Body.h"
#include "Bot.h"
#include "Bonus.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Cluster.h"
#include "Shell.h"
#include "ClusterGrenade.h"
#include "WorldMap.h"
#include "TVector2D.h"
#include <iostream>



PhysicsManager::PhysicsManager()
{

    std::cout << "Starting PhysicsManager ..." << std::endl;

    fPlayerAirbornTimer = 0.0f;
    CreateCollisionTable();

    //  s_xContactMaterial.SetRestitution(0.3);
    //  s_xContactMaterial.SetFriction(0.9);
    //  s_xContactMaterial.SetStaticFriction(0.5);

}

PhysicsManager::~PhysicsManager()
{

    std::cout << "Removing PhysicsManager ..." << std::flush;

    std::list<Body *>::iterator si0 = m_movingObj.begin();
    while (si0 != m_movingObj.end())
    {
       // delete *si0;
        si0 = m_movingObj.erase(si0);
    }

    si0 = m_staticObj.begin();
    while (si0 != m_staticObj.end())
    {
        //delete *si0;
        si0 = m_staticObj.erase(si0);
    }

    std::cout << " DONE" << std::endl;
    //m_pxBodies.clear();
    //m_pxPlayer = m_pxBodies[ePlayer];

}


void PhysicsManager::CreateCollisionTable()
{
    for (unsigned int i = 0; i < 11; ++i)
        for (unsigned int j = 0; j < 11; ++j)
            m_bShouldCollide[i][j] = true;

    // okresl miedzy ktorymi elementami nigdy nie powinno byc kolizji

    m_bShouldCollide[TYPE_PLAYER][TYPE_PLAYER] = false;
    m_bShouldCollide[TYPE_BULLET][TYPE_BULLET] = false;
    m_bShouldCollide[TYPE_BULLET][TYPE_GRENADE] = m_bShouldCollide[TYPE_GRENADE][TYPE_BULLET] = false;
    m_bShouldCollide[TYPE_BULLET][TYPE_CLUSTER] = m_bShouldCollide[TYPE_CLUSTER][TYPE_BULLET] = false;
    m_bShouldCollide[TYPE_BULLET][TYPE_CLUSTERGRENADE] = m_bShouldCollide[TYPE_CLUSTERGRENADE][TYPE_BULLET] = false;
    m_bShouldCollide[TYPE_GRENADE][TYPE_GRENADE] = false;
    m_bShouldCollide[TYPE_GRENADE][TYPE_CLUSTER] = m_bShouldCollide[TYPE_CLUSTER][TYPE_GRENADE] = false;
    m_bShouldCollide[TYPE_GRENADE][TYPE_CLUSTERGRENADE] = m_bShouldCollide[TYPE_CLUSTERGRENADE][TYPE_GRENADE] = false;
    m_bShouldCollide[TYPE_POLYGON][TYPE_POLYGON] = false;
    m_bShouldCollide[TYPE_COLLIDER][TYPE_COLLIDER] = false;

}



void PhysicsManager::GameUpdate(float dt)
{

    // delete if needed

    std::list<Body *>::iterator it = m_movingObj.begin();
    while (it != m_movingObj.end())
    {
        if (((*it)->type == TYPE_BONUS && static_cast<Bonus*>((*it))->killMyself) ||
                ((*it)->type == TYPE_BULLET && static_cast<Bullet*>((*it))->killMyself) ||
                ((*it)->type == TYPE_GRENADE && static_cast<Grenade*>((*it))->killMyself) ||
                ((*it)->type == TYPE_CLUSTER && static_cast<Cluster*>((*it))->killMyself) ||
				((*it)->type == TYPE_SHELL && static_cast<Shell*>((*it))->killMyself) ||
                ((*it)->type == TYPE_CLUSTERGRENADE && static_cast<ClusterGrenade*>((*it))->killMyself))
        {
            //delete (*it);
            it = m_movingObj.erase(it);
        }

        else
            ++it;
    }


    for (it = m_movingObj.begin(); it != m_movingObj.end(); ++it)
    {
        if ((*it)->type == TYPE_BONUS)
            static_cast<Bonus*>(*it)->Update();
        else if ((*it)->type == TYPE_PLAYER)
            static_cast<Bot*>(*it)->Update();
        else if ((*it)->type == TYPE_BULLET)
            static_cast<Bullet*>(*it)->Update();
        else if ((*it)->type == TYPE_GRENADE)
            static_cast<Grenade*>(*it)->Update();
        else if ((*it)->type == TYPE_CLUSTER)
            static_cast<Cluster*>(*it)->Update();
        else if ((*it)->type == TYPE_CLUSTERGRENADE)
            static_cast<ClusterGrenade*>(*it)->Update();
        else if ((*it)->type == TYPE_SHELL)
            static_cast<Shell*>(*it)->Update();
    }


    //UpdatePlayerMovement(dt);

    fPlayerAirbornTimer -= dt;

    if (fPlayerAirbornTimer < 0.0f)
        fPlayerAirbornTimer = 0.0f;

    //-------------------------------------------------------
    // add forces
    //-------------------------------------------------------

    for (it = m_movingObj.begin(); it != m_movingObj.end(); ++it)
    {
        (*it)->AddForce(TVector2D(0, 100 * (*it)->GetMass()));
    }


    //-------------------------------------------------------
    // test collisions, we don't check collisions between static objects
    //-------------------------------------------------------

    // na poczatku wszystkie boty nei koliduja z polygonami



    Spatial.ReAddMovable();

    std::list<Body *>::iterator it1, it2;

    for (int i = 0; i < Spatial.m_numSectors2; ++i)
    {
        // check moving objects
        if (!Spatial.m_hashTableMoving[i].empty())
        {
            for (it1 = Spatial.m_hashTableMoving[i].begin(); it1 != Spatial.m_hashTableMoving[i].end(); ++it1)
            {
                std::list<Body *>::iterator temp = it1;
                for (it2 = ++temp; it2 != Spatial.m_hashTableMoving[i].end(); ++it2)
                {
                    if (m_bShouldCollide[(*it1)->type][(*it2)->type])
                        (*it1)->Collide(*(*it2), dt);
                }
            }

            // check static and moving objects
            if (!Spatial.m_hashTableStatic[i].empty())
            {
                for (it1 = Spatial.m_hashTableStatic[i].begin(); it1 != Spatial.m_hashTableStatic[i].end(); ++it1)
                {
                    for (it2 = Spatial.m_hashTableMoving[i].begin(); it2 != Spatial.m_hashTableMoving[i].end(); ++it2)
                    {
                        if (m_bShouldCollide[(*it1)->type][(*it2)->type])
                            (*it1)->Collide(*(*it2), dt);
                    }
                }
            }

        }
    }

    //-------------------------------------------------------
    // Integrate
    //-------------------------------------------------------

    for (it = m_movingObj.begin(); it != m_movingObj.end(); ++it)
    {
        (*it)->Update(dt);
    }
}

