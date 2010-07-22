/*   Collisions.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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

#include <set>
#include <cmath>
#include <assert.h>

#include "WorldMap.h"
#include "AudioManager.h"
#include "Game.h"


// iloczyn wektorowy
static inline float cross2(TVector2D a, TVector2D b)
{
    return (a.x*b.y - a.y*b.x);
}


// point ob1 - ammo
// circle ob2 - bot
int WorldMap::collisionPoint2Circle(const MovingObject& ob1, const MovingObject& ob2)
{

    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (!bot[i]->isKilled && i != ob1.owner)
        {
            //if (pow(position.x - bot[i]->position.x, 2.0f) + pow(position.y - bot[i]->position.y, 2.0f) <= pow(bot[i]->r, 2.0f))
            if (pow(ob1.position.x - bot[i]->position.x, 2.0f) + pow(ob1.position.y - bot[i]->position.y, 2.0f) <= pow(10, 2.0f))
            {
                return i;
            }
        }
    }
    return -1;
}


// circle ob1 - grenade
// circle ob2 - bot
int WorldMap::collisionCircle2Circle(const MovingObject& ob1, const MovingObject& ob2)
{

    // popr
    for (unsigned int i = 0; i < bot.size(); i++)
    {
        if (!bot[i]->isKilled && i != ob1.owner)
        {
            /*if ((ob1.position.x < bot[i]->position.x + bot[i]->w) &&
                    (ob1.position.x + ob1.w > bot[i]->position.x) &&
                    (ob1.position.y < bot[i]->position.y + bot[i]->h) &&
                    (ob1.position.y + ob1.h > bot[i]->position.y))
                return i;*/
            if (pow(ob1.position.x - bot[i]->position.x, 2.0f) + pow(ob1.position.y - bot[i]->position.y, 2.0f) <= pow(ob1.r + bot[i]->r, 2.0f))
            {
                //std::cout << "HIT " << bot[i]->name << std::endl;;
                return i;
            }
        }
    }
    return -1;

}



