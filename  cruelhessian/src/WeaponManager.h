/*   WeaponManager.h
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


#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#include "Singleton.h"
#include "Globals.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <iostream>

#define WEAPONS_NUMBER 18

class WeaponBase
{
public:
    std::string name;
    Mix_Chunk *fireSound;
    Mix_Chunk *reloadSound;
    float damage;
    Uint32 fireInterval;
    unsigned int ammo;
    Uint32 reloadTime;
    float speed;
    int bulletStyle;
    float startUpTime;
    float bink;
    int movementAcc;
    int recoil;
    Tex textureAmmo;
    Tex textureGun;
};


class WeaponManager : public Singleton<WeaponManager>
{
public:

    //std::vector<WeaponBase> element;
    WeaponBase element[WEAPONS_NUMBER];

    WeaponManager();
    virtual ~WeaponManager();

};

#define Weapons WeaponManager::GetSingleton().element

#endif