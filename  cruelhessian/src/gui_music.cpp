/*   gui_music.cpp
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
//#include <locale.h>
//#include <libintl.h>
#include "boost/filesystem/fstream.hpp"
#include "boost/regex.hpp"

#include "gui.h"
#include "globals.h"

//#define _(string) gettext(string)


int GUI::readM3U()
{
    std::string fold_mp3 = SOL_PATH + "Mp3/", buffer;
    std::vector<std::string> gM3UFiles;
    boost::regex re(SOL_PATH + "Mp3/.+.(m3u|M3U)");
    boost::filesystem::directory_iterator end;

    gMusicList.clear();

    if (!boost::filesystem::exists(fold_mp3))
    {
        mMusicSongDesc->setText("'Mp3' directory doesn't exist !");
        return 1;
    }

    for (boost::filesystem::directory_iterator iter(fold_mp3); iter != end; ++iter)
    {
        if (boost::regex_match(iter->path().string(), re))
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
                gMusicList.push_back(buffer);
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
        SOUNDS_VOL = 0;
        if (!mIsMusic->isSelected())
            setMusicStates(false);
    }
    else
    {
        mSoundsSpinner->setEnabled(true);
        mSoundsSpinner->setCurrentValue(SOUNDS_VOL);
        mSoundsDesc->setEnabled(true);
        SOUNDS_VOL = mSoundsSpinner->getCurrentValue();
        setMusicStates(true);
    }
    return true;
}


bool GUI::onSoundSpinnerChanged(const CEGUI::EventArgs& )
{
    if (mIsSounds->isSelected())
    {
        SOUNDS_VOL = mSoundsSpinner->getCurrentValue();
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
        MUSIC_VOL = 0;
        if (!mIsSounds->isSelected())
            setMusicStates(false);
    }
    else
    {
        mMusicSpinner->setEnabled(true);
        mMusicSpinner->setCurrentValue(MUSIC_VOL);
        mMusicDesc->setEnabled(true);
        mMusicSongDesc->setEnabled(true);
        MUSIC_VOL = mMusicSpinner->getCurrentValue();
        readM3U();
        setMusicStates(true);
    }

    return true;
}


bool GUI::onMusicSpinnerChanged(const CEGUI::EventArgs& )
{
    if (mIsMusic->isSelected())
    {
        MUSIC_VOL = mMusicSpinner->getCurrentValue();
    }
    return true;
}


bool GUI::onAudioQualityChanged(const CEGUI::EventArgs& )
{
    if (mLowQuality->isSelected())
        AUDIO_QUAL = 11025;
    else if (mMediumQuality->isSelected())
        AUDIO_QUAL = 22050;
    else if (mHighQuality->isSelected())
        AUDIO_QUAL = 44100;
    return true;
}
