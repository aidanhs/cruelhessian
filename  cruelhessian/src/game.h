/*   game.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawe³ Konieczny <konp84@gmail.com>
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

#include "gui.h"
#include "worldmap.h"


class Game
{
    GUI *ch;
    WorldMap *world;


    // 0 - nothing, wrong args
    // 1 - tiny mode, worldmap
    // 2 - full mode, gui
    int START_MODE;
public:

    Game();
    ~Game();
    void run(int argc, char *argv[]);

/*
    GAME_MODE CURRENT_GAME_MODE;

    float SOUNDS_VOL, MUSIC_VOL;
//std::string SONG_NAME;
    int AUDIO_QUAL;

    float MAX_WIDTH, MAX_HEIGHT;

    unsigned int TIME_LIMIT;                                   // in seconds !!!!!!!!!!!!!!!
    unsigned int FIRST_LIMIT;

    std::vector<std::string> gMusicList;
    std::vector<std::string> mapsList;
    int CURRENT_SONG_NUMBER;

    int KEY_LEFT;
    int KEY_RIGHT;
    int KEY_UP;
    int KEY_DOWN;
    int KEY_RELOAD;
    int KEY_GRENADE;
    int KEY_CHAT;
    int KEY_TEAMCHAT;

*/
//std::vector<std::string> mapsList;
//std::vector<std::string> gMusicList;
};


#endif
