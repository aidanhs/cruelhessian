/*   parser.cpp
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


#include "GUI.h"
#include "Parser.h"
#include "Game.h"
#include "Globals.h"
#include "parser/SimpleIni.h"



int defaults()
{
    std::cout << "Restoring defaults ..." << std::endl;

    game.MODE         = 1;
    game.SOL_PATH_0   = "";
    game.SOL_PATH_1   = "";
    game.INTERFACE_NAME = "Default";
    game.INTERFACE_PATH  = "";
    game.FULLSCREEN   = 0;
    game.MAX_WIDTH    = 640.0f;
    game.MAX_HEIGHT   = 480.0f;
    game.MAX_BPP      = 16;
    game.SOUNDS_VOL   = 20.0f;
    game.MUSIC_VOL    = 0.0f;
    game.AUDIO_QUAL   = 44100;

    game.PLAYER_NAME = "Player";
    game.COLOR_SHIRT = game.tcolor2rgb("$00EBEBFF");
    game.COLOR_SKIN  = game.tcolor2rgb("$00EBEBFF");
    game.COLOR_HAIR  = game.tcolor2rgb("$00EBEBFF");
    game.COLOR_PANTS = game.tcolor2rgb("$00EBEBFF");
    game.COLOR_JET   = game.tcolor2rgb("$00EBEBFF");

    game.KEY_LEFT     = 97;
    game.KEY_RIGHT    = 100;
    game.KEY_DOWN     = 115;
    game.KEY_UP       = 119;
    game.KEY_RELOAD   = 114;
    game.KEY_GRENADE  = 32;
    game.KEY_CHAT     = 116;
    game.KEY_TEAMCHAT = 121;

    game.LIMIT_TEAMMATCH    = 60;
    game.LIMIT_DEATHMATCH   = 30;
    game.LIMIT_POINTMATCH   = 30;
    game.LIMIT_RAMBOMATCH   = 30;
    game.LIMIT_INFILTRATION = 90;
    game.LIMIT_HOLD         = 80;
    game.LIMIT_CAPTURE      = 5;
    game.LIMIT_TIME         = 15*60;

    game.CONSOLE_SHOW = true;

    game.RANDOM_BOTS   = 5;
    game.RANDOM_BOTS_1 = 1;
    game.RANDOM_BOTS_2 = 1;
    game.RANDOM_BOTS_3 = 1;
    game.RANDOM_BOTS_4 = 1;

    for (int i = 0; i < 14; ++i)
        game.WEAPON[i]  = true;

    for (int i = 0; i < 7; ++i)
        game.BONUSES[i]  = true;

    return 0;
}



int read_configs()
{

    CSimpleIni ini(false, false, false);

    if (ini.LoadFile(game.CH_CONFIG_FILE.c_str()) < 0)
    {
        defaults();
        return 1;
    }

    std::cout << "Loading configs from 'options.ini' ..." << std::endl;

    if (ini.GetLongValue("info", "Version") != game.CONFIG_VERSION)
    {
        std::cout << "New config file version" << std::endl;
        defaults();
        return 1;
    }

    game.MODE           = static_cast<float>(ini.GetLongValue("global", "Mode"));
    game.SOL_PATH_0     = ini.GetValue("global", "SoldatPath");
    game.SOL_PATH_1     = ini.GetValue("global", "CruelHessianPath");
    game.INTERFACE_NAME = ini.GetValue("global", "Interface");
    game.FULLSCREEN     = ini.GetBoolValue("global", "Fullscreen");
    game.MAX_WIDTH      = static_cast<float>(ini.GetLongValue("global", "MaxWidth"));
    game.MAX_HEIGHT     = static_cast<float>(ini.GetLongValue("global", "MaxHeight"));
    game.MAX_BPP        = ini.GetLongValue("global", "MaxDeep");
    game.SOUNDS_VOL     = static_cast<float>(ini.GetLongValue("global", "SoundVolume"));
    game.MUSIC_VOL      = static_cast<float>(ini.GetLongValue("global", "MusicVolume"));
    game.AUDIO_QUAL     = ini.GetLongValue("global", "AudioQuality");

    game.LIMIT_TEAMMATCH    = ini.GetLongValue("game", "TeammatchLimit");
    game.LIMIT_DEATHMATCH   = ini.GetLongValue("game", "DeathmatchLimit");
    game.LIMIT_POINTMATCH   = ini.GetLongValue("game", "PointmatchLimit");
    game.LIMIT_RAMBOMATCH   = ini.GetLongValue("game", "RambomatchLimit");
    game.LIMIT_INFILTRATION = ini.GetLongValue("game", "InfiltrationLimit");
    game.LIMIT_HOLD         = ini.GetLongValue("game", "HoldLimit");
    game.LIMIT_CAPTURE      = ini.GetLongValue("game", "CaptureLimit");
    game.LIMIT_TIME         = 60*ini.GetLongValue("game", "TimeLimit");

    game.CONSOLE_SHOW = ini.GetBoolValue("game", "ConsoleShow");

    game.RANDOM_BOTS   = ini.GetLongValue("game", "RandomBots");
    game.RANDOM_BOTS_1 = ini.GetLongValue("game", "RandomBots1");
    game.RANDOM_BOTS_2 = ini.GetLongValue("game", "RandomBots2");
    game.RANDOM_BOTS_3 = ini.GetLongValue("game", "RandomBots3");
    game.RANDOM_BOTS_4 = ini.GetLongValue("game", "RandomBots4");

    game.PLAYER_NAME = ini.GetValue("player", "Name");
    game.COLOR_SHIRT = game.tcolor2rgb(ini.GetValue("player", "ShirtColor"));
    game.COLOR_SKIN  = game.tcolor2rgb(ini.GetValue("player", "SkinColor"));
    game.COLOR_HAIR  = game.tcolor2rgb(ini.GetValue("player", "HairColor"));
    game.COLOR_PANTS = game.tcolor2rgb(ini.GetValue("player", "PantsColor"));
    game.COLOR_JET   = game.tcolor2rgb(ini.GetValue("player", "JetColor"));

    game.KEY_LEFT     = ini.GetLongValue("controls", "Left");
    game.KEY_RIGHT    = ini.GetLongValue("controls", "Right");
    game.KEY_DOWN     = ini.GetLongValue("controls", "Crouch");
    game.KEY_UP       = ini.GetLongValue("controls", "Jump");
    game.KEY_RELOAD   = ini.GetLongValue("controls", "Reload");
    game.KEY_GRENADE  = ini.GetLongValue("controls", "Grenade");
    game.KEY_CHAT     = ini.GetLongValue("controls", "Chat");
    game.KEY_TEAMCHAT = ini.GetLongValue("controls", "TeamChat");

    game.WEAPON[0]  = ini.GetBoolValue("game", "Weapon1");
    game.WEAPON[1]  = ini.GetBoolValue("game", "Weapon2");
    game.WEAPON[2]  = ini.GetBoolValue("game", "Weapon3");
    game.WEAPON[3]  = ini.GetBoolValue("game", "Weapon4");
    game.WEAPON[4]  = ini.GetBoolValue("game", "Weapon5");
    game.WEAPON[5]  = ini.GetBoolValue("game", "Weapon6");
    game.WEAPON[6]  = ini.GetBoolValue("game", "Weapon7");
    game.WEAPON[7]  = ini.GetBoolValue("game", "Weapon8");
    game.WEAPON[8]  = ini.GetBoolValue("game", "Weapon9");
    game.WEAPON[9]  = ini.GetBoolValue("game", "Weapon10");
    game.WEAPON[10] = ini.GetBoolValue("game", "Weapon11");
    game.WEAPON[11] = ini.GetBoolValue("game", "Weapon12");
    game.WEAPON[12] = ini.GetBoolValue("game", "Weapon13");
    game.WEAPON[13] = ini.GetBoolValue("game", "Weapon14");

    game.BONUSES[0] = ini.GetBoolValue("game", "BonusBerserker");
    game.BONUSES[1] = ini.GetBoolValue("game", "BonusCluster");
    game.BONUSES[2] = false;
    game.BONUSES[3] = ini.GetBoolValue("game", "BonusFlameGod");
    game.BONUSES[4] = false;
    game.BONUSES[5] = ini.GetBoolValue("game", "BonusPredator");
    game.BONUSES[6] = ini.GetBoolValue("game", "BonusVest");

    if (game.MODE == 0)
        game.SOL_PATH = game.SOL_PATH_0;
    else
        game.SOL_PATH = game.SOL_PATH_1;

    if (!game.SOL_PATH.empty())
    {
        if (game.SOL_PATH[game.SOL_PATH.length()-1] == '\\')
        {
            //std::cout << "JEA" << std::endl;
            //SOL_PATH[SOL_PATH.length()-1] = '/';
        }
        else if (game.SOL_PATH[game.SOL_PATH.length()-1] != '/')
        {
            game.SOL_PATH += '/';
        }
        game.INTERFACE_PATH = game.SOL_PATH + "Interface-gfx/";
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



// apply in interface
int GUI::apply_configs()
{

    WINDOW_NO_SOLDAT = false;
    mPlaceSoldat->setText(game.SOL_PATH_0);
    mPlaceCH->setText(game.SOL_PATH_1);

    CEGUI::EventArgs ev;

    // soldat mode
    if (game.MODE == 0)
    {
        mSoldatRadio->setSelected(true);
        onSoldatModeClick(ev);
    }
    // cruel hessian mode
    else
    {
        mCHRadio->setSelected(true);
        onCHModeClick(ev);
    }


    mSpinn2->setCurrentValue(static_cast<int>(game.LIMIT_TIME/60));
    mRandomBotsSpinner->setCurrentValue(game.RANDOM_BOTS);
    mAlphaSpinner->setCurrentValue(game.RANDOM_BOTS_1);
    mBravoSpinner->setCurrentValue(game.RANDOM_BOTS_2);
    mCharlieSpinner->setCurrentValue(game.RANDOM_BOTS_3);
    mDeltaSpinner->setCurrentValue(game.RANDOM_BOTS_4);

    mPlayerName->setText(game.PLAYER_NAME);

    mIsFullscreen->setSelected(game.FULLSCREEN);

    // resolution
    if (game.MAX_WIDTH == 640 && game.MAX_HEIGHT == 480)
        mResol640->setSelected(true);
    else if (game.MAX_WIDTH == 800 && game.MAX_HEIGHT == 600)
        mResol800->setSelected(true);
    else if (game.MAX_WIDTH == 1024 && game.MAX_HEIGHT == 768)
        mResol1024->setSelected(true);


    // deep
    if (game.MAX_BPP == 16)
        mDeep16->setSelected(true);
    else if (game.MAX_BPP == 32)
        mDeep32->setSelected(true);


    // sounds
    if (game.SOUNDS_VOL == 0)
    {
        mIsSounds->setSelected(false);
        mSoundsSpinner->setEnabled(false);
    }
    else if (game.SOUNDS_VOL > 0 && game.SOUNDS_VOL <= 100)
    {
        mIsSounds->setSelected(true);
        mSoundsSpinner->setEnabled(true);
        mSoundsSpinner->setCurrentValue(game.SOUNDS_VOL);
    }

    // music
    if (game.MUSIC_VOL == 0)
    {
        mIsMusic->setSelected(false);
        mMusicSpinner->setEnabled(false);
//        mMusicList->setEnabled(false);
    }
    else if (game.MUSIC_VOL > 0 && game.MUSIC_VOL <= 100)
    {
        mIsMusic->setSelected(true);
        mMusicSpinner->setEnabled(true);
        mMusicSpinner->setCurrentValue(game.MUSIC_VOL);
//        mMusicList->setEnabled(true);
//        mMusicList->setItemSelectState((size_t)0, true);
    }

    if (game.AUDIO_QUAL == 11025)
    {
        mLowQuality->setSelected(true);
    }
    else if (game.AUDIO_QUAL == 22050)
    {
        mMediumQuality->setSelected(true);
    }
    else if (game.AUDIO_QUAL == 44100)
    {
        mHighQuality->setSelected(true);
    }

    if (game.SOUNDS_VOL + game.MUSIC_VOL == 0)
    {
        setMusicStates(false);
    }

    return 0;
}


int save_configs()
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

    ini.SetLongValue("global", "Mode",             static_cast<int>(game.MODE),       NULL);
    ini.SetValue(    "global", "SoldatPath",       game.SOL_PATH_0.c_str(),           NULL);
    ini.SetValue(    "global", "CruelHessianPath", game.SOL_PATH_1.c_str(),           NULL);
    ini.SetValue(    "global", "Interface",        game.INTERFACE_NAME.c_str(),       NULL);
    ini.SetBoolValue("global", "Fullscreen",       game.FULLSCREEN,                   NULL);
    ini.SetLongValue("global", "MaxWidth",         static_cast<int>(game.MAX_WIDTH),  NULL);
    ini.SetLongValue("global", "MaxHeight",        static_cast<int>(game.MAX_HEIGHT), NULL);
    ini.SetLongValue("global", "MaxDeep",          game.MAX_BPP,                      NULL);
    ini.SetLongValue("global", "SoundVolume",      static_cast<int>(game.SOUNDS_VOL), NULL);
    ini.SetLongValue("global", "MusicVolume",      static_cast<int>(game.MUSIC_VOL),  NULL);
    ini.SetLongValue("global", "AudioQuality",     game.AUDIO_QUAL,                   NULL);

    ini.SetValue("player", "Name",       game.PLAYER_NAME.c_str(),                  NULL);
    ini.SetValue("player", "ShirtColor", game.rgb2tcolor(game.COLOR_SHIRT).c_str(), NULL);
    ini.SetValue("player", "SkinColor",  game.rgb2tcolor(game.COLOR_SKIN).c_str(),  NULL);
    ini.SetValue("player", "HairColor",  game.rgb2tcolor(game.COLOR_HAIR).c_str(),  NULL);
    ini.SetValue("player", "PantsColor", game.rgb2tcolor(game.COLOR_PANTS).c_str(), NULL);
    ini.SetValue("player", "JetColor",   game.rgb2tcolor(game.COLOR_JET).c_str(),   NULL);

    ini.SetLongValue("game", "TeammatchLimit",    game.LIMIT_TEAMMATCH,                 NULL);
    ini.SetLongValue("game", "DeathmatchLimit",   game.LIMIT_DEATHMATCH,                NULL);
    ini.SetLongValue("game", "PointmatchLimit",   game.LIMIT_POINTMATCH,                NULL);
    ini.SetLongValue("game", "RambomatchLimit",   game.LIMIT_RAMBOMATCH,                NULL);
    ini.SetLongValue("game", "InfiltrationLimit", game.LIMIT_INFILTRATION,              NULL);
    ini.SetLongValue("game", "HoldLimit",         game.LIMIT_HOLD,                      NULL);
    ini.SetLongValue("game", "CaptureLimit",      game.LIMIT_CAPTURE,                   NULL);
    ini.SetLongValue("game", "TimeLimit",         static_cast<int>(game.LIMIT_TIME/60), NULL);

    ini.SetLongValue("game", "RandomBots",  game.RANDOM_BOTS,   NULL);
    ini.SetLongValue("game", "RandomBots1", game.RANDOM_BOTS_1, NULL);
    ini.SetLongValue("game", "RandomBots2", game.RANDOM_BOTS_2, NULL);
    ini.SetLongValue("game", "RandomBots3", game.RANDOM_BOTS_3, NULL);
    ini.SetLongValue("game", "RandomBots4", game.RANDOM_BOTS_4, NULL);

    ini.SetBoolValue("game", "ConsoleShow", game.CONSOLE_SHOW, NULL);
    ini.SetBoolValue("game", "Weapon1",  game.WEAPON[0], NULL);
    ini.SetBoolValue("game", "Weapon2",  game.WEAPON[1], NULL);
    ini.SetBoolValue("game", "Weapon3",  game.WEAPON[2], NULL);
    ini.SetBoolValue("game", "Weapon4",  game.WEAPON[3], NULL);
    ini.SetBoolValue("game", "Weapon5",  game.WEAPON[4], NULL);
    ini.SetBoolValue("game", "Weapon6",  game.WEAPON[5], NULL);
    ini.SetBoolValue("game", "Weapon7",  game.WEAPON[6], NULL);
    ini.SetBoolValue("game", "Weapon8",  game.WEAPON[7], NULL);
    ini.SetBoolValue("game", "Weapon9",  game.WEAPON[8], NULL);
    ini.SetBoolValue("game", "Weapon10", game.WEAPON[9], NULL);
    ini.SetBoolValue("game", "Weapon11", game.WEAPON[10], NULL);
    ini.SetBoolValue("game", "Weapon12", game.WEAPON[11], NULL);
    ini.SetBoolValue("game", "Weapon13", game.WEAPON[12], NULL);
    ini.SetBoolValue("game", "Weapon14", game.WEAPON[13], NULL);

    ini.SetBoolValue("game", "BonusBerserker", game.BONUSES[0], NULL);
    ini.SetBoolValue("game", "BonusCluster",   game.BONUSES[1], NULL);
    ini.SetBoolValue("game", "BonusFlameGod",  game.BONUSES[3], NULL);
    ini.SetBoolValue("game", "BonusPredator",  game.BONUSES[5], NULL);
    ini.SetBoolValue("game", "BonusVest",      game.BONUSES[6], NULL);

    ini.SetLongValue("controls", "Left",     game.KEY_LEFT,     NULL);
    ini.SetLongValue("controls", "Right",    game.KEY_RIGHT,    NULL);
    ini.SetLongValue("controls", "Jump",     game.KEY_UP,       NULL);
    ini.SetLongValue("controls", "Crouch",   game.KEY_DOWN,     NULL);
    ini.SetLongValue("controls", "Reload",   game.KEY_RELOAD,   NULL);
    ini.SetLongValue("controls", "Grenade",  game.KEY_GRENADE,  NULL);
    ini.SetLongValue("controls", "Chat",     game.KEY_CHAT,     NULL);
    ini.SetLongValue("controls", "TeamChat", game.KEY_TEAMCHAT, NULL);

    ini.SaveFile(game.CH_CONFIG_FILE.c_str());

    return 0;
}
