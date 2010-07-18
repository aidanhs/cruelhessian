/*   Game.cpp
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


#include "boost/filesystem/operations.hpp"

#include "Game.h"
#include "Parser.h"
#include "regexp.h"


Game::Game(int argc, char *argv[])
{

    std::string folders[] = {"Anims", "Bots", "Gostek-gfx", "Interface-gfx", "Maps", "Rest", "Scenery-gfx", "Sfx", "Sparks-gfx", "Textures", "Txt", "Weapons-gfx"};
    SOLDAT_FOLDER.assign(folders, folders + sizeof(folders) / sizeof(std::string) );

    SOL_PATH = "";
    CH_HOME_DIRECTORY = "";
    CH_DATA_DIRECTORY = "data/";
    CH_CONFIG_FILE = "";
    INTERFACE_PATH = "";
    INTERFACE_NAME = "";
    CONFIG_VERSION = 3;

#ifndef WIN32
    CH_DATA_DIRECTORY = "/usr/share/cruelhessian/data/";
    //CH_DATA_DIRECTORY = "../data/";
    CH_HOME_DIRECTORY = getenv("HOME");
    CH_HOME_DIRECTORY += "/.config/cruelhessian/";
    boost::filesystem::create_directory(CH_HOME_DIRECTORY);
#endif

    CH_CONFIG_FILE = CH_HOME_DIRECTORY + "options.ini";

    START_MODE = 0;

    CURRENT_SONG_NUMBER = 0;

    run(argc, argv);
}

Game::~Game()
{

    if (START_MODE == 1)
    {
        if (WorldMap::GetSingletonPtr() != NULL)
            delete worldMgr;
    }
    else if (START_MODE == 2)
        //  {
        //delete worldl
        //}
        delete guiMgr;

    if (BotManager::GetSingletonPtr() != NULL)
        delete botMgr;

    if (WeaponManager::GetSingletonPtr() != NULL)
        delete weaponMgr;
}


void Game::run(int argc, char *argv[])
{

    if (argc == 1)
    {

        START_MODE = 2;
        guiMgr = new GUI;

    }
    else
    {
        if (strcmp(argv[1], "-start") == 0)
        {
            if (read_configs() == 1)
            {
                START_MODE = 0;
                save_configs();
                std::cout << "You are running CH for the first time\n";
                std::cout << "Edit 'options.ini' file before running CH next time\n";
                return;
            }

            setSDL();
            if (!checkSoldat())
            {
                std::cerr << "Cannot find Soldat" << std::endl;
                return;
            }


            // select first CTF map

            std::string firstMap = "";
            std::string re = game.SOL_PATH + "Maps/ctf_.*";
            boost::filesystem::directory_iterator end;

            for (boost::filesystem::directory_iterator iter(game.SOL_PATH + "Maps/"); iter != end; ++iter)
            {
                if (regexec(regcomp((char *)re.c_str()), (char *)iter->path().string().c_str()))
                {
                    firstMap = iter->path().string();
                    break;
                }
            }

            if (firstMap != "")
            {
                START_MODE = 1;
                FIRST_LIMIT = 5;
                CURRENT_GAME_MODE = CTF;
                worldMgr = new WorldMap(firstMap, RANDOM_BOTS_1, RANDOM_BOTS_2, 0, 0);
            }
            else
            {
                std::cout << "At least one CTF map required !" << std::endl;
            }

        }
        else
        {
            START_MODE = 0;
            std::cerr << "Wrong arguments\n";
        }
    }
}


bool Game::checkSoldat()
{

    bool state = true;

    if (!boost::filesystem::exists(game.SOL_PATH))
    {
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Anims/"))
    {
        std::cerr << "Cannot find Anims directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Bots/"))
    {
        std::cerr << "Cannot find Bots directory (not needed)" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Gostek-gfx/"))
    {
        std::cerr << "Cannot find Gostek-gfx directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Interface-gfx/"))
    {
        std::cerr << "Cannot find Interface-gfx directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Scenery-gfx/"))
    {
        std::cerr << "Cannot find Scenery-gfx directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Sfx/"))
    {
        std::cerr << "Cannot find Sfx directory (not needed)" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Sparks-gfx/"))
    {
        std::cerr << "Cannot find Sparks-gfx directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Textures/"))
    {
        std::cerr << "Cannot find Texture directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Txt/"))
    {
        std::cerr << "Cannot find Txt directory" << std::endl;
        state = false;
    }
    if (!boost::filesystem::exists(game.SOL_PATH + "Weapons-gfx/"))
    {
        std::cerr << "Cannot find Weapons-gfx directory" << std::endl;
        state = false;
    }

    if (!state)
    {
        if (BotManager::GetSingletonPtr() != NULL)
            delete botMgr;

        if (WeaponManager::GetSingletonPtr() != NULL)
            delete weaponMgr;
    }
    else
    {
        if (BotManager::GetSingletonPtr() != NULL)
            delete botMgr;

        if (WeaponManager::GetSingletonPtr() != NULL)
            delete weaponMgr;

        weaponMgr = new WeaponManager;
        botMgr = new BotManager;
    }

    return state;
}


// fix it !!!!!!!! leaks
// convert TColor to RGB
// TColor - $00bbggrr
unsigned int* Game::tcolor2rgb(const std::string& col)
{

    unsigned int *res = new unsigned int[3];
    //unsigned int res[3];

    sscanf(col.substr(3, 2).c_str(), "%x", &res[2]);
    sscanf(col.substr(5, 2).c_str(), "%x", &res[1]);
    sscanf(col.substr(7, 2).c_str(), "%x", &res[0]);

    return res;
}


// convert RGB to TColor
// TColor - $00bbggrr
std::string Game::rgb2tcolor(unsigned int* col)
{
    std::string temp("$00"), res;
    std::ostringstream oss;

    for (int i = 2; i >= 0; --i)
    {
        oss.str("");
        oss << std::hex << col[i];
        res = oss.str();
        if (res.length() == 1)
            res.insert(0, "0");
        temp += res;
    }

    return temp;
}



int Game::setSDL()
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
    {
        std::cerr << "Unable to set attributes SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (FULLSCREEN)
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);

    if (!screen)
    {
        std::cerr << "Unable to set video mode: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Cruel Hessian", "");

    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    return 0;

}