// dx, dy - przesuniecie obiektu
bool WorldMap::collisionCircle2Wall(const MovingObject& ob, float dx, float dy, int& triangle, int& line)
{

    std::set<unsigned int> trian_num;
    std::vector<unsigned int> sect_num;
    TVector2D norm, col_norm, pos, ta, tb, tc;
    float d0, d1, col_dist, min, max;
    int a[4], sec;

    pos.x = ob.position.x + dx;
    pos.y = ob.position.y + dy;

    line = triangle = -1;

    // obiekt znajduje sie w sektorze:
    // lewy gorny wierzcholek
    a[0] = static_cast<int>((pos.x - ob.r) / map->sectorDivisions) + map->numSectors;
    a[1] = static_cast<int>((pos.y - ob.r) / map->sectorDivisions) + map->numSectors;
    // prawy dolny wierzcholek
    a[2] = static_cast<int>((pos.x + ob.r) / map->sectorDivisions) + map->numSectors;
    a[3] = static_cast<int>((pos.y + ob.r) / map->sectorDivisions) + map->numSectors;


    for (int j = a[0]-1; j <= a[2]+1; ++j)
        for (int k = a[1]-1; k <= a[3]+1; ++k)
        {
            sec = 51*j + k;
            if (sec > 0 && sec < static_cast<int>(map->sector.size()))
                sect_num.push_back(sec);
            else return false;
        }

    // wyznacz numery trojkatow w tych sektorach do zbadania

    for (std::vector<unsigned int>::iterator s = sect_num.begin(); s != sect_num.end(); ++s)
        for (unsigned int j = 0; j < map->sector[*s].polyCount; ++j)
            trian_num.insert(map->sector[*s].polys[j]-1);


    for (std::set<unsigned int>::iterator i = trian_num.begin(); i != trian_num.end(); ++i)
    {

        if (map->polygon[*i].polyType != map->ptONLY_BULLETS_COLLIDE && map->polygon[*i].polyType != map->ptNO_COLLIDE)
        {
            // uses modified flounder's code from gamedev.net ...
            // ... http://www.gamedev.net/community/forums/topic.asp?topic_id=476981

            ta = TVector2D(map->polygon[*i].vertex[0].x, map->polygon[*i].vertex[0].y);
            tb = TVector2D(map->polygon[*i].vertex[1].x, map->polygon[*i].vertex[1].y);
            tc = TVector2D(map->polygon[*i].vertex[2].x, map->polygon[*i].vertex[2].y);


            //  --------------              Normal tests             -----------------------

            // first
            col_norm = norm = TVector2D(map->polygon[*i].perpendicular[0].x, map->polygon[*i].perpendicular[0].y);
            d0 = (ta - pos) * norm;
            d1 = (pos - tc) * norm;
            col_dist = d0 > d1 ? d0 : d1;
            line = 0;
            if (col_dist > ob.r) continue;

            // second
            norm = TVector2D(map->polygon[*i].perpendicular[1].x, map->polygon[*i].perpendicular[1].y);
            d0 = (tb - pos) * norm;
            d1 = (pos - ta) * norm;
            d0 = d0 > d1 ? d0 : d1;
            if (d0 > ob.r) continue;
            if (d0 > col_dist)
            {
                line = 1;
                col_norm = norm;
                col_dist = d0;
            }

            // third
            norm = TVector2D(map->polygon[*i].perpendicular[2].x, map->polygon[*i].perpendicular[2].y);
            d0 = (tc - pos) * norm;
            d1 = (pos - tb) * norm;
            d0 = d0 > d1 ? d0 : d1;
            if (d0 > ob.r) continue;
            if (d0 > col_dist)
            {
                line = 2;
                col_norm = norm;
                col_dist = d0;
            }

            //  --------------              Vertex tests             -----------------------

            // first
            norm = ta - pos;
            norm.normalize();
            d0 = tb * norm;
            d1 = tc * norm;
            max = d0 > d1 ? d0 : d1;
            min = d0 + d1 - max;
            d0 = ta * norm;
            if (d0 > max) max = d0;
            if (d0 < min) min = d0;
            d0 = pos * norm;
            max = d0 - max;
            min -= d0;
            if (max <= min) max = min;
            if (max > ob.r) continue;
            if (max > col_dist)
            {
                line = 0;
                col_norm = norm;
                col_dist = max;
            }

            // second
            norm = tb - pos;
            norm.normalize();
            d0 = tb * norm;
            d1 = tc * norm;
            max = d0 > d1 ? d0 : d1;
            min = d0 + d1 - max;
            d0 = ta * norm;
            if (d0 > max) max = d0;
            if (d0 < min) min = d0;
            d0 = pos * norm;
            max = d0 - max;
            min -= d0;
            if (max <= min) max = min;
            if (max > ob.r) continue;
            if (max > col_dist)
            {
                line = 1;
                col_norm = norm;
                col_dist = max;
            }

            // third
            norm = tc - pos;
            norm.normalize();
            d0 = tb * norm;
            d1 = tc * norm;
            max = d0 > d1 ? d0 : d1;
            min = d0 + d1 - max;
            d0 = ta * norm;
            if (d0 > max) max = d0;
            if (d0 < min) min = d0;
            d0 = pos * norm;
            max = d0 - max;
            min -= d0;
            if (max <= min) max = min;
            if (max > ob.r) continue;
            if (max > col_dist)
            {
                line = 2;
                col_norm = norm;
                col_dist = max;
            }
            col_dist -= ob.r;
            pos += col_norm*col_dist;

            triangle = *i;

            return true;
        }
    }

    line = triangle = -1;
    return false;

}

