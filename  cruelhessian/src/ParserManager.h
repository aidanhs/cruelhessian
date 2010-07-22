#ifndef PARSER_MANAGER_H
#define PARSER_MANAGER_H


/*   ParserManager.h
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


#include <iostream>
#include <vector>

#include "Singleton.h"



class ParserManager : public Singleton<ParserManager>
{
public:

    ParserManager();
    virtual ~ParserManager();

    int ReadConfigs();
    int SaveConfigs();

    float SOUNDS_VOL;//   = 20.0f;
    float MUSIC_VOL;//    = 0.0f;
    int AUDIO_QUAL;//   = 44100;

    bool FULLSCREEN;
    int MAX_BPP;
    float MAX_WIDTH, MAX_HEIGHT;

    std::vector<unsigned char> COLOR_SHIRT, COLOR_SKIN, COLOR_HAIR, COLOR_PANTS, COLOR_JET;

    unsigned int FIRST_LIMIT;
    unsigned int LIMIT_TEAMMATCH;
    unsigned int LIMIT_DEATHMATCH;
    unsigned int LIMIT_POINTMATCH;
    unsigned int LIMIT_RAMBOMATCH;
    unsigned int LIMIT_INFILTRATION;
    unsigned int LIMIT_HOLD;
    unsigned int LIMIT_CAPTURE;
    unsigned int LIMIT_TIME;
    int RANDOM_BOTS;
    int RANDOM_BOTS_1;
    int RANDOM_BOTS_2;
    int RANDOM_BOTS_3;
    int RANDOM_BOTS_4;

    bool WEAPON[14];
    bool BONUSES[7];
    bool CONSOLE_SHOW;

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
    std::string SOL_PATH_[2];
    std::string INTERFACE_PATH;
    std::string INTERFACE_PATH_[2];
    std::string INTERFACE_NAME;
    std::string INTERFACE_NAME_[2];

};

#define Parser ParserManager::GetSingleton()

#endif
