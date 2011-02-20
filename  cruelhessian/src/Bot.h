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
#include "Enums.h"
#include "TVector2D.h"
#include "Drawable.h"
#include "Body.h"

class Weapon;


class Bot : public Drawable, public Body
{

    void draw_gostek_help(int part, BODY_COLOR xbody) const;
    Bot& operator=(const Bot&) {return *this;}

public:
//Body *bod;
    bool MODE_BERSERKER, MODE_PREDATOR, MODE_FLAMEGOD;
//    Body *body;
    Weapon* weapon;
    std::string name;
    TEAM team;
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;
    void setMoveLeft(bool shouldMove)
    {
        moveLeft = shouldMove;
    };
    void setMoveRight(bool shouldMove)
    {
        moveRight = shouldMove;
    };
    void setMoveUp(bool shouldMove)
    {
        moveUp = shouldMove;
    };
    void setMoveDown(bool shouldMove)
    {
        moveDown = shouldMove;
    };
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
    unsigned int numClusters;
    unsigned int leftAmmos;
    unsigned int currentFrame;       // aktualnie wyswietlana ramka
    int destinationPoint;
    float lastShotTime;
    float startReloadingTime;
    float timerRespawnTime;
    float respawnPeriod;
    float timerChangeFrame;
    float timeGetSuperbonus;
    float youKilledTime;

    bool isShooting;
    bool isKilled;
    bool isAbleToFly;
    bool isReloading;
    bool isAbleToJump;

    bool moveLeft;
    bool moveRight;
    bool moveUp;
    bool moveDown;
    bool changeMove;

    TVector2D shotPoint;
    float procJet;
    float procVest;
    float actLife;



    MT movementType;        // sposob poruszania sie
    MD movementDirection;   // kierunek poruszania sie bota

    Bot(const std::string& _name, const TVector2D& spawn, int gunmodel, TEAM _team, unsigned int bot_nr, int dest);
    ~Bot(void) {}


    void Draw() const;
	void Shot(const TVector2D& dest);
	void ThrowGrenade(const TVector2D& dest, float push_time);
	void ThrowCluster(const TVector2D& dest, float push_time);
    void Update();
    bool is_inside(int x, int y);
    //void AccumulateForces();

};

#endif