// -1 - no collision
// 10000 - out of world (delete ob)
int WorldMap::collisionPoint2Wall(const MovingObject& ob, float dx, float dy)
{

    std::set<unsigned int> trian_num;
    std::vector<unsigned int> sect_num;
    TVector2D ta, tb, tc, pos;
    int a[2];

    pos.x = ob.position.x + dx;
    pos.y = ob.position.y + dy;

    // punkt ciezkosci obiektu znajduje sie w sektorze:

    a[0] = static_cast<int>(pos.x / map->sectorDivisions) + map->numSectors;
    a[1] = static_cast<int>(pos.y / map->sectorDivisions) + map->numSectors;

    unsigned int sec = 51*a[0] + a[1];
    if (sec >= map->sector.size())
        return 10000;

    for (unsigned int j = 0; j < map->sector[sec].polyCount; ++j)
        trian_num.insert(map->sector[sec].polys[j]-1);

// http://www.gamedev.net/community/forums/topic.asp?topic_id=406403

    for (std::set<unsigned int>::const_iterator i = trian_num.begin(); i != trian_num.end(); ++i)
    {

        if (map->polygon[*i].polyType != map->ptONLY_BULLETS_COLLIDE && map->polygon[*i].polyType != map->ptNO_COLLIDE)
        {
            ta = TVector2D(map->polygon[*i].vertex[2].x, map->polygon[*i].vertex[2].y);
            tb = TVector2D(map->polygon[*i].vertex[1].x, map->polygon[*i].vertex[1].y);
            tc = TVector2D(map->polygon[*i].vertex[0].x, map->polygon[*i].vertex[0].y);

            if (cross2(pos - ta, tb - ta) < 0.0f ||
                    cross2(pos - tb, tc - tb) < 0.0f ||
                    cross2(pos - tc, ta - tc) < 0.0f)
                continue;
            else return *i;

        }
    }
    return -1;

}


