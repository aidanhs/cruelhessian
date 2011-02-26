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
#include "PhysicsManager.h"
#include "WeaponManager.h"
#include "Enums.h"
#include "Bot.h"
#include "BotManager.h"
#include "ParserManager.h"
#include "AudioManager.h"
#include "Map.h"
#include "MapElementPolygon.h"
#include "Body.h"
#include "Cluster.h"
#include "Shell.h"
#include "ClusterGrenade.h"
#include "Bonus.h"
#include "Bullet.h"
#include "WorldMap.h"
#include "textinout/TextOutput.h"


bool HandleContact(const TVector2D& N, float& t, Body* pxBody1, Body* pxBody2)
{

    if (N.y > 0.0f)
        Physics.fPlayerAirbornTimer = (N.y* N.y);


    for (int i = 0; i < 2; i++)
    {
        if (i == 1)
        {
            Body* temp = pxBody1;
            pxBody1 = pxBody2;
            pxBody2 = temp;
        }

// ---------------------------------------------------------------------------------------------------
// ------------------------------------   Player collisions  ------------------------------------------
// ---------------------------------------------------------------------------------------------------


        if (pxBody1->type == TYPE_PLAYER)
        {

            if (pxBody2->type == TYPE_PLAYER)
            {
                return false;
            }
            else if (pxBody2->type == TYPE_POLYGON)
            {

                Bot* xbot = static_cast<Bot*>(pxBody1);
                MapElementPolygon *xpoly = static_cast<MapElementPolygon*>(pxBody2);

                switch (xpoly->polyType)
                {

                case MapElementPolygon::ptNORMAL:
                {
                    //std::cout << "normal\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptONLY_PLAYERS_COLLIDE:
                {
                    std::cout << "ONLY PLAYERS COLLIDE\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptONLY_BULLETS_COLLIDE:
                case MapElementPolygon::ptNO_COLLIDE:
                {
                    xbot->collisionWithPoly = false;
                    //std::cout << "NO collide\n";
                    //static_cast<Bot*>(pxBody1)->isAbleToJump = true;
                    return false;
                }
                case MapElementPolygon::ptICE:
                {
                    std::cout << "todo : ICE\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptDEADLY:
                {
                    std::cout << "todo : DEADLY\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptBLOODY_DEADLY:
                {
                    std::cout << "todo : BLOODY DEADLY\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptHURTS:
                {
                    std::cout << "todo : HURTS\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptREGENERATES:
                {
                    std::cout << "todo : REGENERATES\n";
                    xbot->collisionWithPoly = true;
                    if (xbot->actLife < Bots.fullLife)
                    {
                        xbot->actLife += 5;
                        if (Parser.SOUNDS_VOL > 0)
                            Audio.Play(Audio.regenerate);
                    }

                    return true;
                }
                case MapElementPolygon::ptLAVA:
                {
                    std::cout << "todo : LAVA\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }

                case MapElementPolygon::ptRED_PLAYERS_COLLIDE:
                {
                    if (xbot->team == TEAM_ALPHA)
                    {
                        xbot->collisionWithPoly = true;
                        return true;
                    }
                    else
                    {
                        xbot->collisionWithPoly = false;
                        return false;
                    }

                }

                case MapElementPolygon::ptBLUE_PLAYERS_COLLIDE:
                {
                    if (xbot->team == TEAM_BRAVO)
                    {
                        xbot->collisionWithPoly = true;
                        return true;
                    }
                    else
                    {
                        xbot->collisionWithPoly = false;
                        return false;
                    }
                }

                case MapElementPolygon::ptYELLOW_PLAYERS_COLLIDE:
                {
                    if (xbot->team == TEAM_CHARLIE)
                    {
                        xbot->collisionWithPoly = true;
                        return true;
                    }
                    else
                    {
                        xbot->collisionWithPoly = false;
                        return false;
                    }
                }

                case MapElementPolygon::ptGREEN_PLAYERS_COLLIDE:
                {
                    if (xbot->team == TEAM_DELTA)
                    {
                        xbot->collisionWithPoly = true;
                        return true;
                    }
                    else
                    {
                        xbot->collisionWithPoly = false;
                        return false;
                    }
                }
                case MapElementPolygon::ptONLY_FLAGGERS_COLLIDE:
                {
                    std::cout << "todo : only flaggers COLLIDE\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptEXPLOSIVE:
                {
                    std::cout << "todo : EXPLOSIVE\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                case MapElementPolygon::ptHURT_FLAGGERS:
                {
                    std::cout << "todo : HURT FLAGGERS\n";
                    xbot->collisionWithPoly = true;
                    return true;
                }
                default:
                {
                    xbot->collisionWithPoly = true;
                    //xbot->fly = false;
                    break;
                }

                }

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
            else if (pxBody2->type == TYPE_COLLIDER)
            {
                return false;
            }
            else
            {
                //static_cast<Bot*>(pxBody1)->collisionWithPoly = false;
                return false;
            }

        }


// ---------------------------------------------------------------------------------------------------
// ------------------------------------   Bullet collision  ------------------------------------------
// ---------------------------------------------------------------------------------------------------



        else if (pxBody1->type == TYPE_BULLET)
        {
            if (pxBody2->type == TYPE_BULLET)
            {
                return false;
            }
            else if (pxBody2->type == TYPE_POLYGON)
            {
                Bullet* xbullet = static_cast<Bullet*>(pxBody1);
                MapElementPolygon *xpoly = static_cast<MapElementPolygon*>(pxBody2);

                switch (xpoly->polyType)
                {
                case MapElementPolygon::ptNORMAL:
                case MapElementPolygon::ptONLY_BULLETS_COLLIDE:
                {
                    xbullet->killMyself = true;
                    return true;
                }
                case MapElementPolygon::ptRED_BULLETS_COLLIDE:
                {
                    if (world.bot[xbullet->m_iOwner]->team == TEAM_ALPHA)
                    {
                        xbullet->killMyself = true;
                        return true;
                    }
                    else
                        return false;

                }
                case MapElementPolygon::ptBLUE_BULLETS_COLLIDE:
                {
                    if (world.bot[xbullet->m_iOwner]->team == TEAM_BRAVO)
                    {
                        xbullet->killMyself = true;
                        return true;
                    }
                    else
                        return false;
                }
                case MapElementPolygon::ptYELLOW_BULLETS_COLLIDE:
                {
                    if (world.bot[xbullet->m_iOwner]->team == TEAM_CHARLIE)
                    {
                        xbullet->killMyself = true;
                        return true;
                    }
                    else
                        return false;
                }
                case MapElementPolygon::ptGREEN_BULLETS_COLLIDE:
                {
                    if (world.bot[xbullet->m_iOwner]->team == TEAM_DELTA)
                    {
                        xbullet->killMyself = true;
                        return true;
                    }
                    else
                        return false;
                }

                }
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
            else if (pxBody2->type == TYPE_COLLIDER)
            {
                return true;
            }
            else if (pxBody2->type == TYPE_BONUS)
            {
                static_cast<Bullet*>(pxBody1)->killMyself = true;
                return true;
            }
            else if (pxBody2->type == TYPE_PLAYER)
            {
                //std::cout << "CIAL//O pociiiiiiiiiiiiiisk" << std::endl;
                world.onReceiveShot(static_cast<Bot*>(pxBody2)->number,
                                    static_cast<Bullet*>(pxBody1)->m_iOwner,
                                    Weapons[(static_cast<Bullet*>(pxBody1))->m_iGunModel].damage);
                //static_cast<Bullet*>(pxBody2)->killMyself = true;
                return true;
            }
        }

// ---------------------------------------------------------------------------------------------------
// ------------------------------------   Bonus collision  ------------------------------------------
// ---------------------------------------------------------------------------------------------------


        else if (pxBody1->type == TYPE_BONUS)
        {
            if (pxBody2->type == TYPE_BONUS)
            {
                return true;
            }
            if (pxBody2->type == TYPE_PLAYER)
            {
                Bonus* xbonus = static_cast<Bonus*>(pxBody1);
                Bot* xbot = static_cast<Bot*>(pxBody2);

                switch (xbonus->m_iTypeBonus)
                {

                case BONUS_BERSERKER:
                    if (!xbot->MODE_BERSERKER && !xbot->MODE_PREDATOR && !xbot->MODE_FLAMEGOD)
                    {
                        xbonus->killMyself = true;
                        xbot->MODE_BERSERKER = true;
                        xbot->timeGetSuperbonus = world.getCurrentTime;
                        xbot->actLife = Bots.fullLife;
                        if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                            Audio.Play(Audio.take_berserker);
                    }

                    break;

                case BONUS_CLUSTERS:
                    xbonus->killMyself = true;
                    xbot->numGrenades = 0;
                    xbot->numClusters = 3;
                    if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                        Audio.Play(Audio.take_grenades);
                    break;

                case BONUS_GRENADES:
                    if (xbot->numClusters == 0 && xbot->numGrenades < Parser.MAX_GRENADES/2)
                    {
                        xbonus->killMyself = true;
                        if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                            Audio.Play(Audio.take_grenades);

                        xbot->numGrenades = Parser.MAX_GRENADES/2;
                    }

                    break;

                case BONUS_FLAMER:
                    if (!xbot->MODE_BERSERKER && !xbot->MODE_PREDATOR && !xbot->MODE_FLAMEGOD)
                    {
                        xbot->timeGetSuperbonus = world.getCurrentTime;
                        xbot->MODE_FLAMEGOD = true;
                        xbonus->killMyself = true;
                        xbot->actLife = Bots.fullLife;
                        if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                            Audio.Play(Audio.take_flamer);

                    }
                    break;

                case BONUS_MEDIKITS:
                    if (xbot->actLife < Bots.fullLife)
                    {
                        xbonus->killMyself = true;
                        xbot->actLife = Bots.fullLife;
                        if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                            Audio.Play(Audio.take_medikit);
                    }
                    break;

                case BONUS_PREDATOR:
                    if (!xbot->MODE_BERSERKER && !xbot->MODE_PREDATOR && !xbot->MODE_FLAMEGOD)
                    {
                        xbot->timeGetSuperbonus = world.getCurrentTime;
                        xbot->MODE_PREDATOR = true;
                        xbonus->killMyself = true;
                        xbot->actLife = Bots.fullLife;
                        if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                            Audio.Play(Audio.take_predator);
                    }

                    break;

                case BONUS_VESTKIT:
                    // bulletproof vest
                    xbonus->killMyself = true;
                    xbot->procVest = 1.0f;
                    if (xbot->number == world.MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                        Audio.Play(Audio.take_vestkit);
                    break;

                default:
                    break;

                }

            }
            else if (pxBody2->type == TYPE_POLYGON &&
                     static_cast<MapElementPolygon*>(pxBody2)->polyType != MapElementPolygon::ptNO_COLLIDE)
            {
                //if (Parser.SOUNDS_VOL > 0)
                //  Audio.Play(Audio.sound_kitfall[rand()%2]);
                return true;
            }
            else if (pxBody2->type == TYPE_GRENADE)
            {
                return true;
            }
            else if (pxBody2->type == TYPE_COLLIDER)
            {
                return false;
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


// ---------------------------------------------------------------------------------------------------
// ------------------------------------   Grenade collision  ------------------------------------------
// ---------------------------------------------------------------------------------------------------


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
            else if (pxBody2->type == TYPE_COLLIDER)
            {
                return false;
            }
            else if (pxBody2->type == TYPE_POLYGON &&
                     static_cast<MapElementPolygon*>(pxBody2)->polyType == MapElementPolygon::ptNORMAL)
            {
                return true;
            }
        }

// ---------------------------------------------------------------------------------------------------
// ------------------------------------   Cluster collision  ------------------------------------------
// ---------------------------------------------------------------------------------------------------


        else if (pxBody1->type == TYPE_CLUSTER)
        {
            if (pxBody2->type == TYPE_POLYGON)
            {
                static_cast<Cluster*>(pxBody1)->ThrowFive();

                return true;
            }
        }

// ---------------------------------------------------------------------------------------------------
// ------------------------------------   ClusterGrenade collision  ------------------------------------------
// ---------------------------------------------------------------------------------------------------


        else if (pxBody1->type == TYPE_CLUSTERGRENADE)
        {
            if (pxBody2->type == TYPE_POLYGON)
            {
                static_cast<ClusterGrenade*>(pxBody1)->m_iBouncingCount++;

                return true;
            }
        }


// ---------------------------------------------------------------------------------------------------
// ------------------------------------   Shells collision  ------------------------------------------
// ---------------------------------------------------------------------------------------------------


        else if (pxBody1->type == TYPE_SHELL)
        {
            if (pxBody2->type == TYPE_POLYGON)
            {
                if (Parser.SOUNDS_VOL > 0)
                {
                    if (static_cast<Shell*>(pxBody1)->m_iBouncingCount == 0)
                        Audio.Play(Audio.shell[0]);
                    else if (static_cast<Shell*>(pxBody1)->m_iBouncingCount == 1)
                        Audio.Play(Audio.shell[1]);
                }

                static_cast<Shell*>(pxBody1)->m_iBouncingCount++;

                return true;
            }
        }


        /*else if (pxBody1->type == TYPE_POLYGON)
        {
            if (pxBody2->type == TYPE_POLYGON)
            {
                return false;
            }
            else if (pxBody2->type == TYPE_COLLIDER)
            {
                return false;
            }
        }*/
    }

    return false;
}




// shooted - ustrzelony
// owner - sprawca

void WorldMap::onReceiveShot(unsigned int shooted, unsigned int owner, float damage)
{
    //if ((bot[shooted]->team == bot[owner]->team && FRIENDLY_FIRE) || bot[shooted]->team != bot[owner]->team)
    if (!bot[shooted]->MODE_FLAMEGOD && !bot[shooted]->MODE_BERSERKER && !bot[shooted]->MODE_PREDATOR && bot[shooted]->team != bot[owner]->team)
    {
        bot[shooted]->actLife -= damage;
        if (bot[shooted]->actLife <= 0)
        {
            if (shooted != owner) bot[owner]->points += 10;
            bot[owner]->killedNr++;
            bot[owner]->killed = shooted;
            bot[shooted]->deaths++;
            bot[shooted]->isKilled = true;
            //std::cout << "IMIERAM" << std::endl;
            bot[shooted]->killer = owner;
            bot[shooted]->actLife = Bots.fullLife;
            bot[shooted]->respawnPeriod = 1000.f*static_cast<float>((rand()%MAX_RESPAWN_TIME + 1));
            if (bot[owner]->chatWinning != "") text_output->AddMessage(bot[owner]->name, bot[owner]->chatWinning);
            if (bot[shooted]->chatDead != "") text_output->AddMessage(bot[shooted]->name, bot[shooted]->chatDead);

            // if I'm dead
            if (shooted == MY_BOT_NR)
            {
                if (Parser.SOUNDS_VOL > 0)
                {
                    Audio.Play(Audio.sound_death[rand()%3]);
                    Audio.Play(Audio.sound_heaven);
                }

                OLD_POS = bot[MY_BOT_NR]->GetPosition();
            }
            // if I killed someone (not me)
            if (owner == MY_BOT_NR && owner != shooted)
            {
                YOU_KILLED = true;
                bot[MY_BOT_NR]->youKilledTime = getCurrentTime;
            }

            // set start values
            bot[shooted]->timerRespawnTime = getCurrentTime;
            bot[shooted]->currentFrame = 0;
            bot[shooted]->timerChangeFrame = 0;
            bot[shooted]->destinationPoint = 0;
            bot[shooted]->leftAmmos = 1;
            bot[shooted]->numGrenades = 5;
        }
    }
}

