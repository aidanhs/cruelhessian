/*   WeaponManager.cpp
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


#include "WeaponManager.h"
#include "boost/filesystem/fstream.hpp"
#include "Game.h"
#include "parser/SimpleIni.h"
#include "TexturesLoader.h"


WeaponManager::WeaponManager()
{

    std::cout << "Weapon Manager ... " << std::endl;

    std::cout << "   loading guns' information ..." << std::endl;

    CSimpleIni ini(false, false, false);
    if (ini.LoadFile((game.SOL_PATH + "weapons.ini").c_str()) < 0)
    {
        std::cerr << "Cannot read 'weapons.ini' file" << std::endl;
        return;
    }

    std::string str;
    std::ifstream file((game.SOL_PATH + "Txt/weaponnames.txt").c_str());
    int i = 0;

    while (std::getline(file, str) && i < WEAPONS_NUMBER)
    {
        if (!str.empty() && (str[str.size() - 1] == '\r'))
            str.resize(str.size() - 1);

        if (str.empty()) continue;

        element[i].name = str;

        // change Barrett name to Barret
        if (element[i].name == "Barrett M82A1")
            element[i].name = "Barret M82A1";

        //std::cout << "   loading gun " << ++i << " : " << temp.name << std::endl;

        element[i].damage = static_cast<float>(ini.GetLongValue(element[i].name.c_str(), "Damage"));
        element[i].fireInterval = static_cast<Uint32>(16.67 * ini.GetLongValue(element[i].name.c_str(), "FireInterval"));
        element[i].ammo = ini.GetLongValue(element[i].name.c_str(), "Ammo");
        element[i].reloadTime = static_cast<Uint32>(16.67 * ini.GetLongValue(element[i].name.c_str(), "ReloadTime"));
        element[i].speed = static_cast<float>(ini.GetLongValue(element[i].name.c_str(), "Speed"));
        element[i].bulletStyle = ini.GetLongValue(element[i].name.c_str(), "BulletStyle");
        element[i].startUpTime = 16.67f * ini.GetLongValue(element[i].name.c_str(), "StartUpTime");
        element[i].bink = 16.67f * ini.GetLongValue(element[i].name.c_str(), "Bink");
        element[i].movementAcc = ini.GetLongValue(element[i].name.c_str(), "MovementAcc");
        element[i].recoil = ini.GetLongValue(element[i].name.c_str(), "Recoil");

        //element.push_back(temp);
        i++;
    }

    file.close();


    std::cout << "   loading guns' textures ... " << std::endl;

    element[0].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "10");
    element[1].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "1");
    element[2].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "2");
    element[3].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "3");
    element[4].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "4");
    element[5].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "5");
    element[6].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "6");
    element[7].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "7");
    element[8].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "8");
    element[9].textureGun  = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "9");
    element[10].textureGun = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "0");
    element[14].textureGun = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "knife");
    element[15].textureGun = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "chainsaw");
    element[16].textureGun = SOIL_LoadTextureEx2(game.SOL_PATH + "Interface-gfx/Guns/", "law");


    std::cout << "   loading bullets' textures ... " << std::endl;

    element[0].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "minigun-bullet");
    element[1].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "eagles-bullet");
    element[2].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "ak74-bullet");
    element[3].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "ak74-bullet");
    element[4].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "steyraug-bullet");
    element[5].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "spas12-bullet");
    element[6].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "ruger77-bullet");
    element[7].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "m79-bullet");
    element[8].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "barretm82-bullet");
    element[9].textureAmmo  = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "spas12-bullet");
    element[10].textureAmmo = SOIL_LoadTextureEx2(game.SOL_PATH + "Weapons-gfx/", "spas12-bullet");

}

WeaponManager::~WeaponManager()
{
    std::cout << "Removing WeaponManager ..." << std::endl;
//    element.clear();
}
