/*   WeaponManager.cpp
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


#include "WeaponManager.h"
#include "ParserManager.h"
#include "boost/filesystem/fstream.hpp"
#include "Game.h"
#include "parser/SimpleIni.h"
#include "TexturesLoader.h"
#include <sstream>


WeaponManager::WeaponManager()
{

    std::cout << "Starting WeaponManager ... " << std::endl;

    std::cout << "   loading guns' information ..." << std::endl;

    // czytanie nazw broni

    std::string str;
    std::ifstream file((Parser.GAME_PATH + "Txt/weaponnames.txt").c_str());
    unsigned int i = 0;

    while (std::getline(file, str) && i < WEAPONS_NUMBER)
    {

        if (!str.empty() && (str[str.size() - 1] == '\r'))
            str.resize(str.size() - 1);

        if (str.empty()) continue;

        element[i].name = str;

        // change Barrett name to Barret
        if (element[i].name == "Barrett M82A1")
            element[i].name = "Barret M82A1";

        ++i;
    }

    file.close();

  //  str = element[0].name;
  //  element[0].name = element[10].name;
  //  element[10].name = str;

   // for (i = 0; i <= 10; i++)
     //   std::cout << "DF " << i << " " << element[i].name << std::endl;


    // czytanie wlasciwosci broni

    CSimpleIni ini(false, false, false);
    if (ini.LoadFile((Parser.GAME_PATH + "weapons.ini").c_str()) < 0)
    {
        std::cerr << "Cannot read 'weapons.ini' file" << std::endl;
        return;
    }

    i = 0;
    while (i < WEAPONS_NUMBER)
    {

        element[i].damage = static_cast<float>(ini.GetLongValue(element[i].name.c_str(), "Damage"));
        element[i].fireInterval = static_cast<float>(16.67 * ini.GetLongValue(element[i].name.c_str(), "FireInterval"));
        element[i].ammo = ini.GetLongValue(element[i].name.c_str(), "Ammo");
        element[i].reloadTime = static_cast<float>(16.67 * ini.GetLongValue(element[i].name.c_str(), "ReloadTime"));
        element[i].speed = static_cast<float>(ini.GetLongValue(element[i].name.c_str(), "Speed"));
        element[i].bulletStyle = ini.GetLongValue(element[i].name.c_str(), "BulletStyle");
        element[i].startUpTime = 16.67f * ini.GetLongValue(element[i].name.c_str(), "StartUpTime");
        element[i].bink = 16.67f * ini.GetLongValue(element[i].name.c_str(), "Bink");
        element[i].movementAcc = ini.GetLongValue(element[i].name.c_str(), "MovementAcc");
        element[i].recoil = ini.GetLongValue(element[i].name.c_str(), "Recoil");

        ++i;
    }


    std::cout << "   loading guns' textures ... " << std::endl;

    element[0].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "10");
    element[1].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "1");
    element[2].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "2");
    element[3].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "3");
    element[4].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "4");
    element[5].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "5");
    element[6].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "6");
    element[7].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "7");
    element[8].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "8");
    element[9].textureGun  = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "9");
    element[10].textureGun = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "0");
    element[14].textureGun = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "knife");
    element[15].textureGun = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "chainsaw");
    element[16].textureGun = Texture::LoadExt(Parser.GAME_PATH + "Interface-gfx/Guns/", "law");


    std::cout << "   loading bullets' textures ... " << std::endl;

    element[0].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "minigun-bullet");
    element[1].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "eagles-bullet");
    element[2].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "ak74-bullet");
    element[3].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "ak74-bullet");
    element[4].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "steyraug-bullet");
    element[5].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "spas12-bullet");
    element[6].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "ruger77-bullet");
    element[7].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "m79-bullet");
    element[8].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "barretm82-bullet");
    element[9].textureAmmo  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "spas12-bullet");
    element[10].textureAmmo = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "spas12-bullet");


    std::cout << "   loading bullets' shells ... " << std::endl;

    element[0].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "minigun-shell");
    element[1].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "eagles-shell");
    element[2].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "ak74-shell");
    element[3].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "ak74-shell");
    element[4].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "steyraug-shell");
    element[5].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "spas12-shell");
    element[6].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "ruger77-shell");
    element[7].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "m79-shell");
    element[8].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "barretm82-shell");
    element[9].textureShell  = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "spas12-shell");
    element[10].textureShell = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "spas12-shell");


    std::cout << "   loading grenade's textures ... " << std::endl;

    text_cluster = Texture::LoadExt(Parser.INTERFACE_PATH, "nade");

    std::ostringstream oss;

    text_grenade[0] = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "cluster-grenade");
    for (i = 1; i < 17; ++i)
    {
        oss << i;
        text_grenade[i] = Texture::LoadExt(Parser.GAME_PATH + "Sparks-gfx/explosion/", "explode" + oss.str());
        oss.str("");
    }

    text_clustergrenade[0] = Texture::LoadExt(Parser.GAME_PATH + "Weapons-gfx/", "cluster");
    for (i = 1; i < 17; ++i)
    {
        oss << i;
        text_clustergrenade[i] = Texture::LoadExt(Parser.GAME_PATH + "Sparks-gfx/flames/", "explode" + oss.str());
        oss.str("");
    }
}


WeaponManager::~WeaponManager()
{

    std::cout << "Removing WeaponManager ... DONE" << std::endl;

}
