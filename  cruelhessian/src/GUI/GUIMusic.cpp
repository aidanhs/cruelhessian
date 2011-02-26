/*   GUIMusic.cpp
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
#include "boost/filesystem/fstream.hpp"

#include "../AudioManager.h"
#include "../ParserManager.h"
#include "../Regex.h"
#include "GUI.h"
#include "../Game.h"



int GUI::readM3U()
{

    std::string fold_mp3 = Parser.GAME_PATH + "Mp3/", buffer;

    Audio.ClearPlaylist();

    if (!boost::filesystem::exists(fold_mp3))
    {
        mMusicSongDesc->setText("'Mp3' directory doesn't exist !");
        return 1;
    }

    std::vector<std::string> gM3UFiles;
    std::string re = Parser.GAME_PATH + "Mp3/.+.(M3U|m3u)$";
    boost::filesystem::directory_iterator end;

    for (boost::filesystem::directory_iterator iter(fold_mp3); iter != end; ++iter)
    {
        if (regex_match(iter->path().string(), re))
        {
            // m3u file was found
            gM3UFiles.push_back(iter->path().string());
        }
    }

    if (gM3UFiles.empty())
    {
        mMusicSongDesc->setText("'Mp3' directory doesn't contain m3u files !");
        return 1;
    }
    else
    {
        std::ostringstream oss;
        oss << gM3UFiles.size();
        std::string text = "Found ";
        text += oss.str();
        text += " playlist(s)";
        mMusicSongDesc->setText(text);
    }

    for (unsigned int i = 0; i < gM3UFiles.size(); ++i)
    {
        std::ifstream file((gM3UFiles[i]).c_str());

        if (!file.is_open())
        {
            std::cerr << "Error opening file " << gM3UFiles[i] << std::endl;
            return -1;
        }

        while (!file.eof())
        {
            getline(file, buffer);
            if (buffer[0] != '#' && buffer[0] != '\n' && buffer != "")
            {
                Audio.AddToPlaylist(buffer);
            }

        }

        file.close();
    }

    return 0;

}


void GUI::setMusicStates(bool state)
{

    mLowQuality->setEnabled(state);
    mMediumQuality->setEnabled(state);
    mHighQuality->setEnabled(state);
    mGroupQuality->setEnabled(state);

}



bool GUI::onSoundBoxChanged(const CEGUI::EventArgs& )
{
    if (!mIsSounds->isSelected())
    {
        mSoundsSpinner->setEnabled(false);
        mSoundsDesc->setEnabled(false);
        Parser.SOUNDS_VOL = 0.0f;
        if (!mIsMusic->isSelected())
        {
            // nie ma niczego
            if (AudioManager::GetSingletonPtr() != NULL)
            {
                delete game.audioMgr;
            }
            setMusicStates(false);
        }
//std::cout << "SDFDSF" << std::endl;
    }
    else
    {
        mSoundsSpinner->setEnabled(true);
        mSoundsSpinner->setCurrentValue(Parser.SOUNDS_VOL);
        mSoundsDesc->setEnabled(true);
        setMusicStates(true);

        // create (if not exists) AudioManager - not implemented
        if (AudioManager::GetSingletonPtr() == NULL)
        {
            game.audioMgr = new AudioManager;
        }
        Parser.SOUNDS_VOL = mSoundsSpinner->getCurrentValue();
    }

    if (AudioManager::GetSingletonPtr() != NULL)
        Audio.setVolume();

    return true;
}


bool GUI::onSoundSpinnerChanged(const CEGUI::EventArgs& )
{

    if (mIsSounds->isSelected())
    {
        Parser.SOUNDS_VOL = mSoundsSpinner->getCurrentValue();
    }

    if (AudioManager::GetSingletonPtr() != NULL)
    {
        Audio.setVolume();
    }

    return true;
}


bool GUI::onMusicBoxChanged(const CEGUI::EventArgs& )
{

    if (!mIsMusic->isSelected())
    {
        mMusicSpinner->setEnabled(false);
        mMusicDesc->setEnabled(false);
        mMusicSongDesc->setEnabled(false);
        Parser.MUSIC_VOL = 0;
        if (!mIsSounds->isSelected())
        {
            // nie ma niczego
            if (AudioManager::GetSingletonPtr() != NULL)
            {
                delete game.audioMgr;
            }
            setMusicStates(false);
        }

    }
    else
    {
        mMusicSpinner->setEnabled(true);
        mMusicSpinner->setCurrentValue(Parser.MUSIC_VOL);
        mMusicDesc->setEnabled(true);
        mMusicSongDesc->setEnabled(true);

        // create (if not exists) AudioManager - not implemented
        if (AudioManager::GetSingletonPtr() == NULL)
        {
            game.audioMgr = new AudioManager;
        }
        Parser.MUSIC_VOL = mMusicSpinner->getCurrentValue();

        readM3U();
        setMusicStates(true);
    }

    return true;
}


bool GUI::onMusicSpinnerChanged(const CEGUI::EventArgs& )
{
    if (mIsMusic->isSelected())
    {
        Parser.MUSIC_VOL = mMusicSpinner->getCurrentValue();
    }

    if (AudioManager::GetSingletonPtr() != NULL)
        Audio.setVolume();
    return true;
}


bool GUI::onAudioQualityChanged(const CEGUI::EventArgs& )
{
    if (mLowQuality->isSelected())
        Parser.AUDIO_QUAL = 11025;
    else if (mMediumQuality->isSelected())
        Parser.AUDIO_QUAL = 22050;
    else if (mHighQuality->isSelected())
        Parser.AUDIO_QUAL = 44100;
    return true;
}
