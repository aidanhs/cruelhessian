/*   ContactListener.cpp
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


#include "ContactListener.h"
#include "../PhysicsManager.h"
#include "../WeaponManager.h"
#include "../Enums.h"
#include "../Bot.h"
#include "../Body.h"
#include "../Cluster.h"
#include "../ClusterGrenade.h"
#include "../Bonus.h"
#include "../Bullet.h"
#include "../WorldMap.h"


bool HandleContact(const TVector2D& N, float& t, Body* pxBody1, Body* pxBody2)
{

    if (N.y > 0.0f)
        Physics.fPlayerAirbornTimer = (N.y* N.y);


    //static_cast<Bot*>(pxBody1)->isAbleToFly = true;
    //static_cast<Bot*>(pxBody1)->isAbleToJump = false;

    if (pxBody1->type == TYPE_PLAYER)
    {
        //static_cast<Bot*>(pxBody1)->isAbleToJump = false;

        if (pxBody2->type == TYPE_PLAYER)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_POLYGON)
        {
            //std::cout << "CIALO trojkat" << std::endl;
            //      static_cast<Bot*>(pxBody1->pointer)->isAbleToFly = false;
            static_cast<Bot*>(pxBody1)->isAbleToJump = true;
            return true;
        }
        else if (pxBody2->type == TYPE_BONUS)
        {
            //std::cout << "CIALO bonus" << std::endl;
            world.onTouchBonus(static_cast<Bot*>(pxBody1), static_cast<Bonus*>(pxBody2));
            return true;
        }
        else if (pxBody2->type == TYPE_BULLET)
        {
            //std::cout << "CIAL//O pociiiiiiiiiiiiiisk" << std::endl;
            world.onReceiveShot(static_cast<Bot*>(pxBody1)->number,
                              static_cast<Bullet*>(pxBody2)->m_iOwner,
                              Weapons[(static_cast<Bullet*>(pxBody2))->m_iGunModel].damage);
            //static_cast<Bullet*>(pxBody2)->killMyself = true;
            return true;
        }
        else if (pxBody2->type == TYPE_GRENADE)
        {
            return true;
        }
        else if (pxBody2->type == TYPE_CLUSTER)
        {
            return true;
        }
        else if (pxBody2->type == TYPE_CLUSTERGRENADE)
        {
            return true;
        }
        else
        {
            static_cast<Bot*>(pxBody1)->isAbleToJump = false;
            return false;
        }

    }

    else if (pxBody1->type == TYPE_BULLET)
    {
        if (pxBody2->type == TYPE_BULLET)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_POLYGON)
        {
            static_cast<Bullet*>(pxBody1)->killMyself = true;
            return true;
            //return false;
        }
        else if (pxBody2->type == TYPE_GRENADE)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_CLUSTER)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_CLUSTERGRENADE)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_BONUS)
        {
            static_cast<Bullet*>(pxBody1)->killMyself = true;
            return true;
        }
    }

    else if (pxBody1->type == TYPE_BONUS)
    {
        if (pxBody2->type == TYPE_BONUS)
        {
            return true;
        }
        else if (pxBody2->type == TYPE_POLYGON)
        {
            return true;
        }
        else if (pxBody2->type == TYPE_GRENADE)
        {
            return true;
        }
        else if (pxBody2->type == TYPE_CLUSTER)
        {
            return true;
        }
        else if (pxBody2->type == TYPE_CLUSTERGRENADE)
        {
            return true;
        }
    }

    else if (pxBody1->type == TYPE_GRENADE)
    {
        if (pxBody2->type == TYPE_GRENADE)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_CLUSTER)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_CLUSTERGRENADE)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_POLYGON)
        {
            return true;
        }
    }

    else if (pxBody1->type == TYPE_POLYGON)
    {
        if (pxBody2->type == TYPE_POLYGON)
        {
            return false;
        }
        else if (pxBody2->type == TYPE_CLUSTER)
        {
            static_cast<Cluster*>(pxBody2)->ThrowFive();
            //static_cast<Cluster*>(pxBody2)->killMyself = true;
            //world.ThrowFive(static_cast<Cluster*>(pxBody2)->owner,
              //              static_cast<Cluster*>(pxBody2)->GetPosition());

            return true;
        }
        else if (pxBody2->type == TYPE_CLUSTERGRENADE)
        {
            static_cast<ClusterGrenade*>(pxBody2)->m_iTouchNumber++;

            return true;
        }
    }

    return true;
}

