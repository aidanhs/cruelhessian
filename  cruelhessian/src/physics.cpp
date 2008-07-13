/*   physics.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84@gmail.com>
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

#include "SDL_mixer.h"
#include "physics.h"
#include "globals.h"
//#include "gostek.h"
#include "gun.h"
#include "controls.h"
#include "draw_extra.h"
#include "bot.h"
#include "grenade.h"
#include "bullet.h"
#include "worldmap.h"

float OLD_POS_X;
float OLD_POS_Y;

void WorldMap::hurt_bot(int shooted, int owner)
{

    bot[shooted]->isKilled = true;
    bot[shooted]->killer = owner;
    bot[shooted]->respawnTime = 1000*(rand()%world->MAX_RESPAWN_TIME + 1);


    if (shooted == world->MY_BOT_NR)
    {
        ONLY_ONCE = true;
        OLD_POS_X = bot[shooted]->position.x;
        OLD_POS_Y = bot[shooted]->position.y;
    }
    // set start values

    bot[shooted]->position.x = (float)p.spawnpoint[shooted].x;
    bot[shooted]->position.y = (float)p.spawnpoint[shooted].y-10;
    bot[shooted]->procLife = 1.0;
    bot[shooted]->leftAmmos = gun_ammos(bot[shooted]->gunModel);


    /* if (shooted == MY_BOT_NR)
     {
         //bgX = p.spawnpoint[MY_BOT_NR].x - MAX_WIDTH/2;
         //bgY = p.spawnpoint[MY_BOT_NR].y - MAX_HEIGHT/2;
     }*/
    bot[shooted]->timerRespawnTime = SDL_GetTicks();
}

/*
// oblicza odleglosc miedzy punktem (m,n) a odcinkiem zlozonym z wierzcholkow [w1] i [w2] trojkata [number]
static float calculate_distance(int number, int m, int n, int w1, int w2)
{

    return abs(m * (p.polygon[number].vertex[w1].y - p.polygon[number].vertex[w2].y) -
               n * (p.polygon[number].vertex[w1].x - p.polygon[number].vertex[w2].x) +
               p.polygon[number].vertex[w1].x * p.polygon[number].vertex[w2].y -
               p.polygon[number].vertex[w2].x * p.polygon[number].vertex[w1].y) /
           sqrtf(pow(p.polygon[number].vertex[w1].y - p.polygon[number].vertex[w2].y, 2) +
                 pow(p.polygon[number].vertex[w1].x - p.polygon[number].vertex[w2].x, 2));
}
*/
static bool isPointOnLine(float px, float py, float ax, float ay, float bx, float by)
{
    float dx = bx - ax;
    float dy = by - ay;
    float len = sqrtf(dx*dx + dy*dy);

    // Distance is '(B - A)(normalised) X (P - A)'
    float d = (dx * (py - ay) - dy * (px - ax)) / len;
    if ((d < 1) && (d > -1))
        return true;
    else
        return false;
}

// check on which line of triangle [number] lies the point (m,n)
static int onWhichLine(int number, float m, float n)
{
    // 0,1 leg
    if (isPointOnLine(m, n, p.polygon[number].vertex[0].x, p.polygon[number].vertex[0].y, p.polygon[number].vertex[1].x, p.polygon[number].vertex[1].y)) return 0;

    // 1,2 leg
    if (isPointOnLine(m, n, p.polygon[number].vertex[1].x, p.polygon[number].vertex[1].y, p.polygon[number].vertex[2].x, p.polygon[number].vertex[2].y)) return 1;

    // 2,0 leg
    if (isPointOnLine(m, n, p.polygon[number].vertex[2].x, p.polygon[number].vertex[2].y, p.polygon[number].vertex[0].x, p.polygon[number].vertex[0].y)) return 2;

    return -1;
}


static int POL_NUMBER = 0, LINE_NUMBER = 0;

