/*   Game.h
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

#ifndef GAME_H
#define GAME_H

#include "GUI.h"
#include "WorldMap.h"
#include "Singleton.h"
#include "WeaponManager.h"
#include "BotManager.h"


class Game : public Singleton<Game>
{
    GUI *guiMgr;
    void run(int argc, char *argv[]);

    // static Game* ms_Singleton;


    // 0 - nothing, wrong args
    // 1 - tiny mode, worldmap
    // 2 - full mode, gui
    int START_MODE;

public:
    std::vector<std::string> SOLDAT_FOLDER;
    WorldMap *worldMgr;
    WeaponManager *weaponMgr;// = new WeaponManager;
    BotManager *botMgr;// = new BotManager;

    Game(int argc, char *argv[]);
    ~Game();

    bool checkSoldat();
    unsigned int* tcolor2rgb(const std::string& col);
    std::string rgb2tcolor(unsigned int* col);
    int setSDL();

    SDL_Surface *screen;

    int CONFIG_VERSION;
    int SOUNDS_VOL_INT;
    float SOUNDS_VOL, MUSIC_VOL;
    int AUDIO_QUAL;
    bool FULLSCREEN;
    int MAX_BPP;
    float MAX_WIDTH, MAX_HEIGHT;
    unsigned int *COLOR_SHIRT, *COLOR_SKIN, *COLOR_HAIR, *COLOR_PANTS, *COLOR_JET;

    int FontMenuSize;
    int FontConsoleSize;
    int FontBigSize;
    int FontWeaponMenuSize;
    int FontConsoleSmallSize;
    int FontHeightScale;
    int FontMenuBold;
    int FontConsoleBold;
    int FontBigBold;
    int FontWeaponMenuBold;
    int FontConsoleSmallBold;
    int KillConsoleNameSpace;

    unsigned int FIRST_LIMIT;
    unsigned int LIMIT_TEAMMATCH;
    unsigned int LIMIT_DEATHMATCH;
    unsigned int LIMIT_POINTMATCH;
    unsigned int LIMIT_RAMBOMATCH;
    unsigned int LIMIT_INFILTRATION;
    unsigned int LIMIT_HOLD;
    unsigned int LIMIT_CAPTURE;
    unsigned int LIMIT_TIME;
    bool CONSOLE_SHOW;
    int RANDOM_BOTS;
    int RANDOM_BOTS_1;
    int RANDOM_BOTS_2;
    int RANDOM_BOTS_3;
    int RANDOM_BOTS_4;

    bool WEAPON[14];
    bool BONUSES[7];

    std::vector<std::string> gMusicList;
    GAME_MODE CURRENT_GAME_MODE;

    int CURRENT_SONG_NUMBER;

    int KEY_LEFT;
    int KEY_RIGHT;
    int KEY_UP;
    int KEY_DOWN;
    int KEY_RELOAD;
    int KEY_GRENADE;
    int KEY_CHAT;
    int KEY_TEAMCHAT;

    int MODE;

    std::string PLAYER_NAME;
    std::string SOL_PATH;
    std::string SOL_PATH_0;
    std::string SOL_PATH_1;
    std::string CH_HOME_DIRECTORY;
    std::string CH_DATA_DIRECTORY;
    std::string CH_CONFIG_FILE;
    std::string INTERFACE_PATH;
    std::string INTERFACE_NAME;
};

#define game Game::GetSingleton()



#endif
