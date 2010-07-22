#ifndef GAME_H
#define GAME_H

/*   Game.h
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


#include "GUI.h"
#include "WorldMap.h"
#include "Singleton.h"
#include "WeaponManager.h"
#include "BotManager.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "ParserManager.h"
#include "BonusManager.h"






class Game : public Singleton<Game>
{
    GUI *guiMgr;
    void run(int argc, char *argv[]);

    // 0 - nothing, wrong args
    // 1 - tiny mode, worldmap
    // 2 - full mode, gui
    int START_MODE;

    unsigned char convertFromHex(const std::string& hex);

    void RemoveManagers();

public:

    std::vector<std::string> SOLDAT_FOLDER;
    WorldMap *worldMgr;
    WeaponManager *weaponMgr;
    BonusManager *bonusMgr;
    BotManager *botMgr;
    FontManager *fontMgr;
    AudioManager *audioMgr;
    ParserManager *parserMgr;

    void CreateManagers();

    Game(int argc, char *argv[]);
    virtual ~Game();

    bool checkSoldatStart();

    std::vector<unsigned char> tcolor2rgb(const std::string& hex);
    std::string rgb2tcolor(const std::vector<unsigned char>& col);
    int setSDL();

    SDL_Surface *screen;

    int CONFIG_VERSION;

    GAME_MODE CURRENT_GAME_MODE;

    std::string CH_HOME_DIRECTORY;
    std::string CH_DATA_DIRECTORY;
    std::string CH_CONFIG_FILE;

};

#define game Game::GetSingleton()

#endif