/*
bool Grenade::collision_det2_with_wall(float deltax, float deltay)
{
    int d;
    float m = deltax + this->position.x;
    float n = deltay + this->position.y;

    for (int number = 0; number < p.polygonCount; number++)
    {
        if (p.polygon[number].polyType != ptONLY_BULLETS_COLLIDE && p.polygon[number].polyType != ptNO_COLLIDE)
        {
            // sprawdz ktora krawedz trojkata [number] dotyka granat
            // (przelec wszystkie cztery wierzcholki granatu)
            if ((d = onWhichLine(number, m, n)) != -1)
            {
                POL_NUMBER = number;
                LINE_NUMBER = d;
                return true;
            }
            else if ((d = onWhichLine(number, m+this->w, n)) != -1)
            {
                POL_NUMBER = number;
                LINE_NUMBER = d;
                return true;
            }
            else if ((d = onWhichLine(number, m+this->w, n+this->h)) != -1)
            {
                POL_NUMBER = number;
                LINE_NUMBER = d;
                return true;
            }
            else if ((d = onWhichLine(number, m, n+this->h)) != -1)
            {
                POL_NUMBER = number;
                LINE_NUMBER = d;
                return true;
            }
            //else cout << "Nico\n";
        }
    }
    return false;
}
*/
void WorldMap::calculate_physics(float aDeltaTime)
{
    float deltax, deltay;
    int which_bot, number;


    // boty
    for (int i = 0; i < BOTS_NUM; i++)
    {
        // Zero's the forces
        bot[i]->forces.x = 0;

        if (!bot[i]->isFlying)
        {
            // Obliczanie sily grawitacji
            bot[i]->forces.y = bot[i]->mass * (-sGravity);

            // Obliczanie sily oporu
            bot[i]->forces.x += bot[i]->velocity.x * (-sDragWalking);
            bot[i]->forces.y += bot[i]->velocity.y * (-sDrag);
        }
        // Jesli chce latac, to go w gore
        else
        {
            // Obliczanie sily lotnej
            bot[i]->forces.y = bot[i]->mass * sFlying;

            // Obliczanie sily oporu
            bot[i]->forces.x += bot[i]->velocity.x * (-sDragFly);
            bot[i]->forces.y += bot[i]->velocity.y * (-sDragFly);
        }

        bot[i]->velocity.x += bot[i]->forces.x * bot[i]->massInv * aDeltaTime;
        bot[i]->velocity.y += bot[i]->forces.y * bot[i]->massInv * aDeltaTime-0.1;

        deltax = bot[i]->velocity.x * aDeltaTime;
        if (bot[i]->collision_det_with_wall((deltax > 0) ? -1.0: 1.0, 0.0) == -1)
        {
            bot[i]->position.x -= deltax;
            if (i == MY_BOT_NR) glTranslatef(deltax, 0.0f, 0.0f);
        }
        else
        {
            bot[i]->velocity.x = 0.0;
        }

        deltay = bot[i]->velocity.y * aDeltaTime;
        if (bot[i]->collision_det_with_wall(0.0, (deltay > 0) ? -1.0: 1.0) == -1)
        {
            bot[i]->position.y -= deltay;
            if (i == MY_BOT_NR) glTranslatef(0.0f, deltay, 0.0f);
        }
        else
        {
            bot[i]->velocity.y = 0.0;
        }

    }

    // pociski
    for (list<Bullet *>::iterator temp = ammo_list.begin(); temp != ammo_list.end(); temp++)
    {

        // Czyszczenie sil
        (*temp)->forces.x = 0.0;

        // Obliczanie sily grawitacji
        (*temp)->forces.y = -sGravity * (*temp)->mass;

        // Obliczanie sily oporu
        (*temp)->forces.x += (*temp)->velocity.x * (-sDrag);
        (*temp)->forces.y += (*temp)->velocity.y * (-sDrag);

        (*temp)->velocity.x += (*temp)->forces.x * (*temp)->massInv * aDeltaTime;
        (*temp)->velocity.y += (*temp)->forces.y * (*temp)->massInv * aDeltaTime;

        // if ammo touched the wall
        if ((*temp)->collision_det_with_wall(-(*temp)->velocity.x * aDeltaTime, -(*temp)->velocity.y * aDeltaTime) != -1)
        {
            //am_temp = &(*temp);
            temp = ammo_list.erase(temp);    // ta liniua ok!!!
            //delete am_temp;
        }
        // if ammo touched bot
        else if ((which_bot = (*temp)->collision_det_with_bots()) != -1)
        {
            /*if (bot[which_bot]->procLife >= 0.5)
                bot[which_bot]->procLife -= 0.5;
            else*/
                hurt_bot(which_bot, (*temp)->owner);
            //printf("Busted\n");

            //am_temp = &(*temp);
            temp = ammo_list.erase(temp);
            //delete am_temp;
        }
        else
        {
            (*temp)->position.x -= (*temp)->velocity.x * aDeltaTime;
            (*temp)->position.y -= (*temp)->velocity.y * aDeltaTime;
        }
    }

    // granaty
    //ST_AMMO *am_temp;
    float m, n;//, kx, ky;
    int P_A, d;

    for (list<Grenade *>::iterator tempg = gren_list.begin(); tempg != gren_list.end(); tempg++)
    {
        //ile++;

        // Czyszczenie sil
        (*tempg)->forces.x = 0.0;

        // Obliczanie sily grawitacji
        (*tempg)->forces.y = -sGravity * (*tempg)->mass;

        // Obliczanie sily oporu
        (*tempg)->forces.x += (*tempg)->velocity.x * (-sDrag);
        (*tempg)->forces.y += (*tempg)->velocity.y * (-sDrag);

        (*tempg)->velocity.x += (*tempg)->forces.x * (*tempg)->massInv * aDeltaTime;
        (*tempg)->velocity.y += (*tempg)->forces.y * (*tempg)->massInv * aDeltaTime;


        // if grenade touched the wall
        deltax = -(*tempg)->velocity.x * aDeltaTime;
        deltay = -(*tempg)->velocity.y * aDeltaTime;
        //if ((number = collision_det_gren_with_wall(-tempg->velocity.x * aDeltaTime, -tempg->velocity.y * aDeltaTime, &(*tempg))) != -1)
        if ((number = (*tempg)->collision_det_with_wall(deltax, deltay)) != -1)
        //if (collision2_det_gren_with_wall(deltax, deltay, &(*tempg)))
        {
            //cout << "KOLLIZJKA\n";
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, Mix_LoadWAV("Sfx/grenade-bounce.wav"), 0);

            m = deltax + (*tempg)->position.x;
            n = deltay + (*tempg)->position.y;

            // sprawdz ktora krawedz trojkata [number] dotyka granat
            // (przelec wszystkie cztery wierzcholki granatu)
            P_A = 0;
            if ((d = onWhichLine(number, m, n)) != -1) P_A = d;
            else if ((d = onWhichLine(number, m+(*tempg)->w, n)) != -1) P_A = d;
            else if ((d = onWhichLine(number, m+(*tempg)->w, n+(*tempg)->h)) != -1) P_A = d;
            else if ((d = onWhichLine(number, m, n+(*tempg)->h)) != -1) P_A = d;
            else cout << "CHUJA\n";
            POL_NUMBER = number;
            LINE_NUMBER = P_A;

            // V -=  2 * N * dot(V • N)

            (*tempg)->velocity.x -= (2*p.polygon[POL_NUMBER].perpendicular[LINE_NUMBER].x*
                                  p.polygon[POL_NUMBER].perpendicular[LINE_NUMBER].x*
                                  (*tempg)->velocity.x)*0.9;
            (*tempg)->velocity.y -= (2*p.polygon[POL_NUMBER].perpendicular[LINE_NUMBER].y*
                                  p.polygon[POL_NUMBER].perpendicular[LINE_NUMBER].y*
                                  (*tempg)->velocity.y)*0.9;


            /*// v' = 2K(K dot v) - v
            kx = p.polygon[POL_NUMBER].perpendicular[LINE_NUMBER].y;
            ky = -p.polygon[POL_NUMBER].perpendicular[LINE_NUMBER].x;

            tempg->velocity.x = (2*kx*kx*tempg->velocity.x - tempg->velocity.x);
            tempg->velocity.y = (2*ky*ky*tempg->velocity.y - tempg->velocity.y);
            */
            // k = P_A - P_B
            // K = k / |k|
            // v' = 2K(K dot v) - v
            /*kx = (float)(p.polygon[number].vertex[P_A].x - p.polygon[number].vertex[P_B].x);
            ky = (float)(p.polygon[number].vertex[P_A].y - p.polygon[number].vertex[P_B].y);

            sq = 2 / (kx*kx + ky*ky);

            tempg->velocity.x = (kx*kx*sq*tempg->velocity.x - tempg->velocity.x)*0.7;
            tempg->velocity.y = (ky*ky*sq*tempg->velocity.y - tempg->velocity.y)*0.7;*/



        }
        // if grenade touched bot, explode immediately
        /*else if ((which_bot = collision_det_with_bots(&(*tempg))) != -1)
        {
            tempg->timer_throw -= 4000;
            if (bot[which_bot]->procLife >= 0.8)
                bot[which_bot]->procLife -= 0.8;
            else
                killBot(which_bot, tempg->owner);
        }*/
        else
        {
            (*tempg)->position.x -= (*tempg)->velocity.x * aDeltaTime;
            //if (!collision_det(0, ammo[i].position.y-bot[i].velocity.x * aDeltaTime, &bot[i]))
            (*tempg)->position.y -= (*tempg)->velocity.y * aDeltaTime;
        }
        //tempg->position.x -= tempg->velocity.x * aDeltaTime;
        //tempg->position.y -= tempg->velocity.y * aDeltaTime;
    }
}
