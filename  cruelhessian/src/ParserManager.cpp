/*   ParserManager.cpp
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


#include "ParserManager.h"
#include "Game.h"
#include "parser/SimpleIni.h"


ParserManager::ParserManager()
{

    std::cout << "Starting ParserManager ..." << std::endl;

    MODE               = 1;
    SOL_PATH_[0]       = "";
    INTERFACE_NAME_[0] = "Default";
    INTERFACE_PATH_[0] = "";
    SOL_PATH_[1]       = "";
    INTERFACE_NAME_[1] = "Default";
    INTERFACE_PATH_[1] = "";
    FULLSCREEN         = 0;
    MAX_WIDTH          = 640.0f;
    MAX_HEIGHT         = 480.0f;
    MAX_BPP            = 16;
    SOUNDS_VOL         = 20.0f;
    MUSIC_VOL          = 0.0f;
    AUDIO_QUAL         = 44100;

    PLAYER_NAME = "Player";
    COLOR_SHIRT = game.tcolor2rgb("$00EBEBFF");
    COLOR_SKIN  = game.tcolor2rgb("$00EBEBFF");
    COLOR_HAIR  = game.tcolor2rgb("$00EBEBFF");
    COLOR_PANTS = game.tcolor2rgb("$00EBEBFF");
    COLOR_JET   = game.tcolor2rgb("$00EBEBFF");

    KEY_LEFT     = 97;
    KEY_RIGHT    = 100;
    KEY_DOWN     = 115;
    KEY_UP       = 119;
    KEY_RELOAD   = 114;
    KEY_GRENADE  = 32;
    KEY_CHAT     = 116;
    KEY_TEAMCHAT = 121;

    LIMIT_TEAMMATCH    = 60;
    LIMIT_DEATHMATCH   = 30;
    LIMIT_POINTMATCH   = 30;
    LIMIT_RAMBOMATCH   = 30;
    LIMIT_INFILTRATION = 90;
    LIMIT_HOLD         = 80;
    LIMIT_CAPTURE      = 5;
    LIMIT_TIME         = 15*60;

    CONSOLE_SHOW = true;

    RANDOM_BOTS   = 5;
    RANDOM_BOTS_1 = 1;
    RANDOM_BOTS_2 = 1;
    RANDOM_BOTS_3 = 1;
    RANDOM_BOTS_4 = 1;

    for (int i = 0; i < 14; ++i)
        WEAPON[i]  = true;

    for (int i = 0; i < 7; ++i)
        BONUSES[i]  = true;

}

ParserManager::~ParserManager()
{

    std::cout << "Removing ParserManager ..." << std::endl;

}


int ParserManager::ReadConfigs()
{

    CSimpleIni ini(false, false, false);

    if (ini.LoadFile(game.CH_CONFIG_FILE.c_str()) < 0)
    {
        std::cout << "   cannot read config file" << std::endl;
        return 1;
    }

    std::cout << "   loading configs from 'options.ini' ..." << std::endl;

    if (ini.GetLongValue("info", "Version") != game.CONFIG_VERSION)
    {
        std::cout << "   new config file version" << std::endl;
        return 1;
    }

    MODE             = static_cast<int>(ini.GetLongValue("global", "Mode"));
    SOL_PATH_[0]       = ini.GetValue("global", "SoldatPath");
    SOL_PATH_[1]       = ini.GetValue("global", "CruelHessianPath");
    INTERFACE_NAME_[0] = ini.GetValue("global", "SoldatInterface");
    INTERFACE_NAME_[1] = ini.GetValue("global", "CruelHessianInterface");
    FULLSCREEN       = ini.GetBoolValue("global", "Fullscreen");
    MAX_WIDTH        = static_cast<float>(ini.GetLongValue("global", "MaxWidth"));
    MAX_HEIGHT       = static_cast<float>(ini.GetLongValue("global", "MaxHeight"));
    MAX_BPP          = ini.GetLongValue("global", "MaxDeep");
    SOUNDS_VOL            = static_cast<float>(ini.GetLongValue("global", "SoundVolume"));
    MUSIC_VOL             = static_cast<float>(ini.GetLongValue("global", "MusicVolume"));
    AUDIO_QUAL            = static_cast<int>(ini.GetLongValue("global", "AudioQuality"));

    LIMIT_TEAMMATCH    = ini.GetLongValue("game", "TeammatchLimit");
    LIMIT_DEATHMATCH   = ini.GetLongValue("game", "DeathmatchLimit");
    LIMIT_POINTMATCH   = ini.GetLongValue("game", "PointmatchLimit");
    LIMIT_RAMBOMATCH   = ini.GetLongValue("game", "RambomatchLimit");
    LIMIT_INFILTRATION = ini.GetLongValue("game", "InfiltrationLimit");
    LIMIT_HOLD         = ini.GetLongValue("game", "HoldLimit");
    LIMIT_CAPTURE      = ini.GetLongValue("game", "CaptureLimit");
    LIMIT_TIME         = 60*ini.GetLongValue("game", "TimeLimit");

    CONSOLE_SHOW = ini.GetBoolValue("game", "ConsoleShow");

    RANDOM_BOTS   = ini.GetLongValue("game", "RandomBots");
    RANDOM_BOTS_1 = ini.GetLongValue("game", "RandomBots1");
    RANDOM_BOTS_2 = ini.GetLongValue("game", "RandomBots2");
    RANDOM_BOTS_3 = ini.GetLongValue("game", "RandomBots3");
    RANDOM_BOTS_4 = ini.GetLongValue("game", "RandomBots4");

    PLAYER_NAME = ini.GetValue("player", "Name");
    COLOR_SHIRT = game.tcolor2rgb(ini.GetValue("player", "ShirtColor"));
    COLOR_SKIN  = game.tcolor2rgb(ini.GetValue("player", "SkinColor"));
    COLOR_HAIR  = game.tcolor2rgb(ini.GetValue("player", "HairColor"));
    COLOR_PANTS = game.tcolor2rgb(ini.GetValue("player", "PantsColor"));
    COLOR_JET   = game.tcolor2rgb(ini.GetValue("player", "JetColor"));

    KEY_LEFT     = ini.GetLongValue("controls", "Left");
    KEY_RIGHT    = ini.GetLongValue("controls", "Right");
    KEY_DOWN     = ini.GetLongValue("controls", "Crouch");
    KEY_UP       = ini.GetLongValue("controls", "Jump");
    KEY_RELOAD   = ini.GetLongValue("controls", "Reload");
    KEY_GRENADE  = ini.GetLongValue("controls", "Grenade");
    KEY_CHAT     = ini.GetLongValue("controls", "Chat");
    KEY_TEAMCHAT = ini.GetLongValue("controls", "TeamChat");

    WEAPON[0]  = ini.GetBoolValue("game", "Weapon1");
    WEAPON[1]  = ini.GetBoolValue("game", "Weapon2");
    WEAPON[2]  = ini.GetBoolValue("game", "Weapon3");
    WEAPON[3]  = ini.GetBoolValue("game", "Weapon4");
    WEAPON[4]  = ini.GetBoolValue("game", "Weapon5");
    WEAPON[5]  = ini.GetBoolValue("game", "Weapon6");
    WEAPON[6]  = ini.GetBoolValue("game", "Weapon7");
    WEAPON[7]  = ini.GetBoolValue("game", "Weapon8");
    WEAPON[8]  = ini.GetBoolValue("game", "Weapon9");
    WEAPON[9]  = ini.GetBoolValue("game", "Weapon10");
    WEAPON[10] = ini.GetBoolValue("game", "Weapon11");
    WEAPON[11] = ini.GetBoolValue("game", "Weapon12");
    WEAPON[12] = ini.GetBoolValue("game", "Weapon13");
    WEAPON[13] = ini.GetBoolValue("game", "Weapon14");

    BONUSES[0] = ini.GetBoolValue("game", "BonusBerserker");
    BONUSES[1] = ini.GetBoolValue("game", "BonusCluster");
    BONUSES[2] = false;
    BONUSES[3] = ini.GetBoolValue("game", "BonusFlameGod");
    BONUSES[4] = false;
    BONUSES[5] = ini.GetBoolValue("game", "BonusPredator");
    BONUSES[6] = ini.GetBoolValue("game", "BonusVest");


    for (unsigned int i = 0; i < 2; ++i)
    {
        if (INTERFACE_NAME_[i] == "Default" || INTERFACE_NAME_[i] == "--None--")
        {
            INTERFACE_PATH_[i] = SOL_PATH_[i] + "Interface-gfx/";
        }
        else
        {
            INTERFACE_PATH_[i] = SOL_PATH_[i] + "Custom-Interfaces/";
        }

    }

    SOL_PATH = SOL_PATH_[MODE];
    INTERFACE_NAME = INTERFACE_NAME_[MODE];
    INTERFACE_PATH = INTERFACE_PATH_[MODE];


    if (!SOL_PATH.empty())
    {
        if (SOL_PATH[SOL_PATH.length()-1] == '\\')
        {
            //std::cout << "JEA" << std::endl;
            //SOL_PATH[SOL_PATH.length()-1] = '/';
        }
        else if (SOL_PATH[SOL_PATH.length()-1] != '/')
        {
            SOL_PATH += '/';
        }
        //game.INTERFACE_PATH = SOL_PATH + "Interface-gfx/";
    }
//	std::cout << "JEA" << INTERFACE_PATH << std::endl;
    /*
    //std::locale loc("");
        std::cout << "Loading maps list from 'mapslist.txt' ..." << std::endl;
        //std::wifstream file((SOL_PATH + "mapslist.txt").c_str(), std::ios_base::binary);
        std::ifstream file((SOL_PATH + "mapslist.txt").c_str());
        //std::locale loc(file.getloc(), new boost::archive::codecvt_null<wchar_t>);

    //    file.imbue(loc);

        if (file.is_open())
        {
            std::string newbuffer;
            std::string buffer;
    int k = 0;
            file.seekg(2, std::ios::beg);
            while (getline(file, buffer))
            //while (file >> buffer)
            {
                //std::cout << buffer[0]<<std::endl;
                //if (buffer[0] != '\n' && buffer[0] != '\r' && buffer != "")
                //if (buffer[0] >= 'a' && buffer[0] <= 'Z')
                {
                              //  newbuffer = buffer;
                //std::cout << "ONE" << std::endl;
                //newbuffer.assign(buffer.begin(), buffer.end());
              //  newbuffer += ".PMS";
                //if (boost::filesystem::exists(newbuffer))
                    mapsList.push_back(buffer);
                //std::cout << buffer << std::endl;
                }

            }
            for (unsigned int i = 0; i < mapsList.size(); ++i)
            {
                std::cout << mapsList[i] << std::endl;
                //if (mapsList[i][0] == ' ') std::cout << "KURE\n";
            }

            file.close();
        }
        else
        {
            std::cout << "ERROR opening file 'mapslist.txt'" << std::endl;
            return -1;
        }
    */
    return 0;
}




