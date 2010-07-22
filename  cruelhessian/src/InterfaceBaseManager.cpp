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


#include <sstream>

#include "InterfaceBaseManager.h"
#include "WeaponManager.h"
#include "AudioManager.h"
#include "ParserManager.h"
#include "FontManager.h"
#include "TexturesLoader.h"
#include "WorldMap.h"



InterfaceBaseManager::InterfaceBaseManager()
{

    std::cout << "Starting InterfaceBaseManager ..." << std::endl;

    std::cout << "   loading textures ... " << std::endl;

    text_arrow     = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "arrow");
    text_mouse     = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "cursor");
    text_deaddot   = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "deaddot");
    text_smalldot  = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "smalldot");

}

InterfaceBaseManager::~InterfaceBaseManager()
{

    std::cout << "Removing InterfaceBaseManager ..." << std::endl;

}


void InterfaceBaseManager::Draw()
{

    if (!world.bot[world.MY_BOT_NR]->isKilled)
        world.CHOICE_GUN = false;

    if (world.YOU_KILLED)
    {
        if (world.getCurrentTime - world.bot[world.MY_BOT_NR]->youKilledTime <= 1000)
            Fonts.printTextMiddle(Fonts.font[1][Fonts.FontMenuSize], "You killed " + world.bot[world.bot[world.MY_BOT_NR]->killed]->name, Fonts.textCol[1], 50.0f);
        else
            world.YOU_KILLED = false;
    }


    if (world.SHOW_GUN_MENU && !world.CHOICE_GUN)
    {
        int x, y;
        //SDL_ShowCursor(SDL_ENABLE);
        world.window_guns->draw(world.mouse->getLocalPosition());

        // if select gun with mouse button
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            if (Parser.SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, Audio.menu_click, 0);

            int nr = world.window_guns->select();

            if (nr != -1)
            {
                world.bot[world.MY_BOT_NR]->gunModel = nr;
                world.bot[world.MY_BOT_NR]->leftAmmos = Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
                world.CHOICE_GUN = true;
                world.SHOW_GUN_MENU = false;
            }
        }
        // if select gun with key
        else if (world.KEY_PRESSED >= SDLK_0 && world.KEY_PRESSED <= SDLK_9)
        {
            if (Parser.SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, Audio.menu_click, 0);

            world.bot[world.MY_BOT_NR]->gunModel = world.KEY_PRESSED-48;
            world.bot[world.MY_BOT_NR]->leftAmmos = Weapons[world.bot[world.MY_BOT_NR]->gunModel].ammo;
            world.CHOICE_GUN = true;
            world.SHOW_GUN_MENU = false;
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }

    world.chat->update();
    world.chat->draw();

    std::ostringstream oss;

    if (world.SHOW_STATS)
    {
        oss << "FPS: ";
        oss << world.currentFPS;
        Fonts.printText(Fonts.font[1][Fonts.FontConsoleSize], oss.str(), Fonts.textCol[3], 0.8f*Parser.MAX_WIDTH, 15.0f);
        oss.str("");
    }

    // scores
    if (world.SHOW_SCORES)
    {
        world.window_scores->draw();
    }

    if (world.SHOW_COMMAND_LINE)
    {
        world.command_line();
    }

    if (Parser.CONSOLE_SHOW && world.SHOW_MYCHAT_LINE)
    {
        world.mychat_line();
    }

    if (world.SHOW_ESC)
    {
        int x, y;
        //glutSetCursor(GLUT_CURSOR_INHERIT);
        world.window_exit->draw(world.mouse->getLocalPosition());
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            world.CHOICE_EXIT = world.window_exit->select();
            // std::cout << "C " << CHOICE_EXIT << std::endl;
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }
}
