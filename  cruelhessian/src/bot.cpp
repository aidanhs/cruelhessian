/*   bot.cpp
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

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "globals.h"
#include "bullet.h"
#include "bot.h"



Bot::Bot(unsigned int bot_nr, const std::string& _name, int spawn_nr, int gunmodel, TEAM _team)
{
    type = 0;
    botNumber = bot_nr;
    team = _team;
    name = _name;
    movementDirection = LEFT;
    movementType = STOI;
    position = TVector2D(p.spawnpoint[spawn_nr].x, p.spawnpoint[spawn_nr].y);

    // treat as a circle (ellipse - that will the best)
    w = SOL_WIDTH[STOI];
    h = SOL_HEIGHT[STOI];
    r = SOL_HEIGHT[movementType] / 2;

    velocity = TVector2D(0.0f, 0.0f);
    mass = 100;
    massInv = 1 / mass;
    maxSpeedX = 140;
    maxSpeedY = 150;
    isFlying = isReloading = isKilled = false;
    procJet = 1.0;
    actLife = fullLife;

    gunModel = gunmodel;
    respawnTime = 0;
    lastShotTime = 0;
    currentFrame = 0;
    timerChangeFrame = 0;
    timerRespawnTime = 0;
    youKilledTime = 0;
    destinationPoint = find_nearest_waypoint_nr(spawn_nr);
    leftAmmos = 1;
    numGrenades = 5;
    points = 0;
    deaths = 0;
}

/*
Bot::Bot(unsigned int bot_nr, const Bots& bots, int spawn_nr, TEAM _team)
{
    botNumber = bot_nr;
    team = _team;
    name = _name;
    w = SOL_WIDTH[STOI];
    h = SOL_HEIGHT[STOI];
    position = TVector2D(p.spawnpoint[spawn_nr].x - w/2, p.spawnpoint[spawn_nr].y - h/2);
    //this->shotPointX = this->position.x + this->w/2;
    //this->shotPointY = this->position.y + this->h/2;
    velocity = TVector2D(0.0f, 0.0f);
    mass = 100;
    massInv = 1 / mass;
    maxSpeedX = 140;
    maxSpeedY = 150;
    isFlying = isReloading = isKilled = false;
    procJet = 1.0;
    actLife = fullLife;
    movementDirection = LEFT;
    movementType = STOI;
    gunModel = gunmodel;
    respawnTime = 0;
    lastShotTime = 0;
    currentFrame = 0;
    timerChangeFrame = 0;
    timerRespawnTime = 0;
    youKilledTime = 0;
    destinationPoint = find_nearest_waypoint_nr(spawn_nr);
    leftAmmos = 1;
    numGrenades = 5;
    points = 0;
    deaths = 0;
}
*/
int Bot::find_nearest_waypoint_nr(int spawn_nr)
{
    double min_val = 0.0;
    int min_nr = 0;
    int i, j;

    for (i = 1; i < p.waypointCount; i++)
    {
        if (p.waypoint[i].path == 1 && (p.waypoint[i].right || p.waypoint[i].left || p.waypoint[i].up || p.waypoint[i].down || p.waypoint[i].jet))
        {
            min_val = pow(p.spawnpoint[spawn_nr].x-p.waypoint[i].x, 2.0) + pow(p.spawnpoint[spawn_nr].y-p.waypoint[i].y, 2.0);
            min_nr = i;
            break;
        }
    }

    for (j = i+1; j < p.waypointCount; j++)
    {
        if ((pow(p.spawnpoint[spawn_nr].x-p.waypoint[j].x, 2.0) + pow(p.spawnpoint[spawn_nr].y-p.waypoint[j].y, 2.0) < min_val) &&
                (p.waypoint[i].path == 1 && (p.waypoint[j].right || p.waypoint[j].left || p.waypoint[j].up || p.waypoint[j].down || p.waypoint[j].jet)))
        {
            min_val = pow(p.spawnpoint[spawn_nr].x-p.waypoint[j].x, 2.0) + pow(p.spawnpoint[spawn_nr].y-p.waypoint[j].y, 2.0);
            min_nr = j;
        }
    }
    return min_nr;
}

bool Bot::is_inside(int x, int y)
{
    return ((x > position.x) && (x < position.x + w) &&
            (y > position.y) && (y < position.y + h));
}

