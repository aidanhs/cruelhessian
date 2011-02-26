/*   InterfaceManager.cpp
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


#include "InterfaceManager.h"
#include <cstdio>
#include "WeaponManager.h"
#include "WorldMap.h"
#include "FontManager.h"
#include "BotManager.h"
#include "Game.h"
#include "Bot.h"
#include "Jet.h"
#include "TexturesLoader.h"
#include "ParserManager.h"
#include <fstream>
#include "boost/filesystem/fstream.hpp"
//#include "InterfaceElement.h"


template<class T> T read_bin(std::istream& is)
{
    T tmp;
    is.read(reinterpret_cast<char*>(&tmp), sizeof(T));
    return tmp;
}


void InterfaceManager::Parse()
{

    std::string temp = Parser.INTERFACE_PATH + "setup.SIF";
    std::ifstream is(temp.c_str(), std::ios::binary);

    //m_cInterfaceAlpha = read_bin<unsigned char>(is);
    m_cInterfaceAlpha = read_bin<unsigned char>(is);
    m_cInterfaceAlpha = read_bin<unsigned char>(is);

    m_xHealthIcon.visible = read_bin<bool>(is);
    m_xHealthBar.visible = m_xHealthIcon.visible;

    m_xAmmoIcon.visible = read_bin<bool>(is);
    m_xAmmoBar.visible = m_xAmmoIcon.visible;
    m_xAmmoCount.visible = m_xAmmoIcon.visible;

    m_xVestBar.visible = read_bin<bool>(is);

    m_xJetIcon.visible = read_bin<bool>(is);
    m_xJetBar.visible = m_xJetIcon.visible;

    m_xNade.visible = read_bin<bool>(is);
    m_xWeaponName.visible = read_bin<bool>(is);

    m_xFireIcon.visible = read_bin<bool>(is);
    m_xFireBar.visible = m_xFireIcon.visible;

    m_xTeamBox.visible = read_bin<bool>(is);
    m_xPingDot.visible = read_bin<bool>(is);
    m_xStatus.visible = read_bin<bool>(is);

    m_xHealthIcon.x = read_bin<int>(is);
    m_xHealthIcon.y = read_bin<int>(is);
    m_xHealthIcon.angle = read_bin<int>(is);

    m_xHealthBar.x = read_bin<int>(is);
    m_xHealthBar.y = read_bin<int>(is);
    m_xHealthBar.width = read_bin<int>(is);
    m_xHealthBar.height = read_bin<int>(is);

    m_xHealthIcon.type = (TYPE_BAR)read_bin<int>(is);
    m_xHealthBar.type = m_xHealthIcon.type;

    m_xHealthBar.angle = read_bin<int>(is);

    m_xAmmoIcon.x = read_bin<int>(is);
    m_xAmmoIcon.y = read_bin<int>(is);
    m_xAmmoIcon.angle = read_bin<int>(is);

    m_xAmmoBar.x = read_bin<int>(is);
    m_xAmmoBar.y = read_bin<int>(is);
    m_xAmmoBar.width = read_bin<int>(is);
    m_xAmmoBar.height = read_bin<int>(is);

    m_xAmmoIcon.type = (TYPE_BAR)read_bin<int>(is);
    m_xAmmoBar.type = m_xAmmoIcon.type;
    m_xAmmoCount.type = m_xAmmoIcon.type;

    m_xAmmoBar.angle = read_bin<int>(is);
    m_xAmmoCount.angle = m_xAmmoBar.angle;

    m_xJetIcon.x = read_bin<int>(is);
    m_xJetIcon.y = read_bin<int>(is);
    m_xJetIcon.angle = read_bin<int>(is);

    m_xJetBar.x = read_bin<int>(is);
    m_xJetBar.y = read_bin<int>(is);
    m_xJetBar.width = read_bin<int>(is);
    m_xJetBar.height = read_bin<int>(is);

    m_xJetIcon.type = (TYPE_BAR)read_bin<int>(is);
    m_xJetBar.type = m_xJetIcon.type;

    m_xJetBar.angle = read_bin<int>(is);

    m_xVestBar.x = read_bin<int>(is);
    m_xVestBar.y = read_bin<int>(is);
    m_xVestBar.width = read_bin<int>(is);
    m_xVestBar.height = read_bin<int>(is);
    m_xVestBar.type = (TYPE_BAR)read_bin<int>(is);
    m_xVestBar.angle = read_bin<int>(is);

    m_xNade.x = read_bin<int>(is);
    m_xNade.y = read_bin<int>(is);
    m_xNade.width = m_xClusterNade.width = read_bin<int>(is);
    m_xNade.height = m_xClusterNade.height = read_bin<int>(is);
    m_xNade.type = (TYPE_BAR)read_bin<int>(is);

    m_xAmmoCount.x = read_bin<int>(is);
    m_xAmmoCount.y = read_bin<int>(is);

    m_xWeaponName.x = read_bin<int>(is);
    m_xWeaponName.y = read_bin<int>(is);

    m_xFireBar.x = read_bin<int>(is);
    m_xFireBar.y = read_bin<int>(is);
    m_xFireBar.angle = read_bin<int>(is);
    m_xFireIcon.x = read_bin<int>(is);
    m_xFireIcon.y = read_bin<int>(is);
    m_xFireIcon.width = read_bin<int>(is);
    m_xFireIcon.height = read_bin<int>(is);

    m_xFireIcon.type = (TYPE_BAR)read_bin<int>(is);
    m_xFireBar.type = m_xFireIcon.type;

    m_xFireIcon.angle = read_bin<int>(is);

    m_xTeamBox.x = read_bin<int>(is);
    m_xTeamBox.y = read_bin<int>(is);

    m_xPingDot.x = read_bin<int>(is);
    m_xPingDot.y = read_bin<int>(is);

    m_xStatus.x = read_bin<int>(is);
    m_xStatus.y = read_bin<int>(is);

    is.close();

}


InterfaceManager::InterfaceManager()
{

    std::cout << "Starting InterfaceManager ..." << std::endl;

    m_xHealthBar.texture = HealthBar.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "health-bar");
    m_xHealthIcon.texture = HealthIcon.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "health");
    m_xAmmoBar.texture = AmmoBar.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "reload-bar");
    m_xAmmoIcon.texture = AmmoIcon.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "ammo");
    m_xJetBar.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "jet-bar");

    m_xJetIcon.texture = JetIcon.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "jet");
    m_xFireBar.texture = FireBar.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "fire-bar");
    m_xFireIcon.texture = FireIcon.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "fire-bar-r");
    m_xVestBar.texture = VestBar.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "vest-bar");
    m_xNade.texture = Nade.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "nade");
    m_xClusterNade.texture = ClusterNade.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "cluster-nade");
    m_xTeamBox.texture = TeamBox.texture = Texture::LoadExt(Parser.INTERFACE_PATH, "back");


    //std::cout << "Set default alignment\n";
    m_xHealthIcon.x = 5;
    m_xHealthIcon.y = 445;
    m_xHealthBar.x = 45;
    m_xHealthBar.y = 455;
    m_xAmmoIcon.x = 285;
    m_xAmmoIcon.y = 445;
    m_xAmmoBar.x = 352;
    m_xAmmoBar.y = 455;
    m_xAmmoCount.x = 315;
    m_xAmmoCount.y = 455;
    m_xJetIcon.x = 480;
    m_xJetIcon.y = 445;
    m_xJetBar.x = 520;
    m_xJetBar.y = 455;
    m_xFireIcon.x = 409;
    m_xFireIcon.y = 470;
    m_xFireBar.x = 409;
    m_xFireBar.y = 470;
    m_xVestBar.x = 45;
    m_xVestBar.y = 465;
    m_xNade.x = m_xClusterNade.x = 305;
    m_xNade.y = m_xClusterNade.y = 468;
    m_xPingDot.x = 600;
    m_xPingDot.y = 18;
    m_xStatus.x = 590;
    m_xStatus.y = 421;
    m_xTeamBox.x = 583;
    m_xTeamBox.y = 330;
    m_xWeaponName.x = 182;
    m_xWeaponName.y = 460;


    if (boost::filesystem::exists(Parser.INTERFACE_PATH + "setup.SIF"))
    {
        Parse();
    }

    if (m_xHealthBar.width == 0 || m_xHealthBar.height == 0)
    {
        m_xHealthBar.width = m_xHealthBar.texture.w;
        m_xHealthBar.height = m_xHealthBar.texture.h;
    }
    if (m_xHealthIcon.width == 0 || m_xHealthIcon.height == 0)
    {
        m_xHealthIcon.width = m_xHealthIcon.texture.w;
        m_xHealthIcon.height = m_xHealthIcon.texture.h;
    }
    if (m_xAmmoBar.width == 0 || m_xAmmoBar.height == 0)
    {
        m_xAmmoBar.width = m_xAmmoBar.texture.w;
        m_xAmmoBar.height = m_xAmmoBar.texture.h;
    }
    if (m_xAmmoIcon.width == 0 || m_xAmmoIcon.height == 0)
    {
        m_xAmmoIcon.width = m_xAmmoIcon.texture.w;
        m_xAmmoIcon.height = m_xAmmoIcon.texture.h;
    }
    if (m_xJetBar.width == 0 || m_xJetBar.height == 0)
    {
        m_xJetBar.width = m_xJetBar.texture.w;
        m_xJetBar.height = m_xJetBar.texture.h;
    }
    if (m_xJetIcon.width == 0 || m_xJetIcon.height == 0)
    {
        m_xJetIcon.width = m_xJetIcon.texture.w;
        m_xJetIcon.height = m_xJetIcon.texture.h;
    }
    if (m_xFireBar.width == 0 || m_xFireBar.height == 0)
    {
        m_xFireBar.width = m_xFireBar.texture.w;
        m_xFireBar.height = m_xFireBar.texture.h;
    }
    if (m_xFireIcon.width == 0 || m_xFireIcon.height == 0)
    {
        m_xFireIcon.width = m_xFireIcon.texture.w;
        m_xFireIcon.height = m_xFireIcon.texture.h;
    }
    if (m_xVestBar.width == 0 || m_xVestBar.height == 0)
    {
        m_xVestBar.width = m_xVestBar.texture.w;
        m_xVestBar.height = m_xVestBar.texture.h;
    }
    if (m_xNade.width == 0 || m_xNade.height == 0)
    {
        m_xNade.width = m_xClusterNade.width = m_xNade.texture.w;
        m_xNade.height = m_xClusterNade.height = m_xNade.texture.h;
    }
    if (m_xTeamBox.width == 0 || m_xTeamBox.height == 0)
    {
        m_xTeamBox.width = m_xTeamBox.texture.w;
        m_xTeamBox.height = m_xTeamBox.texture.h;
    }


    jetBar.Set(m_xJetBar.width, m_xJetBar.texture);

    // change positions

    HealthIcon.x = m_xHealthIcon.x * Parser.MAX_WIDTH / 640.f;
    HealthIcon.y = m_xHealthIcon.y * Parser.MAX_HEIGHT / 480.f;
    HealthIcon.width = m_xHealthIcon.width;
    HealthIcon.height = m_xHealthIcon.height;
    HealthIcon.angle = m_xHealthIcon.angle;
    HealthIcon.type = m_xHealthIcon.type;
    HealthIcon.visible = m_xHealthIcon.visible;

    HealthBar.x = m_xHealthBar.x - m_xHealthIcon.x + HealthIcon.x;       // relative to HealthIcon
    HealthBar.y = m_xHealthBar.y - m_xHealthIcon.y + HealthIcon.y;       // relative to HealthIcon
    HealthBar.width = m_xHealthBar.width;
    HealthBar.height = m_xHealthBar.height;
    HealthBar.angle = m_xHealthBar.angle;
    HealthBar.type = m_xHealthIcon.type;
    HealthBar.visible = m_xHealthIcon.visible;


    JetIcon.x = m_xJetIcon.x * Parser.MAX_WIDTH / 640.f;
    JetIcon.y = m_xJetIcon.y * Parser.MAX_HEIGHT / 480.f;
    JetIcon.width = m_xJetIcon.width;
    JetIcon.height = m_xJetIcon.height;
    JetIcon.angle = m_xJetIcon.angle;
    JetIcon.type = m_xJetIcon.type;
    JetIcon.visible = m_xJetIcon.visible;

    jetBar.x = m_xJetBar.x - m_xJetIcon.x + JetIcon.x;       // relative to JetIcon;
    jetBar.y = m_xJetBar.y - m_xJetIcon.y + JetIcon.y;       // relative to JetIcon;
    jetBar.width = m_xJetBar.width;
    jetBar.height = m_xJetBar.height;
    jetBar.angle = m_xJetBar.angle;
    jetBar.type = m_xJetIcon.type;
    jetBar.visible = m_xJetIcon.visible;


    AmmoIcon.x = m_xAmmoIcon.x * Parser.MAX_WIDTH / 640.f;
    AmmoIcon.y = m_xAmmoIcon.y * Parser.MAX_HEIGHT / 480.f;
    AmmoIcon.width = m_xAmmoIcon.width;
    AmmoIcon.height = m_xAmmoIcon.height;
    AmmoIcon.angle = m_xAmmoIcon.angle;
    AmmoIcon.type = m_xAmmoIcon.type;
    AmmoIcon.visible = m_xAmmoIcon.visible;

    AmmoBar.x = (m_xAmmoBar.x - m_xAmmoIcon.x) + AmmoIcon.x;       // relative to AmmoIcon
    AmmoBar.y = (m_xAmmoBar.y - m_xAmmoIcon.y  + AmmoIcon.y);//* Parser.MAX_HEIGHT / 480.f;       // relative to AmmoIcon
    //AmmoBar.x = m_xAmmoBar.x - m_xJetIcon.x + JetIcon.x;       // relative to AmmoIcon
    //AmmoBar.y = m_xAmmoBar.y - m_xJetIcon.y + JetIcon.y;       // relative to AmmoIcon
    AmmoBar.width = m_xAmmoBar.width;
    AmmoBar.height = m_xAmmoBar.height;
    AmmoBar.angle = m_xAmmoBar.angle;
    AmmoBar.type = m_xAmmoIcon.type;
    AmmoBar.visible = m_xAmmoIcon.visible;

    AmmoCount.x = m_xAmmoCount.x - m_xAmmoIcon.x + AmmoIcon.x;     // relative to AmmoIcon;
    AmmoCount.y = m_xAmmoCount.y - m_xAmmoIcon.y + AmmoIcon.y;     // relative to AmmoIcon;
    AmmoCount.angle = m_xAmmoBar.angle;
    AmmoCount.type = m_xAmmoIcon.type;
    AmmoCount.visible = m_xAmmoIcon.visible;




    VestBar.x = m_xVestBar.x - m_xHealthBar.x + HealthBar.x;     // relative to HealthBar;
    VestBar.y = m_xVestBar.y - m_xHealthBar.y + HealthBar.y;     // relative to HealthBar
    VestBar.width = m_xVestBar.width;
    VestBar.height = m_xVestBar.height;
    VestBar.angle = m_xVestBar.angle;
    VestBar.type = m_xVestBar.type;
    VestBar.visible = m_xVestBar.visible;

    Nade.x = m_xNade.x - m_xAmmoIcon.x + AmmoIcon.x;     // relative to AmmoIcon;
    Nade.y = m_xNade.y - m_xAmmoIcon.y + AmmoIcon.y;     // relative to AmmoIcon;
    Nade.width = m_xNade.width;
    Nade.height = m_xNade.height;
    Nade.type = m_xNade.type;
    Nade.visible = ClusterNade.visible = m_xNade.visible;



    WeaponName.x = m_xWeaponName.x - m_xHealthIcon.x + HealthIcon.x;     // relative to HealthIcon;
    WeaponName.y = m_xWeaponName.y - m_xHealthIcon.y + HealthIcon.y;     // relative to HealthIcon;
    //WeaponName.x =  m_xWeaponName.x* Parser.MAX_WIDTH / 640.f;
    //WeaponName.y = m_xWeaponName.y* Parser.MAX_HEIGHT / 480.f;
    WeaponName.visible = m_xWeaponName.visible;

    FireBar.x = m_xFireBar.x - m_xAmmoIcon.x + AmmoIcon.x;     // relative to AmmoIcon;
    FireBar.y = m_xFireBar.y - m_xAmmoIcon.y + AmmoIcon.y;     // relative to AmmoIcon;
    //FireBar.x = m_xFireBar.x - m_xJetIcon.x + JetIcon.x;     // relative to AmmoIcon;
    //FireBar.y = m_xFireBar.y - m_xJetIcon.y + JetIcon.y;     // relative to AmmoIcon;
    FireBar.width = m_xFireBar.width;
    FireBar.height = m_xFireBar.height;
    FireBar.angle = m_xFireBar.angle;
    FireBar.type = m_xFireIcon.type;
    FireBar.visible = m_xFireIcon.visible;

    FireIcon.x = m_xFireIcon.x - m_xAmmoIcon.x + AmmoIcon.x;     // relative to AmmoIcon;
    FireIcon.y = m_xFireIcon.y - m_xAmmoIcon.y + AmmoIcon.y;     // relative to AmmoIcon;
    FireIcon.width = m_xFireIcon.width;
    FireIcon.height = m_xFireIcon.height;
    FireIcon.angle = m_xFireIcon.angle;
    FireIcon.type = m_xFireIcon.type;
    FireIcon.visible = m_xFireIcon.visible;

    //TeamBox.x = m_xTeamBox.x  - m_xJetIcon.x + JetIcon.x;       // relative to JetIcon;
    TeamBox.x = m_xTeamBox.x  * Parser.MAX_WIDTH / 640.f;;       // relative to JetIcon;
    //TeamBox.y = m_xTeamBox.y  - m_xJetIcon.y + JetIcon.y;       // relative to JetIcon;
    TeamBox.y = m_xTeamBox.y  * Parser.MAX_HEIGHT / 480.f;       // relative to JetIcon;
    TeamBox.width = m_xTeamBox.width;
    TeamBox.height = m_xTeamBox.height;
    TeamBox.visible = m_xTeamBox.visible;

    PingDot.x = m_xPingDot.x  - m_xJetIcon.x + JetIcon.x;       // relative to JetIcon;
    PingDot.y = m_xPingDot.y  - m_xJetIcon.y + JetIcon.y;       // relative to JetIcon;
    PingDot.visible = m_xPingDot.visible;

    //Status.x = m_xStatus.x  - m_xJetIcon.x + JetIcon.x;       // relative to JetIcon;
    Status.x = m_xStatus.x  * Parser.MAX_WIDTH / 640.f;       // relative to JetIcon;
    //Status.y = m_xStatus.y  - m_xJetIcon.y + JetIcon.y;       // relative to JetIcon;
    Status.y = m_xStatus.y  * Parser.MAX_HEIGHT / 480.f;       // relative to JetIcon;
    Status.visible = m_xStatus.visible;

    if (!HealthIcon.visible) std::cout << "NIE MA HEALTH\n";
    if (!JetIcon.visible) std::cout << "NIE MA JET\n";
    if (!AmmoIcon.visible) std::cout << "NIE MA AMMO\n";
    if (!FireIcon.visible) std::cout << "NIE MA FIRE\n";

}


void InterfaceManager::ReloadBar()
{

    AmmoBar.width = static_cast<int>(m_xAmmoBar.width * (world.getCurrentTime - world.bot[world.MY_BOT_NR]->startReloadingTime) / Weapons[world.bot[world.MY_BOT_NR]->gunModel].reloadTime);

}

InterfaceManager::~InterfaceManager()
{

    std::cout << "Removing InterfaceManager ... DONE" << std::endl;

}


inline static std::string IntToString(int num)
{
    char chars[128];
    sprintf(chars, "%i", num);
    return std::string(chars);
}


void InterfaceManager::Draw()
{

    std::string str;



    if (HealthIcon.visible)
    {

        // ----- Health Icon -----

        HealthIcon.Draw();


        // ----- Health Bar  ------

        float bar_prog = world.bot[world.MY_BOT_NR]->actLife / Bots.fullLife;
        if (HealthBar.type == BAR_TEXT)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(100 * bar_prog) + "%", Fonts.textCol[3],
                            HealthBar.x, HealthBar.y);
        }
        else
        {
            HealthBar.width = static_cast<int>(m_xHealthBar.width * bar_prog);
            HealthBar.Draw();
        }
    }


    if (VestBar.visible)
    {

        // ----- Vest Bar-----

        if (VestBar.type == BAR_TEXT)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(100 * world.bot[world.MY_BOT_NR]->procVest) + "%", Fonts.textCol[3],
                            VestBar.x, VestBar.y);
        }
        else
        {
            VestBar.width = static_cast<int>(m_xVestBar.width * world.bot[world.MY_BOT_NR]->procVest);
            VestBar.Draw();
        }
    }


    if (AmmoIcon.visible)
    {

        // ----- Ammo Icon -----

        AmmoIcon.Draw();


        // ----- Ammo Bar -----

        if (AmmoBar.type == BAR_TEXT)
        {
            //Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(world.bot[world.MY_BOT_NR]->leftAmmos), Fonts.textCol[3],
            //              AmmoBar.x, AmmoBar.y);
        }
        else
        {
            if (!world.bot[world.MY_BOT_NR]->isReloading)
                AmmoBar.width = m_xAmmoBar.width * world.bot[world.MY_BOT_NR]->leftAmmos / Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
            AmmoBar.Draw();
        }


    }



    if (FireIcon.visible)
    {
//std::cout <<"LF\n";
        // -----  Fire Icon -----

        FireIcon.Draw();


        // -----  Fire Bar -----

        float time_gold = static_cast<float>(world.getCurrentTime - world.bot[world.MY_BOT_NR]->lastShotTime) /
                          static_cast<float>(Weapons[world.bot[world.MY_BOT_NR]->gunModel].fireInterval);

        if (time_gold > 1.0f)
            time_gold = 1.0f;

        if (FireBar.type == BAR_TEXT)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(100*time_gold) + "%", Fonts.textCol[3],
                            FireBar.x, FireBar.y);
        }
        else
        {
            FireBar.width = static_cast<int>(m_xFireBar.width * time_gold);
            FireBar.Draw();
        }

    }


    if (JetIcon.visible)
    {

        // ----- Jet Icon -----

        JetIcon.Draw();


        // ----- Jet Bar -----

        jetBar.Draw(world.bot[world.MY_BOT_NR]->jet->GetAmount());

    }



    if (WeaponName.visible)
    {

        // ----- Gun's name -----

        Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[world.bot[world.MY_BOT_NR]->gunModel].name, Fonts.textCol[3],
                        WeaponName.x, WeaponName.y);
    }

    // ----- Ammo count -----

    Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(world.bot[world.MY_BOT_NR]->leftAmmos), Fonts.textCol[3],
                    AmmoCount.x, AmmoCount.y);





    if (Nade.visible)
    {

        // grenades' count
        if (world.bot[world.MY_BOT_NR]->numClusters == 0)
        {
            if (Nade.type == BAR_TEXT)
            {
                Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(world.bot[world.MY_BOT_NR]->numGrenades), Fonts.textCol[3],
                                Nade.x, Nade.y);
            }
            else if (world.bot[world.MY_BOT_NR]->numGrenades > 0)
            {
                m_xNade.x = Nade.x;
                m_xNade.y = Nade.y;
                for (unsigned int i = 0; i < world.bot[world.MY_BOT_NR]->numGrenades; ++i)
                {
                    if (Nade.type == BAR_HORIZONTAL)
                        m_xNade.x += 10;
                    else if (Nade.type == BAR_VERTICAL)
                        m_xNade.y += 10;
                    m_xNade.Draw();
                }
            }
        }

        // clusternades' count
        else if (world.bot[world.MY_BOT_NR]->numClusters > 0)
        {
            if (Nade.type == BAR_TEXT)
            {
                Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(world.bot[world.MY_BOT_NR]->numClusters), Fonts.textCol[3],
                                Nade.x, Nade.y);
            }
            else if (world.bot[world.MY_BOT_NR]->numClusters > 0)
            {
                m_xClusterNade.x = Nade.x;
                m_xClusterNade.y = Nade.y;
                for (unsigned int i = 0; i < world.bot[world.MY_BOT_NR]->numClusters; ++i)
                {
                    if (Nade.type == BAR_HORIZONTAL)
                        m_xClusterNade.x += 10;
                    else if (Nade.type == BAR_VERTICAL)
                        m_xClusterNade.y += 10;
                    m_xClusterNade.Draw();
                }
            }
        }

    }


    if (PingDot.visible)
    {

    }

    if (TeamBox.visible && game.CURRENT_GAME_MODE == CTF)
    {
        TeamBox.Draw();
        Fonts.printText(Fonts.font[0], Fonts.FontMenu, "0", Fonts.textCol[1], TeamBox.x, TeamBox.y);
        Fonts.printText(Fonts.font[0], Fonts.FontMenu, "0", Fonts.textCol[2], TeamBox.x, TeamBox.y + 20);
    }


    if (Status.visible)
    {

        // ----- Player status -----


        // my position / players number
        str = IntToString(world.MY_CURRENT_POS);
        str.append("/");
        str.append(IntToString(world.bot.size()));
        Fonts.printText(Fonts.font[1], Fonts.FontConsole, str, Fonts.textCol[4], Status.x, Status.y);

        // kills / points (points to leader)
        if (game.CURRENT_GAME_MODE == DM)
        {
            str = IntToString(world.bot[world.MY_BOT_NR]->killedNr);
        }
        else
        {
            str = IntToString(world.bot[world.MY_BOT_NR]->points);
        }

        str.append("(");
        if (world.DISTANCE_SCORE < 0)
        {
            str.append("-");
        }
        else
        {
            str.append("+");
        }
        str.append(IntToString(abs(world.DISTANCE_SCORE)));
        str.append(")");
        Fonts.printText(Fonts.font[1], Fonts.FontConsole, str, Fonts.textCol[1], Status.x, Status.y + 10);

        // kill / point / capture limit
        Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(Parser.FIRST_LIMIT),
                        Fonts.textCol[2], Status.x, Status.y + 20);

    }


}
