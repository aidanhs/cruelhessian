/*   bot.h
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

#ifndef BOT_H
#define BOT_H

#include <iostream>
#include "moving_object.h"
#include "globals.h"
//#include "SDL.h"


class Bot : public MovingObject
{
    const Tex* texture[9][2];
    void draw_gostek_help(int partx, int party, float angle, const Tex* xtex);
    float getAngle(int x, int y);

public:

    std::string name;
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;

    unsigned int points;
    //float r;
   // Uint8 *colorShirt;
    unsigned int color[4][3];
    unsigned int deaths;
    int wayNumber;
    unsigned int botNumber;
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
    bool isFlying;
    bool isReloading;

    TVector2D shotPoint;
    float procJet;
    float actLife;

    TEAM team;

    MT movementType;        // sposob poruszania sie
    MD movementDirection;   // kierunek poruszania sie bota

    Bot(const std::string& _name, float spawn_x, float spawn_y, int gunmodel, TEAM _team, unsigned int bot_nr, int dest, Tex text[9][2]);


    void draw();
    void update();
    bool is_inside(int x, int y);

};



#endif
