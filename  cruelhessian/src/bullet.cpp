/*   bullet.cpp
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


#include "bot.h"
#include "worldmap.h"
#include "bullet.h"


list<Bullet *> ammo_list;


Bullet::Bullet(float posx, float posy, int gunmodel, int owner)
{
    this->position.x = posx;
    this->position.y = posy;
    this->w = AMMO_WIDTH[gunmodel];
    this->h = AMMO_HEIGHT[gunmodel];
    this->gunModel = gunmodel;
    this->owner = owner;
    this->mass = gun_mass(gunmodel);
    this->massInv = 1 / this->mass;
    this->maxV = gun_speed(gunmodel);
}

/**
 * Funkcja wyznaczajac
 * @see co_robi
 */
int
Bullet::collision_det_with_bots()
{
    for (int i = 0; i < world->BOTS_NUM; i++)
    {
        if (!bot[i]->isKilled && i != this->owner)
        {
            if ((this->position.x < bot[i]->position.x+bot[i]->w) &&
                    (this->position.x+this->w > bot[i]->position.x) &&
                    (this->position.y < bot[i]->position.y+bot[i]->h) &&
                    (this->position.y+this->h > bot[i]->position.y))
                return i;
        }
    }
    return -1;
}

