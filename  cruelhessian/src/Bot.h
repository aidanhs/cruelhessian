#ifndef BOT_H
#define BOT_H

/*   Bot.h
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



#include <iostream>
#include <vector>

#include "MovingObject.h"
#include "Enums.h"
#include "SDL.h"
#include "TVector2D.h"


class Bot : public MovingObject
{

    void draw_gostek_help(int part);
    float getAngle(int x, int y);
    float getAngleTemp(float x, float y);

public:

    std::string name;
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;

    unsigned int points;

    std::vector<std::vector<unsigned char> > color;
    unsigned int deaths;
    int wayNumber;
    unsigned int number;
    unsigned int killer;
    unsigned int killed;
    unsigned int killedNr;
    unsigned int ping;
    unsigned int gunModel;
    unsigned int numGrenades;
    unsigned int leftAmmos;
    unsigned int currentFrame;       // aktualnie wyswietlana ramka
    int destinationPoint;
    Uint32 lastShotTime;
    Uint32 startReloadingTime;
    Uint32 timerRespawnTime;
    Uint32 respawnTime;
    Uint32 timerChangeFrame;
    Uint32 youKilledTime;

    bool isShooting;
    bool isKilled;
    bool isAbleToFly;
    bool isReloading;
    bool isAbleToJump;

    TVector2D shotPoint;
    float procJet;
    float actLife;

    TEAM team;

    MT movementType;        // sposob poruszania sie
    MD movementDirection;   // kierunek poruszania sie bota

    Bot(const std::string& _name, float spawn_x, float spawn_y, int gunmodel, TEAM _team, unsigned int bot_nr, int dest);


    void draw();
    void update();
    bool is_inside(int x, int y);
   //void AccumulateForces();

};

#endif

