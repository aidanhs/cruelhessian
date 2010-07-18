/*   BotManager.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawel Konieczny <konp84 at gmail.com>
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


#ifndef BOTMANAGER_H
#define BOTMANAGER_H

#include "Singleton.h"
#include "Globals.h"
#include "TVector2D.h"
#include <iostream>
#include <vector>


class BotsBase
{
public:
    std::string name;
    int favouriteWeapon;
    unsigned int* color[4];
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;
};



class BotManager : public Singleton<BotManager>
{
public:
    Tex gostek[9][2];
    float mass;
    float massInv;
    TVector2D maxSpeed;
    unsigned int FRAMES_MAX[50];
    Frame frame[20][100][100][2];
    BotManager();
    virtual ~BotManager();
    std::vector<BotsBase> element;
    TVector2D renderInfo[23];
    BODY indices[15];
    // protected:
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
