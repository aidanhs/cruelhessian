/*   InterfaceManager.cpp
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


#include <sstream>

#include "InterfaceManager.h"
#include "WorldMap.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "Game.h"
#include "TexturesLoader.h"



InterfaceManager::InterfaceManager()
{

    std::cout << "Starting InterfaceManager ..." << std::endl;

    std::cout << "   loading textures ... " << std::endl;

    text_health    = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "health");
    text_healthbar = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "health-bar");
    text_ammo      = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "ammo");
    text_nade      = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "nade");
    text_reloadbar = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "reload-bar");
    text_jet       = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "jet");
    text_jetbar    = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "jet-bar");
    text_firebar   = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "fire-bar");
    text_firebar_r = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "fire-bar-r");


}


void InterfaceManager::ReloadBar()
{

    text_reloadbar.w = 115.0f * (world.getCurrentTime - world.bot[world.MY_BOT_NR]->startReloadingTime) / Weapons[world.bot[world.MY_BOT_NR]->gunModel].reloadTime;

}

InterfaceManager::~InterfaceManager()
{

    std::cout << "Removing InterfaceManager ..." << std::endl;

}


void InterfaceManager::DrawIntHelp(const Tex& texture, float dx, float dy)
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(dx, dy, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(texture.w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, texture.h);
    glEnd();

    glPopMatrix();

}


void InterfaceManager::Draw()
{

    std::ostringstream oss;
    std::string temp;


    // ----- Gun's name -----

    Fonts.printText(Fonts.font[1][Fonts.FontConsoleSize], Weapons[world.bot[world.MY_BOT_NR]->gunModel].name, Fonts.textCol[3], 0.01f*Parser.MAX_WIDTH+170, Parser.MAX_HEIGHT-34.0f);

    // ----- Ammo count -----

    oss << world.bot[world.MY_BOT_NR]->leftAmmos;
    Fonts.printText(Fonts.font[1][Fonts.FontConsoleSize], oss.str(), Fonts.textCol[3], 0.46f*Parser.MAX_WIDTH+33, Parser.MAX_HEIGHT-32.0f);
    oss.str("");


    glEnable(GL_TEXTURE_2D);

    // ----- Health -----

    DrawIntHelp(text_health, 0.01f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-38.0f);

    text_healthbar.w = 115.0f*world.bot[world.MY_BOT_NR]->actLife / world.fullLife;
    DrawIntHelp(text_healthbar, 0.01f*Parser.MAX_WIDTH+40, Parser.MAX_HEIGHT-29.0f);


    // ----- Ammo -----

    DrawIntHelp(text_ammo, 0.46f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-38.0f);

    // grenades' count
    for (unsigned int i = 0; i < world.bot[world.MY_BOT_NR]->numGrenades; ++i)
    {
        DrawIntHelp(text_nade, 0.46f*Parser.MAX_WIDTH+i*10, Parser.MAX_HEIGHT-15.0f);
    }

    if (!world.bot[world.MY_BOT_NR]->isReloading)
        text_reloadbar.w = 115.0f*world.bot[world.MY_BOT_NR]->leftAmmos / Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
    DrawIntHelp(text_reloadbar, 0.46f*Parser.MAX_WIDTH+67, Parser.MAX_HEIGHT-29.0f);


    // -----  Little gold -----

    DrawIntHelp(text_firebar_r, 0.46f*Parser.MAX_WIDTH + text_ammo.w - text_firebar_r.w, Parser.MAX_HEIGHT-14.0f);
    float time_gold = static_cast<float>(world.getCurrentTime - world.bot[world.MY_BOT_NR]->lastShotTime) /
        static_cast<float>(Weapons[world.bot[world.MY_BOT_NR]->gunModel].fireInterval);
    text_firebar.w = 57.0f*(time_gold > 1.0f ? 1.0f : time_gold);
    DrawIntHelp(text_firebar, 0.46f*Parser.MAX_WIDTH + text_ammo.w - text_firebar_r.w, Parser.MAX_HEIGHT-14.0f);


    // ----- Flying -----

    DrawIntHelp(text_jet, 0.75f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-38.0f);
    text_jetbar.w = 115.0f*world.bot[world.MY_BOT_NR]->procJet;
    DrawIntHelp(text_jetbar, 0.75f*Parser.MAX_WIDTH+40.0f, Parser.MAX_HEIGHT-29.0f);


    // ----- Player status -----

    // my position / players number
    oss << world.MY_CURRENT_POS;
    temp = oss.str() + "/";
    oss.str("");
    oss << world.bot.size();
    temp += oss.str();
    Fonts.printText(Fonts.font[1][Fonts.FontConsoleSize], temp, Fonts.textCol[4], 0.87f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-65.0f);
    oss.str("");

    // kills / points (points to leader)
    if (game.CURRENT_GAME_MODE == DM)
        oss << world.bot[world.MY_BOT_NR]->killedNr;
    else
        oss << world.bot[world.MY_BOT_NR]->points;
    temp = oss.str() + " (";
    oss.str("");
    if (world.DISTANCE_SCORE < 0)
        temp += "-";
    else
        temp += "+";
    oss << abs(world.DISTANCE_SCORE);
    temp += oss.str() + ")";
    Fonts.printText(Fonts.font[1][Fonts.FontConsoleSize], temp, Fonts.textCol[1], 0.87f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-55.0f);
    oss.str("");

    // kill / point / capture limit
    oss << Parser.FIRST_LIMIT;
    Fonts.printText(Fonts.font[1][Fonts.FontConsoleSize], oss.str(), Fonts.textCol[2], 0.87f*Parser.MAX_WIDTH, Parser.MAX_HEIGHT-45.0f);
    oss.str("");

    glDisable(GL_TEXTURE_2D);

}
