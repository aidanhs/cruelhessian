/*   game.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84 at gmail.com>
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
#include "game.h"
#include "globals.h"



Game::Game()
{

    SOL_PATH = "";
    CH_HOME_DIRECTORY = "";
    CH_DATA_DIRECTORY = "data/";
    CH_CONFIG_FILE = "";
    INTERF_PATH = "";
    //std::string INTERFACES_PATH = "";
    //std::string INTERFACESX_PATH = "";
    CH_INTERFACE = "";

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
}

Game::~Game()
{
    if (START_MODE == 1)
        delete world;
    else if (START_MODE == 2)
  //  {
        //delete worldl
    //}
        delete ch;
}


void Game::run(int argc, char *argv[])
{

    if (argc == 1)
    {
        START_MODE = 2;
        ch = new GUI;
        ch->run();
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
			if (!checkSoldat())
			{
				std::cerr << "Cannot find Soldat" << std::endl;
                return;
			}
            START_MODE = 1;
            FIRST_LIMIT = 5;
            TIME_LIMIT = 15*60;
            CURRENT_GAME_MODE = CTF;
            setSDL();
            //world = new WorldMap(SOL_PATH + "Maps/RatCave.PMS", 6);
            world = new WorldMap(SOL_PATH + "Maps/ctf_Ash.PMS", 2, 2, 2, 2);
            world->run();
        }
        else
        {
            START_MODE = 0;
            std::cerr << "Wrong arguments\n";
        }
    }
}
