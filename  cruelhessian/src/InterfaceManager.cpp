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
#include "TexturesLoader.h"
#include "ParserManager.h"



InterfaceManager::InterfaceManager(void) :
    text_health      (Texture::LoadExt(Parser.INTERFACE_PATH, "health")),
    text_healthbar   (Texture::LoadExt(Parser.INTERFACE_PATH, "health-bar")),
    text_ammo        (Texture::LoadExt(Parser.INTERFACE_PATH, "ammo")),
    text_nade        (Texture::LoadExt(Parser.INTERFACE_PATH, "nade")),
    text_clusternade (Texture::LoadExt(Parser.INTERFACE_PATH, "cluster-nade")),
    text_reloadbar   (Texture::LoadExt(Parser.INTERFACE_PATH, "reload-bar")),
    text_jet         (Texture::LoadExt(Parser.INTERFACE_PATH, "jet")),
    text_jetbar      (Texture::LoadExt(Parser.INTERFACE_PATH, "jet-bar")),
    text_firebar     (Texture::LoadExt(Parser.INTERFACE_PATH, "fire-bar")),
    text_firebar_r   (Texture::LoadExt(Parser.INTERFACE_PATH, "fire-bar-r")),
    text_vestbar     (Texture::LoadExt(Parser.INTERFACE_PATH, "vest-bar"))
{

    std::cout << "Starting InterfaceManager ..." << std::endl;

    // std::cout << "   loading textures ... " << std::endl;

}


void InterfaceManager::ReloadBar(void)
{

    text_reloadbar.w = static_cast<int>(115 * (world.getCurrentTime - world.bot[world.MY_BOT_NR]->startReloadingTime) / Weapons[world.bot[world.MY_BOT_NR]->gunModel].reloadTime);

}

InterfaceManager::~InterfaceManager(void)
{

    std::cout << "Removing InterfaceManager ... DONE" << std::endl;

}


void InterfaceManager::DrawIntHelp(const Tex& texture, float dx, float dy)
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(dx, dy, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2i(0, 0);
    glTexCoord2i(1, 1);
    glVertex2i(texture.w, 0);
    glTexCoord2i(1, 0);
    glVertex2i(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2i(0, texture.h);
    glEnd();

    glPopMatrix();

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

    // ----- Gun's name -----

    Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, Weapons[world.bot[world.MY_BOT_NR]->gunModel].name, Fonts.textCol[3], 0.01f*Parser.MAX_WIDTH+170, Parser.MAX_HEIGHT-34.0f);

    // ----- Ammo count -----

    Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, IntToString(world.bot[world.MY_BOT_NR]->leftAmmos),
                    Fonts.textCol[3], 0.46f*Parser.MAX_WIDTH+33, Parser.MAX_HEIGHT-32.0f);


    glEnable(GL_TEXTURE_2D);

    // ----- Health -----

    DrawIntHelp(text_health, 0.01f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-38.0f);

    text_healthbar.w = static_cast<int>(115*world.bot[world.MY_BOT_NR]->actLife / Bots.fullLife);
    DrawIntHelp(text_healthbar, 0.01f*Parser.MAX_WIDTH+40, Parser.MAX_HEIGHT-29.0f);


    // ----- Vest -----

    text_vestbar.w = static_cast<int>(115*world.bot[world.MY_BOT_NR]->procVest);
    DrawIntHelp(text_vestbar, 0.01f*Parser.MAX_WIDTH+40, Parser.MAX_HEIGHT-17.0f);


    // ----- Ammo -----

    DrawIntHelp(text_ammo, 0.46f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-38.0f);

    // grenades' count
    for (unsigned int i = 0; i < world.bot[world.MY_BOT_NR]->numGrenades; ++i)
    {
        DrawIntHelp(text_nade, 0.46f*Parser.MAX_WIDTH+i*10, Parser.MAX_HEIGHT-15.0f);
    }
    // nades' count
    for (unsigned int i = 0; i < world.bot[world.MY_BOT_NR]->numClusters; ++i)
    {
        DrawIntHelp(text_clusternade, 0.46f*Parser.MAX_WIDTH+i*10, Parser.MAX_HEIGHT-15.0f);
    }

    if (!world.bot[world.MY_BOT_NR]->isReloading)
        text_reloadbar.w = 115*world.bot[world.MY_BOT_NR]->leftAmmos / Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
    DrawIntHelp(text_reloadbar, 0.46f*Parser.MAX_WIDTH+67, Parser.MAX_HEIGHT-29.0f);


    // -----  Little gold -----

    DrawIntHelp(text_firebar_r, 0.46f*Parser.MAX_WIDTH + text_ammo.w - text_firebar_r.w, Parser.MAX_HEIGHT-14.0f);
    float time_gold = static_cast<float>(world.getCurrentTime - world.bot[world.MY_BOT_NR]->lastShotTime) /
                      static_cast<float>(Weapons[world.bot[world.MY_BOT_NR]->gunModel].fireInterval);
    text_firebar.w = time_gold > 1.0f ? 57 : static_cast<int>(57*time_gold);
    DrawIntHelp(text_firebar, 0.46f*Parser.MAX_WIDTH + text_ammo.w - text_firebar_r.w, Parser.MAX_HEIGHT-14.0f);


    // ----- Flying -----

    DrawIntHelp(text_jet, 0.75f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-38.0f);
    text_jetbar.w = static_cast<int>(115*world.bot[world.MY_BOT_NR]->procJet);
    DrawIntHelp(text_jetbar, 0.75f*Parser.MAX_WIDTH+40.0f, Parser.MAX_HEIGHT-29.0f);

    glDisable(GL_TEXTURE_2D);

    // ----- Player status -----

    // my position / players number
    str = IntToString(world.MY_CURRENT_POS);
    str.append("/");
    str.append(IntToString(world.bot.size()));
    Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, str, Fonts.textCol[4], 0.87f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-65.0f);

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
    Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, str, Fonts.textCol[1], 0.87f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-55.0f);

    // kill / point / capture limit
    Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, IntToString(Parser.FIRST_LIMIT),
                    Fonts.textCol[2], 0.87f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-45.0f);

}
