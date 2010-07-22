#ifndef BOT_MANAGER_H
#define BOT_MANAGER_H

/*   BotManager.h
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

#include "Singleton.h"
#include "Enums.h"
#include "Tex.h"
#include "TVector2D.h"



class Frame
{

public:
    float x, y, r;
};


class BotsBase
{
public:

    BotsBase()
    {
        color.resize(4);
        for (int i = 0; i < 4; ++i)
        {
            color[i].resize(3);
        }
    }

    std::string name;
    int favouriteWeapon;
    std::vector<std::vector<unsigned char> > color;
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;
};



class BotManager : public Singleton<BotManager>
{
public:

    BotManager();
    virtual ~BotManager();

    Tex gostek[9][2];
    float mass;
    float massInv;
    TVector2D maxSpeed;
    unsigned int FRAMES_MAX[50];
    Frame frame[20][100][100][2];

    std::vector<BotsBase> element;
    TVector2D renderInfo[23];
    BODY indices[15];

private:

    float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie (prawo, lewo)
    std::string anim_type(MT name);
    int read_poa(const MT name);
    float getAngle(float x, float y);
    float GetAngle(int a, int b, int c, int x, int y);
    int getWeaponNumber(const std::string& gun);
};

#define Bots BotManager::GetSingleton()

#endif
