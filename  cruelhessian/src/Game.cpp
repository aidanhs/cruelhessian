/*   Game.cpp
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


#include "boost/filesystem/operations.hpp"

#include "Game.h"
#include "GUI/GUI.h"
#include "Regex.h"
#include "WorldMap.h"
//#include "Singleton.h"
#include "WeaponManager.h"
#include "BotManager.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "ParserManager.h"
#include "BonusManager.h"

//#include "SDL.h"
/*
extern "C" {
#include <lua50/lua.h>
#include <lua50/lualib.h>
#include <lua50/lauxlib.h>
}
*/

Game::Game(int argc, char *argv[])
{

    // directories name
    std::string folders[] = {"Anims", "Bots", "Gostek-gfx", "Interface-gfx", "Maps", "Rest", "Scenery-gfx", "Sfx", "Sparks-gfx", "Textures", "Txt", "Weapons-gfx"};
    SOLDAT_FOLDER.assign(folders, folders + sizeof(folders) / sizeof(std::string) );

    // colors name
    std::string colors[] = {"clAqua", "clBlack", "clBlue", "clCream", "clFuchsia", "clGray", "clGreen", "clLime", "clMaroon", "clMedGray",
                            "clMoneyGreen", "clNavy", "clOlive", "clPurple", "clRed", "clSilver", "clSkyBlue", "clTeal", "clWhite", "clYellow"
                           };

    unsigned int size_col = sizeof(colors) / sizeof(std::string);
    CL_COLOR.resize(size_col);

    for (unsigned int i = 0; i < size_col; ++i)
    {
        CL_COLOR[i].name = colors[i];
        CL_COLOR[i].rgb.resize(3);
    }

    // aqua
    CL_COLOR[0].rgb[0] = 0;
    CL_COLOR[0].rgb[1] = 255;
    CL_COLOR[0].rgb[2] = 255;

    // black
    CL_COLOR[1].rgb[0] =  CL_COLOR[1].rgb[1] = CL_COLOR[1].rgb[2] = 0;

    // blue
    CL_COLOR[2].rgb[0] = 0;
    CL_COLOR[2].rgb[1] = 0;
    CL_COLOR[2].rgb[2] = 255;

    // cream
    CL_COLOR[3].rgb[0] = 255;
    CL_COLOR[3].rgb[1] = 251;
    CL_COLOR[3].rgb[2] = 240;

    // fuchsia
    CL_COLOR[4].rgb[0] = 255;
    CL_COLOR[4].rgb[1] = 0;
    CL_COLOR[4].rgb[2] = 255;

    // gray
    CL_COLOR[5].rgb[0] = CL_COLOR[5].rgb[1] = CL_COLOR[5].rgb[2] = 128;

    // green
    CL_COLOR[6].rgb[0] = 0;
    CL_COLOR[6].rgb[1] = 255;
    CL_COLOR[6].rgb[2] = 0;

    // lime
    CL_COLOR[7].rgb[0] = 0;
    CL_COLOR[7].rgb[1] = 255;
    CL_COLOR[7].rgb[2] = 0;

    // maroon
    CL_COLOR[8].rgb[0] = 128;
    CL_COLOR[8].rgb[1] = 0;
    CL_COLOR[8].rgb[2] = 0;

    // medgray
    CL_COLOR[9].rgb[0] = 160;
    CL_COLOR[9].rgb[1] = 160;
    CL_COLOR[9].rgb[2] = 164;

    // money green
    CL_COLOR[10].rgb[0] = 192;
    CL_COLOR[10].rgb[1] = 220;
    CL_COLOR[10].rgb[2] = 192;

    // navy
    CL_COLOR[11].rgb[0] = 0;
    CL_COLOR[11].rgb[1] = 0;
    CL_COLOR[11].rgb[2] = 128;

    // olive
    CL_COLOR[12].rgb[0] = 128;
    CL_COLOR[12].rgb[1] = 128;
    CL_COLOR[12].rgb[2] = 0;

    // purple
    CL_COLOR[13].rgb[0] = 128;
    CL_COLOR[13].rgb[1] = 0;
    CL_COLOR[13].rgb[2] = 128;

    // red
    CL_COLOR[14].rgb[0] = 255;
    CL_COLOR[14].rgb[1] = 0;
    CL_COLOR[14].rgb[2] = 0;

    // silver
    CL_COLOR[15].rgb[0] =  CL_COLOR[15].rgb[1] = CL_COLOR[15].rgb[2] = 192;

    // sky blue
    CL_COLOR[16].rgb[0] = 166;
    CL_COLOR[16].rgb[1] = 202;
    CL_COLOR[16].rgb[2] = 240;

    // teal
    CL_COLOR[17].rgb[0] = 0;
    CL_COLOR[17].rgb[1] = 128;
    CL_COLOR[17].rgb[2] = 128;

    // white
    CL_COLOR[18].rgb[0] = CL_COLOR[18].rgb[1] = CL_COLOR[18].rgb[2] = 255;

    // yellow
    CL_COLOR[19].rgb[0] = 255;
    CL_COLOR[19].rgb[1] = 255;
    CL_COLOR[19].rgb[2] = 0;


    // parser loading

    parserMgr = new ParserManager;

    //std::cout << (int)tcolor2rgb("$00DDCB21")[0] << " " << (int)tcolor2rgb("$00DDCB21")[1] << " " << (int)tcolor2rgb("$00DDCB21")[2] << std::endl;
    //std::cout << (int)tcolor2rgb("$00796938")[0] << " " << (int)tcolor2rgb("$00796938")[1] << " " << (int)tcolor2rgb("$00796938")[2] << std::endl;

    START_MODE = 0;
    CONFIG_VERSION = 5;

#ifndef _WIN32
    CH_DATA_DIRECTORY = "/usr/share/cruelhessian/data/";
    //CH_DATA_DIRECTORY = "../data/";
    CH_HOME_DIRECTORY = getenv("HOME");
    CH_HOME_DIRECTORY += "/.config/cruelhessian/";
    boost::filesystem::create_directory(CH_HOME_DIRECTORY);
#else
	CH_HOME_DIRECTORY = boost::filesystem::current_path().string() + "/";
    CH_DATA_DIRECTORY = "data/";
#endif

    CH_CONFIG_FILE = CH_HOME_DIRECTORY + "options.ini";

    int parsing = Parser.ReadConfigs();

    if (argc == 1)
    {
        START_MODE = 2;
        setSFML();
        guiMgr = new GUI;
    }
    else
    {
        if (strcmp(argv[1], "-start") == 0)
        {
            if (parsing == 1)
            {
                START_MODE = 0;
                Parser.SaveConfigs();
                std::cout << "You are running CH for the first time\n";
                std::cout << "Edit 'options.ini' file before running CH next time\n";
                return;
            }


            if (!checkSoldatStart())
            {
                std::cerr << "Cannot find Soldat" << std::endl;
                return;
            }

            CreateManagers();

            // select the first CTF map

            std::string firstMap = "";
            std::string re = "ctf_.+.pms$";
            boost::filesystem::directory_iterator end;

            for (boost::filesystem::directory_iterator iter(Parser.GAME_PATH + "Maps/"); iter != end; ++iter)
            {
                if (regex_match(iter->leaf(), re))
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

                setSFML();

                worldMgr = new WorldMap(firstMap);
                //worldMgr = new WorldMap(Parser.GAME_PATH + "Maps/ctf_Cobra.PMS");
                worldMgr->SetPlayers(4, Parser.RANDOM_BOTS_1, Parser.RANDOM_BOTS_2, 0, 0);

            }
            else
            {
                std::cout << "At least one CTF map is required !" << std::endl;
            }

        }
        else
        {
            START_MODE = 0;
            std::cerr << "Wrong arguments\n";
        }
    }
}