// shooted - ustrzelony
// owner - sprawca
void WorldMap::hurt_bot(unsigned int shooted, unsigned int owner, float damage)
{
    //if ((bot[shooted]->team == bot[owner]->team && FRIENDLY_FIRE) || bot[shooted]->team != bot[owner]->team)
    {
        bot[shooted]->actLife -= damage;
        if (bot[shooted]->actLife <= 0)
        {
            if (shooted != owner) bot[owner]->points += 10;
            bot[owner]->killedNr++;
            bot[owner]->killed = shooted;
            bot[shooted]->deaths++;
            bot[shooted]->isKilled = true;
            bot[shooted]->killer = owner;
            bot[shooted]->actLife = fullLife;
            bot[shooted]->respawnTime = 1000*(rand()%MAX_RESPAWN_TIME + 1);
            if (Parser.CONSOLE_SHOW && bot[owner]->chatWinning != "") chat->addMessage(bot[owner]->name, bot[owner]->chatWinning);
            if (Parser.CONSOLE_SHOW && bot[shooted]->chatDead != "") chat->addMessage(bot[shooted]->name, bot[shooted]->chatDead);

            // if I'm dead
            if (shooted == MY_BOT_NR)
            {
                if (Parser.SOUNDS_VOL > 0)
                {
                    Mix_PlayChannel(-1, Audio.sound_death[rand()%3], 0);
                    Mix_PlayChannel(-1, Audio.sound_heaven, 0);
                }

                OLD_POS = bot[MY_BOT_NR]->position;
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



void WorldMap::collisions()
{

    int which_bot;
    int shooted;
    int pol_number = 0, line_number = 0;

    TVector2D delta;
    int t1, t2;


    // bots
    for (std::vector<Bot *>::iterator temp = bot.begin(); temp != bot.end();)
    {

        delta = (*temp)->velocity * fTimeStep;
        if (delta.y > 0.0)
        {

            (*temp)->isAbleToFly = true;

            // when will touch the ground
            if (collisionCircle2Wall(**temp, 0.0f, delta.y, t1, t2))
            {
                /*
                if (delta.x < 0.0)
                    (*temp)->velocity.x -= 5;
                else
                    (*temp)->velocity.x += 5;*/
                (*temp)->velocity.y = -5.0f;
                (*temp)->isAbleToJump = true;
                (*temp)->a = 0;
                (*temp)->friction = 5;
            }
            else
                (*temp)->friction = 0;
        }
        else
        {
            (*temp)->isAbleToJump = false;

            // when will touch the ceiling
            if (collisionCircle2Wall(**temp, 0.0f, delta.y, t1, t2))
            {
                (*temp)->velocity.y = 5.0f;
                (*temp)->isAbleToFly = false;
                (*temp)->a = 0;
            }
        }

        if (collisionCircle2Wall(**temp, delta.x, 0.0f, t1, t2))
        {
            (*temp)->velocity.x = (delta.x > 0) ? -5.0f : 5.0f;
            (*temp)->a = 0;
        }

        ++temp;
    }

    // bonuses
    for (std::list<Bonus *>::iterator temp = bonus_list.begin(); temp != bonus_list.end();)
    {

        // if bonus will touch the wall
        delta = (*temp)->velocity * fTimeStep;
        //if (collisionCircle2Wall(**temp, delta.x, delta.y, t1, t2))
        if (collisionCircle2Wall(**temp, 0.0f, delta.y, t1, t2))
        {
            if (Parser.SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, Audio.sound_kitfall[rand()%2], 0);
            (*temp)->a = 0;
            (*temp)->velocity.y = -5.0f;

        }
        // if bonus touched bot
        /*else if (collisionCircle2Circle(**temp, **temp) < 0)
        {
            //hurt_bot(which_bot, (*temp)->owner, weapon[bot[(*temp)->owner]->gunModel].damage);
        	bonus_list.erase(temp++);
        }*/
        ++temp;
    }


    // ammos
    for (std::list<Bullet *>::iterator temp = bullet_list.begin(); temp != bullet_list.end();)
    {

        // if ammo will touch the wall
        delta = (*temp)->velocity * fTimeStep;
        if (collisionPoint2Wall(**temp, delta.x, delta.y) >= 0)
        {
            //bullet_list.erase(temp++);
            delete *temp;
            temp = bullet_list.erase(temp);
        }
        // if ammo touched bot
        else if ((which_bot = collisionPoint2Circle(**temp, **temp)) >= 0)
        {
            hurt_bot(which_bot, (*temp)->owner, Weapons[bot[(*temp)->owner]->gunModel].damage);
            //bullet_list.erase(temp++);
            delete *temp;
            temp = bullet_list.erase(temp);
        }
        else
        {
            //(*temp)->position -= fTimeStep * (*temp)->velocity;
            ++temp;
        }
    }


    for (std::list<Grenade *>::iterator temp = gren_list.begin(); temp != gren_list.end();)
    {

        // if grenade will touch the wall
        delta = (*temp)->velocity * fTimeStep;
        if (collisionCircle2Wall(**temp, delta.x, delta.y, pol_number, line_number))
        {

            if (Parser.SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, Audio.grenade_bounce, 0);

            // V -=  2 * N * dot(V • N)

            (*temp)->velocity.x -= 1.8f*map->polygon[pol_number].perpendicular[line_number].x*
                                   map->polygon[pol_number].perpendicular[line_number].x*
                                   (*temp)->velocity.x;
            (*temp)->velocity.y -= 1.8f*map->polygon[pol_number].perpendicular[line_number].y*
                                   map->polygon[pol_number].perpendicular[line_number].y*
                                   (*temp)->velocity.y;


            //     (*temp)->velocity.x = -(*temp)->velocity.x;
            //     (*temp)->velocity.y = -(*temp)->velocity.y;

        }
        // if grenade touched bot, explode immediately
        shooted = collisionCircle2Circle(**temp, **temp);
        if (shooted != static_cast<int>((*temp)->owner) && shooted >= 0)
        {
            //std::cout << "SOATAL\n";
            (*temp)->timer_throw -= 4000;
            hurt_bot(shooted, (*temp)->owner, 50.0f);
            //gren_list.erase(temp++);
            delete *temp;
            temp = gren_list.erase(temp);
            continue;
        }
        //else
        {
            //  (*temp)->position -= (*temp)->velocity * fTimeStep;
            //temp;
            //if (!collision_det(0, ammo[i].position.y-bot[i].velocity.x * aDeltaTime, &bot[i]))
        }
        if ((*temp)->killMyself)
        {
            //gren_list.erase(temp++);
            delete *temp;
            temp = gren_list.erase(temp);
            continue;
        }
        else
            ++temp;
        //tempg->position.x -= tempg->velocity.x * aDeltaTime;
        //tempg->position.y -= tempg->velocity.y * aDeltaTime;
    }
}

