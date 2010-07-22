/*   Game.cpp
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


#include "boost/filesystem/operations.hpp"

#include "Game.h"
#include "regexp.h"


Game::Game(int argc, char *argv[])
{

    std::string folders[] = {"Anims", "Bots", "Gostek-gfx", "Interface-gfx", "Maps", "Rest", "Scenery-gfx", "Sfx", "Sparks-gfx", "Textures", "Txt", "Weapons-gfx"};
    SOLDAT_FOLDER.assign(folders, folders + sizeof(folders) / sizeof(std::string) );

    parserMgr = new ParserManager;

    CH_HOME_DIRECTORY = "";
    CH_DATA_DIRECTORY = "data/";
    CH_CONFIG_FILE = "";
    START_MODE = 0;
    CONFIG_VERSION = 4;

#ifndef WIN32
    CH_DATA_DIRECTORY = "/usr/share/cruelhessian/data/";
    //CH_DATA_DIRECTORY = "../data/";
    CH_HOME_DIRECTORY = getenv("HOME");
    CH_HOME_DIRECTORY += "/.config/cruelhessian/";
    boost::filesystem::create_directory(CH_HOME_DIRECTORY);
#endif

    CH_CONFIG_FILE = CH_HOME_DIRECTORY + "options.ini";

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
        delete guiMgr;

    RemoveManagers();

    if (ParserManager::GetSingletonPtr() != NULL)
        delete parserMgr;
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
            if (Parser.ReadConfigs() == 1)
            {
                START_MODE = 0;
                Parser.SaveConfigs();
                std::cout << "You are running CH for the first time\n";
                std::cout << "Edit 'options.ini' file before running CH next time\n";
                return;
            }

            setSDL();
            if (!checkSoldatStart())
            {
                std::cerr << "Cannot find Soldat" << std::endl;
                return;
            }

            CreateManagers();

            // select the first CTF map

            std::string firstMap = "";
            std::string re = Parser.SOL_PATH + "Maps/ctf_.*";
            boost::filesystem::directory_iterator end;

            for (boost::filesystem::directory_iterator iter(Parser.SOL_PATH + "Maps/"); iter != end; ++iter)
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
                Parser.FIRST_LIMIT = 5;
                CURRENT_GAME_MODE = CTF;
                worldMgr = new WorldMap(firstMap, Parser.RANDOM_BOTS_1, Parser.RANDOM_BOTS_2, 0, 0);
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


bool Game::checkSoldatStart()
{

    if (!boost::filesystem::exists(Parser.SOL_PATH))
    {
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Anims/"))
    {
        std::cerr << "Cannot find Anims directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Bots/"))
    {
        std::cerr << "Cannot find Bots directory (not needed)" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Gostek-gfx/"))
    {
        std::cerr << "Cannot find Gostek-gfx directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Interface-gfx/"))
    {
        std::cerr << "Cannot find Interface-gfx directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Scenery-gfx/"))
    {
        std::cerr << "Cannot find Scenery-gfx directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Sfx/"))
    {
        std::cerr << "Cannot find Sfx directory (not needed)" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Sparks-gfx/"))
    {
        std::cerr << "Cannot find Sparks-gfx directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Textures/"))
    {
        std::cerr << "Cannot find Texture directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Txt/"))
    {
        std::cerr << "Cannot find Txt directory" << std::endl;
        RemoveManagers();
        return false;
    }
    if (!boost::filesystem::exists(Parser.SOL_PATH + "Weapons-gfx/"))
    {
        std::cerr << "Cannot find Weapons-gfx directory" << std::endl;
        RemoveManagers();
        return false;
    }

    return true;

}


void Game::RemoveManagers()
{

    if (BotManager::GetSingletonPtr() != NULL)
        delete botMgr;

    if (WeaponManager::GetSingletonPtr() != NULL)
        delete weaponMgr;

    if (BonusManager::GetSingletonPtr() != NULL)
        delete bonusMgr;

    if (AudioManager::GetSingletonPtr() != NULL)
        delete audioMgr;

    if (FontManager::GetSingletonPtr() != NULL)
        delete fontMgr;
}


void Game::CreateManagers()
{

    RemoveManagers();

    fontMgr = new FontManager;
    audioMgr = new AudioManager;
    bonusMgr = new BonusManager;
    weaponMgr = new WeaponManager;
    botMgr = new BotManager;

}



// based on MikeTacular's code
unsigned char Game::convertFromHex(const std::string& hex)
{

    int value = 0;

    int a = 0;
    int b = hex.length() - 1;
    for (; b >= 0; a++, b--)
    {
        if (hex[b] >= '0' && hex[b] <= '9')
        {
            value += (hex[b] - '0') * (1 << (a * 4));
        }
        else
        {
            switch (hex[b])
            {
            case 'A':
            case 'a':
                value += 10 * (1 << (a * 4));
                break;

            case 'B':
            case 'b':
                value += 11 * (1 << (a * 4));
                break;

            case 'C':
            case 'c':
                value += 12 * (1 << (a * 4));
                break;

            case 'D':
            case 'd':
                value += 13 * (1 << (a * 4));
                break;

            case 'E':
            case 'e':
                value += 14 * (1 << (a * 4));
                break;

            case 'F':
            case 'f':
                value += 15 * (1 << (a * 4));
                break;

            default:
                std::cout << "Error, invalid character '" << hex[a] << "' in hex number" << std::endl;
                break;
            }
        }
    }

    return (unsigned char)value;
}


// convert TColor to RGB
// TColor - $00bbggrr
std::vector<unsigned char> Game::tcolor2rgb(const std::string& hex)
{

    std::vector<unsigned char> rgb(3);

    if (hex == "clAqua")
    {
        rgb[0] = 0;
        rgb[1] = 255;
        rgb[2] = 255;
        return rgb;
    }
    else if (hex == "clBlack")
    {
        rgb[0] = rgb[1] = rgb[2] = 0;
        return rgb;
    }
    else if (hex == "clBlue")
    {
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 255;
        return rgb;
    }
    else if (hex == "clCream")
    {
        rgb[0] = 255;
        rgb[1] = 251;
        rgb[2] = 240;
        return rgb;
    }
    else if (hex == "clFuchsia")
    {
        rgb[0] = 255;
        rgb[1] = 0;
        rgb[2] = 255;
        return rgb;
    }
    else if (hex == "clGray")
    {
        rgb[0] = 128;
        rgb[1] = 128;
        rgb[2] = 128;
        return rgb;
    }
    else if (hex == "clGreen")
    {
        rgb[0] = 0;
        rgb[1] = 255;
        rgb[2] = 0;
        return rgb;
    }
    else if (hex == "clLime")
    {
        rgb[0] = 0;
        rgb[1] = 255;
        rgb[2] = 0;
        return rgb;
    }
    else if (hex == "clMaroon")
    {
        rgb[0] = 128;
        rgb[1] = 0;
        rgb[2] = 0;
        return rgb;
    }
    else if (hex == "clMedGray" || hex == "clMediumGray")
    {
        rgb[0] = 160;
        rgb[1] = 160;
        rgb[2] = 164;
        return rgb;
    }
    else if (hex == "clMoneyGreen")
    {
        rgb[0] = 192;
        rgb[1] = 220;
        rgb[2] = 192;
        return rgb;
    }
    else if (hex == "clNavy")
    {
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 128;
        return rgb;
    }
    else if (hex == "clOlive")
    {
        rgb[0] = 128;
        rgb[1] = 128;
        rgb[2] = 0;
        return rgb;
    }
    else if (hex == "clPurple")
    {
        rgb[0] = 128;
        rgb[1] = 0;
        rgb[2] = 128;
        return rgb;
    }
    else if (hex == "clRed")
    {
        rgb[0] = 255;
        rgb[1] = 0;
        rgb[2] = 0;
        return rgb;
    }
    else if (hex == "clSilver")
    {
        rgb[0] = 192;
        rgb[1] = 192;
        rgb[2] = 192;
        return rgb;
    }
    else if (hex == "clSkyBlue")
    {
        rgb[0] = 166;
        rgb[1] = 202;
        rgb[2] = 240;
        return rgb;
    }
    else if (hex == "clTeal")
    {
        rgb[0] = 0;
        rgb[1] = 128;
        rgb[2] = 128;
        return rgb;
    }
    else if (hex == "clWhite")
    {
        rgb[0] = rgb[1] = rgb[2] = 255;
        return rgb;
    }
    else if (hex == "clYellow")
    {
        rgb[0] = 255;
        rgb[1] = 255;
        rgb[2] = 0;
        return rgb;
    }
    else
    {
        rgb[0] = convertFromHex(hex.substr(7, 2));
        rgb[1] = convertFromHex(hex.substr(5, 2));
        rgb[2] = convertFromHex(hex.substr(3, 2));
        return rgb;
    }

}


// convert RGB to TColor
// TColor - $00bbggrr
std::string Game::rgb2tcolor(const std::vector<unsigned char>& col)
{
    std::string temp("$00"), res;
    std::ostringstream oss;

    for (int i = 2; i >= 0; --i)
    {
        oss.str("");
        oss << std::hex << (int)col[i];
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

    if (Parser.FULLSCREEN)
        screen = SDL_SetVideoMode(static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT), Parser.MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT), Parser.MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);

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