Game::~Game()
{

    if (START_MODE == 1)
    {
        if (WorldMap::GetSingletonPtr() != NULL)
        {
            delete worldMgr;
//            SDL_FreeSurface(screen);
//           SDL_Quit();
        }

    }
    else if (START_MODE == 2)
    {
        delete guiMgr;
        //     SDL_FreeSurface(screen);
        //   SDL_Quit();
    }


    RemoveManagers();

    if (ParserManager::GetSingletonPtr() != NULL)
        delete parserMgr;


}



bool Game::checkSoldatStart()
{
    bool exit = false;

    if (!boost::filesystem::exists(Parser.GAME_PATH))
    {
        std::cerr << "Cannot find Soldat directory : " << Parser.GAME_PATH << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Anims/"))
    {
        std::cerr << "Cannot find Anims directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Bots/"))
    {
        std::cerr << "Cannot find Bots directory (not needed)" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Gostek-gfx/"))
    {
        std::cerr << "Cannot find Gostek-gfx directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Interface-gfx/"))
    {
        std::cerr << "Cannot find Interface-gfx directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Scenery-gfx/"))
    {
        std::cerr << "Cannot find Scenery-gfx directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Sfx/"))
    {
        std::cerr << "Cannot find Sfx directory (not needed)" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Sparks-gfx/"))
    {
        std::cerr << "Cannot find Sparks-gfx directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Textures/"))
    {
        std::cerr << "Cannot find Texture directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Txt/"))
    {
        std::cerr << "Cannot find Txt directory" << std::endl;
        exit = true;
    }
    if (!boost::filesystem::exists(Parser.GAME_PATH + "Weapons-gfx/"))
    {
        std::cerr << "Cannot find Weapons-gfx directory" << std::endl;
        exit = true;
    }

    if (exit)
    {
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
    if (Parser.SOUNDS_VOL > 0 || Parser.MUSIC_VOL > 0)
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

    for (unsigned int i = 0; i < CL_COLOR.size(); ++i)
    {
        if (hex == CL_COLOR[i].name)
            return CL_COLOR[i].rgb;
    }

    std::vector<unsigned char> rgb(3);
    rgb[0] = convertFromHex(hex.substr(7, 2));
    rgb[1] = convertFromHex(hex.substr(5, 2));
    rgb[2] = convertFromHex(hex.substr(3, 2));

    return rgb;

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


int Game::setSFML()
{

    sf::VideoMode Mode(static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT), Parser.MAX_BPP);
    App.Create(Mode, "Cruel Hessian", sf::Style::Close);
    App.PreserveOpenGLStates(true);
    App.SetFramerateLimit(300);
    return 0;
}