int ParserManager::SaveConfigs()
{
    std::cout << "Saving configs to 'options.ini' ..." << std::endl;

    CSimpleIni ini(false, false, false);

    if (!std::ifstream(game.CH_CONFIG_FILE.c_str()))
    {
        std::ifstream o_file(game.CH_CONFIG_FILE.c_str());
        o_file.close();
    }

    ini.LoadFile(game.CH_CONFIG_FILE.c_str());

    ini.SetLongValue("info", "Version", game.CONFIG_VERSION, NULL);

    ini.SetLongValue("global", "Mode",                  static_cast<int>(MODE),   NULL);
    ini.SetValue(    "global", "SoldatPath",            SOL_PATH_[0].c_str(),       NULL);
    ini.SetValue(    "global", "SoldatInterface",       INTERFACE_NAME_[0].c_str(), NULL);
    ini.SetValue(    "global", "CruelHessianPath",      SOL_PATH_[1].c_str(),       NULL);
    ini.SetValue(    "global", "CruelHessianInterface", INTERFACE_NAME_[1].c_str(), NULL);
    ini.SetBoolValue("global", "Fullscreen",       FULLSCREEN,                   NULL);
    ini.SetLongValue("global", "MaxWidth",         static_cast<int>(MAX_WIDTH),  NULL);
    ini.SetLongValue("global", "MaxHeight",        static_cast<int>(MAX_HEIGHT), NULL);
    ini.SetLongValue("global", "MaxDeep",          MAX_BPP,                      NULL);
    ini.SetLongValue("global", "SoundVolume",      static_cast<int>(SOUNDS_VOL), NULL);
    ini.SetLongValue("global", "MusicVolume",      static_cast<int>(MUSIC_VOL),  NULL);
    ini.SetLongValue("global", "AudioQuality",     AUDIO_QUAL,                   NULL);

    ini.SetValue("player", "Name",       PLAYER_NAME.c_str(),                  NULL);
    ini.SetValue("player", "ShirtColor", game.rgb2tcolor(COLOR_SHIRT).c_str(), NULL);
    ini.SetValue("player", "SkinColor",  game.rgb2tcolor(COLOR_SKIN).c_str(),  NULL);
    ini.SetValue("player", "HairColor",  game.rgb2tcolor(COLOR_HAIR).c_str(),  NULL);
    ini.SetValue("player", "PantsColor", game.rgb2tcolor(COLOR_PANTS).c_str(), NULL);
    ini.SetValue("player", "JetColor",   game.rgb2tcolor(COLOR_JET).c_str(),   NULL);

    ini.SetLongValue("game", "TeammatchLimit",    LIMIT_TEAMMATCH,                 NULL);
    ini.SetLongValue("game", "DeathmatchLimit",   LIMIT_DEATHMATCH,                NULL);
    ini.SetLongValue("game", "PointmatchLimit",   LIMIT_POINTMATCH,                NULL);
    ini.SetLongValue("game", "RambomatchLimit",   LIMIT_RAMBOMATCH,                NULL);
    ini.SetLongValue("game", "InfiltrationLimit", LIMIT_INFILTRATION,              NULL);
    ini.SetLongValue("game", "HoldLimit",         LIMIT_HOLD,                      NULL);
    ini.SetLongValue("game", "CaptureLimit",      LIMIT_CAPTURE,                   NULL);
    ini.SetLongValue("game", "TimeLimit",         static_cast<int>(LIMIT_TIME/60), NULL);

    ini.SetLongValue("game", "RandomBots",  RANDOM_BOTS,   NULL);
    ini.SetLongValue("game", "RandomBots1", RANDOM_BOTS_1, NULL);
    ini.SetLongValue("game", "RandomBots2", RANDOM_BOTS_2, NULL);
    ini.SetLongValue("game", "RandomBots3", RANDOM_BOTS_3, NULL);
    ini.SetLongValue("game", "RandomBots4", RANDOM_BOTS_4, NULL);

    ini.SetBoolValue("game", "ConsoleShow", CONSOLE_SHOW, NULL);
    ini.SetBoolValue("game", "Weapon1",  WEAPON[0], NULL);
    ini.SetBoolValue("game", "Weapon2",  WEAPON[1], NULL);
    ini.SetBoolValue("game", "Weapon3",  WEAPON[2], NULL);
    ini.SetBoolValue("game", "Weapon4",  WEAPON[3], NULL);
    ini.SetBoolValue("game", "Weapon5",  WEAPON[4], NULL);
    ini.SetBoolValue("game", "Weapon6",  WEAPON[5], NULL);
    ini.SetBoolValue("game", "Weapon7",  WEAPON[6], NULL);
    ini.SetBoolValue("game", "Weapon8",  WEAPON[7], NULL);
    ini.SetBoolValue("game", "Weapon9",  WEAPON[8], NULL);
    ini.SetBoolValue("game", "Weapon10", WEAPON[9], NULL);
    ini.SetBoolValue("game", "Weapon11", WEAPON[10], NULL);
    ini.SetBoolValue("game", "Weapon12", WEAPON[11], NULL);
    ini.SetBoolValue("game", "Weapon13", WEAPON[12], NULL);
    ini.SetBoolValue("game", "Weapon14", WEAPON[13], NULL);

    ini.SetBoolValue("game", "BonusBerserker", BONUSES[0], NULL);
    ini.SetBoolValue("game", "BonusCluster",   BONUSES[1], NULL);
    ini.SetBoolValue("game", "BonusFlameGod",  BONUSES[3], NULL);
    ini.SetBoolValue("game", "BonusPredator",  BONUSES[5], NULL);
    ini.SetBoolValue("game", "BonusVest",      BONUSES[6], NULL);

    ini.SetLongValue("controls", "Left",     KEY_LEFT,     NULL);
    ini.SetLongValue("controls", "Right",    KEY_RIGHT,    NULL);
    ini.SetLongValue("controls", "Jump",     KEY_UP,       NULL);
    ini.SetLongValue("controls", "Crouch",   KEY_DOWN,     NULL);
    ini.SetLongValue("controls", "Reload",   KEY_RELOAD,   NULL);
    ini.SetLongValue("controls", "Grenade",  KEY_GRENADE,  NULL);
    ini.SetLongValue("controls", "Chat",     KEY_CHAT,     NULL);
    ini.SetLongValue("controls", "TeamChat", KEY_TEAMCHAT, NULL);

    ini.SaveFile(game.CH_CONFIG_FILE.c_str());

    return 0;
}