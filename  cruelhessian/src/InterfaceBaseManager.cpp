/*   InterfaceBaseManager.cpp
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


#include "InterfaceBaseManager.h"
#include <cstdio>
#include "Bot.h"
#include "textinout/ChatLine.h"
#include "textinout/CommandLine.h"
#include "textinout/TextOutput.h"
#include "WindowScores.h"
#include "WindowExit.h"
#include "WindowGuns.h"
#include "Mouse.h"
#include "WeaponManager.h"
#include "AudioManager.h"
#include "ParserManager.h"
#include "FontManager.h"
#include "TexturesLoader.h"
#include "WorldMap.h"
#include "Game.h"



InterfaceBaseManager::InterfaceBaseManager(void) :
    text_arrow   (Texture::LoadExt(Parser.INTERFACE_PATH, "arrow")),
    text_mouse   (Texture::LoadExt(Parser.INTERFACE_PATH, "cursor")),
    text_deaddot (Texture::LoadExt(Parser.INTERFACE_PATH, "deaddot")),
    text_smalldot(Texture::LoadExt(Parser.INTERFACE_PATH, "smalldot"))
{

    std::cout << "Starting InterfaceBaseManager ..." << std::endl;

    std::cout << "   loading textures ... " << std::endl;

}

InterfaceBaseManager::~InterfaceBaseManager(void)
{

    std::cout << "Removing InterfaceBaseManager ... DONE" << std::endl;

}


void InterfaceBaseManager::Draw()
{

    if (!world.bot[world.MY_BOT_NR]->isKilled)
    {
        world.CHOICE_GUN = false;
    }
    else
    {
        if (world.getCurrentTime - world.bot[world.MY_BOT_NR]->timerRespawnTime <= world.bot[world.MY_BOT_NR]->respawnPeriod)
        {

            // wait half a second after death and then show gun menu
            if (world.getCurrentTime - world.bot[world.MY_BOT_NR]->timerRespawnTime >= 500 && !world.CHOICE_GUN && world.window_guns->GetSingletonPtr() == NULL)
            {
                world.window_guns = new WindowGuns();
//                world.SHOW_GUN_MENU = true;
            }

            char chars[512];

            sprintf(chars, "%.2f", static_cast<float>(world.bot[world.MY_BOT_NR]->respawnPeriod - world.getCurrentTime + world.bot[world.MY_BOT_NR]->timerRespawnTime)/1000);
            std::string str = chars;

            Fonts.printTextMiddle(Fonts.font[1], Fonts.FontMenuSize, "Respawn in " + str, Fonts.textCol[1], 50.0f);
            Fonts.printTextMiddle(Fonts.font[1], Fonts.FontMenuSize, "Killed by " + world.bot[world.bot[world.MY_BOT_NR]->killer]->name, Fonts.textCol[1], Parser.MAX_HEIGHT-94.0f);
        }

    }


    if (world.YOU_KILLED)
    {
        if (world.getCurrentTime - world.bot[world.MY_BOT_NR]->youKilledTime <= 1000)
            Fonts.printTextMiddle(Fonts.font[1], Fonts.FontMenuSize, "You killed " + world.bot[world.bot[world.MY_BOT_NR]->killed]->name, Fonts.textCol[1], 50.0f);
        else
            world.YOU_KILLED = false;
    }

    char chars[512];
    std::string str;

    if (world.bot[world.MY_BOT_NR]->MODE_FLAMEGOD)
    {
        sprintf(chars, "%.2f", static_cast<float>(10000 - world.getCurrentTime + (world.bot[world.MY_BOT_NR]->timeGetSuperbonus))/1000);
        str = "Flamegod mode : ";// + chars;
        str.append(chars);
        Fonts.printTextMiddle(Fonts.font[1], Fonts.FontMenuSize, str, Fonts.textCol[1], Parser.MAX_HEIGHT-150.0f);
    }
    else if (world.bot[world.MY_BOT_NR]->MODE_BERSERKER)
    {
        sprintf(chars, "%.2f", static_cast<float>(15000 - world.getCurrentTime + (world.bot[world.MY_BOT_NR]->timeGetSuperbonus))/1000);
        str = "Berserker mode : ";// + chars;
        str.append(chars);
        Fonts.printTextMiddle(Fonts.font[1], Fonts.FontMenuSize, str, Fonts.textCol[1], Parser.MAX_HEIGHT-150.0f);
    }
    else if (world.bot[world.MY_BOT_NR]->MODE_PREDATOR)
    {
        sprintf(chars, "%.2f", static_cast<float>(25000 - world.getCurrentTime + (world.bot[world.MY_BOT_NR]->timeGetSuperbonus))/1000);
        str = "Predator mode : ";// + chars;
        str.append(chars);
        Fonts.printTextMiddle(Fonts.font[1], Fonts.FontMenuSize, str, Fonts.textCol[1], Parser.MAX_HEIGHT-150.0f);
    }


    if (world.window_guns->GetSingletonPtr() != NULL && !world.CHOICE_GUN)
    {
        world.window_guns->Update(world.mouse->getLocalPosition());
        world.window_guns->Draw();

        // if select gun with mouse button
        if (game.App.GetInput().IsMouseButtonDown(sf::Mouse::Left))
        {
            if (Parser.SOUNDS_VOL > 0)
                Audio.Play(Audio.menu_click);

            int nr = world.window_guns->select();

            if (nr != -1)
            {
                world.bot[world.MY_BOT_NR]->gunModel = nr;
                world.bot[world.MY_BOT_NR]->leftAmmos = Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
                world.CHOICE_GUN = true;
                delete world.window_guns;
            }
        }
        // if select gun with key
        else if (world.KEY_PRESSED >= sf::Key::Num0 && world.KEY_PRESSED <= sf::Key::Num9)
        {
            if (Parser.SOUNDS_VOL > 0)
                Audio.Play(Audio.menu_click);

            if (Parser.WEAPON[world.KEY_PRESSED-48])
            {
                world.bot[world.MY_BOT_NR]->gunModel = world.KEY_PRESSED-48;
                world.bot[world.MY_BOT_NR]->leftAmmos = Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
                world.CHOICE_GUN = true;
                delete world.window_guns;
            }

        }

    }

    world.text_output->Update();
    world.text_output->Draw();

    if (world.SHOW_STATS)
    {
        sprintf(chars, "%i", world.currentFPS);
        str = "FPS: ";
        str.append(chars);
        Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, str, Fonts.textCol[3], 0.8f*Parser.MAX_WIDTH, 15.0f);
    }

    // scores
    if (world.SHOW_SCORES)
    {
        world.window_scores->Draw();
    }

    if (world.command_line->GetSingletonPtr() != NULL)
    {
        world.command_line->Draw();
    }

    if (Parser.CONSOLE_SHOW && world.chat_line->GetSingletonPtr() != NULL)
    {
        world.chat_line->Draw();
    }

    if (world.window_exit->GetSingletonPtr() != NULL)
    {
        world.window_exit->Draw(world.mouse->getLocalPosition());

        if (game.App.GetInput().IsMouseButtonDown(sf::Mouse::Left))
        {
            world.CHOICE_EXIT = world.window_exit->Select();
        }

    }
}
