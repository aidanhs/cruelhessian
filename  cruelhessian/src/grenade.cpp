/*   grenade.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84 at gmail.com>
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

#include "grenade.h"
#include "bot.h"
#include "worldmap.h"


std::list<Grenade *> gren_list;

Grenade::Grenade(float src_x, float src_y, float dest_x, float dest_y, int _owner, Uint32 push_time)
{
    type = 0;
    position = TVector2D(src_x, src_y);
    w = 10;
    h = 10;
    r = 5;
    owner = _owner;
    timer_change_frame = 0;
    currentFrame = 0;
    mass = 500;
    massInv = 1 / mass;
    maxSpeedX = 100;
    maxSpeedY = 100;

    float tang = (dest_y - src_y) / (dest_x - src_x);
    float sq = push_time / sqrt(1+tang*tang);
    velocity = (dest_x - src_x > 0) ? TVector2D(-sq, -sq * tang) : TVector2D(sq, sq * tang);

    timer_throw = SDL_GetTicks();
}

int Grenade::collision_det_with_bots()
{
    for (unsigned int i = 0; i < bot.size(); i++)
    {
        if (!bot[i]->isKilled)
        {
            if ((position.x < bot[i]->position.x + bot[i]->w) &&
                    (position.x + w > bot[i]->position.x) &&
                    (position.y < bot[i]->position.y + bot[i]->h) &&
                    (position.y + h > bot[i]->position.y))
                return i;
        }
    }
    return -1;
}

