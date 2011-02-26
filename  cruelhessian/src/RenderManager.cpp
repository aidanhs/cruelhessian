/*   RenderManager.cpp
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


#include "RenderManager.h"
#include "WorldMap.h"
#include "Background.h"
#include "Mouse.h"
#include "Map.h"
#include "Body.h"
#include "Bot.h"
#include "Bonus.h"
#include "Game.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Cluster.h"
#include "Shell.h"
#include "ClusterGrenade.h"
#include "Arrow.h"
#include "InterfaceBaseManager.h"
#include "InterfaceManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"

#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


RenderManager::RenderManager()
{
    std::cout << "Starting RenderManager ... DONE" << std::endl;
}


RenderManager::~RenderManager()
{
    std::cout << "Removing RenderManager ... DONE" << std::endl;

    // std::queue<Drawable *> empty;
    // std::swap(m_queue, empty);
}



void RenderManager::AddToDraw(Drawable* dr)
{
    m_queue.push(dr);
}


void RenderManager::Draw()
{


    /* while (!m_queue.empty())
     {
         //std::cout << kolejkaLiczb.front() << ", ";
         m_queue.front()->Draw();
         m_queue.pop();
     }
    */


    world.backg->Draw();


    world.mapBack->Draw();

/*
    for (std::list<Body *>::const_iterator it = Physics.m_staticObj.begin(); it != Physics.m_staticObj.end(); ++it)
    {
        (*it)->draw();
    }
    for (std::list<Body *>::const_iterator it = Physics.m_movingObj.begin(); it != Physics.m_movingObj.end(); ++it)
    {
        (*it)->draw();
    }
    */

    glEnable(GL_TEXTURE_2D);
    for (std::list<Body *>::iterator it = Physics.m_movingObj.begin(); it != Physics.m_movingObj.end(); ++it)
    {

        if ((*it)->type == TYPE_BULLET)
            static_cast<Bullet*>(*it)->Draw();
        else if ((*it)->type == TYPE_BONUS)
            static_cast<Bonus*>(*it)->Draw();
        else if ((*it)->type == TYPE_SHELL)
            static_cast<Shell*>(*it)->Draw();
        else if ((*it)->type == TYPE_PLAYER)
            static_cast<Bot*>(*it)->Draw();
        else if ((*it)->type == TYPE_GRENADE)
            static_cast<Grenade*>(*it)->Draw();
        else if ((*it)->type == TYPE_CLUSTERGRENADE)
            static_cast<ClusterGrenade*>(*it)->Draw();
        else if ((*it)->type == TYPE_CLUSTER)
            static_cast<Cluster*>(*it)->Draw();
    }
    glDisable(GL_TEXTURE_2D);


    world.mapFront->Draw();

    world.arrow->Draw();

    if (InterfaceManager::GetSingletonPtr() != NULL)
        Interface.Draw();

    InterfaceBase.Draw();

    world.mouse->Draw();

    game.App.Display();

}
