#ifndef WEAPON_MANAGER_H
#define WEAPON_MANAGER_H


/*   WeaponManager.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
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




#include "Singleton.h"
#include "TexturesLoader.h"
#include <iostream>



class WeaponManager : public Singleton<WeaponManager>
{
    struct WeaponBase
    {
        std::string name;
        float fireInterval;
        float reloadTime;
        float damage;
        unsigned int ammo;
        float speed;
        int bulletStyle;
        float startUpTime;
        float bink;
        int movementAcc;
        int recoil;
        Tex textureAmmo;
        Tex textureGun;
    };

public:

    static const unsigned int WEAPONS_NUMBER = 18;

    WeaponBase element[WEAPONS_NUMBER];
    Tex text_grenade[17];
    Tex text_clustergrenade[17];
    Tex text_cluster;

    WeaponManager();
    virtual ~WeaponManager();

};

#define Weapons WeaponManager::GetSingleton().element

#endif