int Game::LoadMapsList()
{

    std::cout << "   loading maps list from 'mapslist.txt' (UTF-16LE with BOM encoding) ..." << std::endl;

    std::ifstream file((Parser.GAME_PATH + "mapslist.txt").c_str());

    mapsListFromFile.clear();

    if (file.is_open())
    {
        std::string newbuffer;
        std::string buffer;
        std::string newword;
        file.seekg(2, std::ios::beg);

        while (getline(file, buffer))
        {
            mapsListFromFile.push_back(buffer);
        }
        //if (game.mapsList.back().empty())
        //std::cout << "JEA" << game.mapsList.back() << "POI"<<std::endl;

        // last line has to be empty
        mapsListFromFile.pop_back();

        for (unsigned int i = 0; i < mapsListFromFile.size(); ++i)
        {
            /*for(size_t j = 0; j < mapsListFromFile[j].length(); ++j)
            {
                std::cout << " " << static_cast<unsigned int>(static_cast<unsigned short>(mapsListFromFile[i][j]));
            }
            */
            //for(int j = 0; j < mapsListFromFile[i].length(); j++)
            //  newword[j] = mapsListFromFile[i][j];

            std::cout << "   " << mapsListFromFile[i] << std::endl;
            //std::cout << "   " << newword << std::endl;
        }

        file.close();

        return 0;
    }
    else
    {
        std::cout << "ERROR opening file 'mapslist.txt'" << std::endl;
        return -1;
    }

}
