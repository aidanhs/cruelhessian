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

#include <iostream>
#include <fstream>

#include "gui.h"
#include "globals.h"
#include "parser/SimpleIni.h"


// 'soldat style' interface - fix !!

void CH_GUI::defaults()
{
    std::cout << "Restoring defaults ..." << std::endl;

    SOL_PATH = "";
    CH_INTERFACE = "Default";
    FULLSCREEN = 0;
    MAX_WIDTH = 640;
    MAX_HEIGHT = 480;
    MAX_BPP = 16;
    SOUNDS_VOL = 80.0f;
    MUSIC_VOL = 0.0f;
    AUDIO_QUAL = 44100;

    KEY_LEFT = 97;
    KEY_RIGHT = 100;
    KEY_DOWN = 115;
    KEY_UP = 119;
    KEY_RELOAD = 114;
    KEY_GRENADE = 32;
}

int CH_GUI::read_configs()
{
    CSimpleIni ini(false, false, false);

    if (ini.LoadFile(CH_CONFIG_FILE.c_str()) < 0)
    {
        defaults();
        return 0;
    }

    std::cout << "Loading configs from 'options.ini' ..." << std::endl;

    SOL_PATH = ini.GetValue("global", "SoldatPath");
    CH_INTERFACE = ini.GetValue("global", "Interface");
    FULLSCREEN = ini.GetBoolValue("global", "Fullscreen");
    MAX_WIDTH = ini.GetLongValue("global", "MaxWidth");
    MAX_HEIGHT = ini.GetLongValue("global", "MaxHeight");
    MAX_BPP = ini.GetLongValue("global", "MaxDeep");
    SOUNDS_VOL = static_cast<float>(ini.GetLongValue("global", "SoundVolume"));
    MUSIC_VOL = static_cast<float>(ini.GetLongValue("global", "MusicVolume"));
    AUDIO_QUAL = ini.GetLongValue("global", "AudioQuality");

    KEY_LEFT = ini.GetLongValue("controls", "Left");
    KEY_RIGHT = ini.GetLongValue("controls", "Right");
    KEY_DOWN = ini.GetLongValue("controls", "Crouch");
    KEY_UP = ini.GetLongValue("controls", "Jump");
    KEY_RELOAD = ini.GetLongValue("controls", "Reload");
    KEY_GRENADE = ini.GetLongValue("controls", "Grenade");

    return 0;
}



// apply in interface
int CH_GUI::apply_configs()
{

    mPlaceSoldat->setText(SOL_PATH);
    if (!checkSoldat())
    {
        mTab->setSelectedTabAtIndex(2);
    }

    mIsFullscreen->setSelected(FULLSCREEN);

    // resolution
    if (MAX_WIDTH == 640 && MAX_HEIGHT == 480)
        mResol640->setSelected(true);
    else if (MAX_WIDTH == 800 && MAX_HEIGHT == 600)
        mResol800->setSelected(true);
    else if (MAX_WIDTH == 1024 && MAX_HEIGHT == 768)
        mResol1024->setSelected(true);


    // deep
    if (MAX_BPP == 16)
        mDeep16->setSelected(true);
    else if (MAX_BPP == 32)
        mDeep32->setSelected(true);


    // sounds
    if (SOUNDS_VOL == 0)
    {
        mIsSounds->setSelected(false);
        mSoundsSpinner->setEnabled(false);
    }
    else if (SOUNDS_VOL > 0 && SOUNDS_VOL <= 100)
    {
        mIsSounds->setSelected(true);
        mSoundsSpinner->setEnabled(true);
        mSoundsSpinner->setCurrentValue(SOUNDS_VOL);
    }

    // music
    if (MUSIC_VOL == 0)
    {
        mIsMusic->setSelected(false);
        mMusicSpinner->setEnabled(false);
//        mMusicList->setEnabled(false);
    }
    else if (MUSIC_VOL > 0 && MUSIC_VOL <= 100)
    {
        mIsMusic->setSelected(true);
        mMusicSpinner->setEnabled(true);

        mMusicSpinner->setCurrentValue(MUSIC_VOL);
//        mMusicList->setEnabled(true);
//        mMusicList->setItemSelectState((size_t)0, true);
    }

    if (AUDIO_QUAL == 11025)
    {
        mLowQuality->setSelected(true);
    }
    else if (AUDIO_QUAL == 22050)
    {
        mMediumQuality->setSelected(true);
    }
    else if (AUDIO_QUAL == 44100)
    {
        mHighQuality->setSelected(true);
    }

    if (SOUNDS_VOL + MUSIC_VOL == 0)
    {
        setMusicStates(false);
    }

    return 0;
}


int CH_GUI::save_configs()
{
    std::cout << "Saving configs to 'options.ini' ..." << std::endl;

    CSimpleIni ini(false, false, false);

    if (!std::ifstream(CH_CONFIG_FILE.c_str()))
    {
        std::ifstream o_file(CH_CONFIG_FILE.c_str());
        o_file.close();
    }

    ini.LoadFile(CH_CONFIG_FILE.c_str());

    ini.SetValue("global", "SoldatPath", SOL_PATH.c_str(), NULL);
    ini.SetValue("global", "Interface", CH_INTERFACE.c_str(), NULL);
    ini.SetBoolValue("global", "Fullscreen", FULLSCREEN, NULL);
    ini.SetLongValue("global", "MaxWidth", MAX_WIDTH, NULL);
    ini.SetLongValue("global", "MaxHeight", MAX_HEIGHT, NULL);
    ini.SetLongValue("global", "MaxDeep", mDeep16->isSelected() ? 16 : 32, NULL);
    ini.SetLongValue("global", "SoundVolume", static_cast<int>(SOUNDS_VOL), NULL);
    ini.SetLongValue("global", "MusicVolume", static_cast<int>(MUSIC_VOL), NULL);
    ini.SetLongValue("global", "AudioQuality", AUDIO_QUAL, NULL);

    ini.SetLongValue("controls", "Left", KEY_LEFT, NULL);
    ini.SetLongValue("controls", "Right", KEY_RIGHT, NULL);
    ini.SetLongValue("controls", "Jump", KEY_UP, NULL);
    ini.SetLongValue("controls", "Crouch", KEY_DOWN, NULL);
    ini.SetLongValue("controls", "Reload", KEY_RELOAD, NULL);
    ini.SetLongValue("controls", "Grenade", KEY_GRENADE, NULL);

    ini.SaveFile(CH_CONFIG_FILE.c_str());

    return 0;
}
