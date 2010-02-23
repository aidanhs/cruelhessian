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


//#include <codecvt_utf.h>
//#include "boost/detail/utf8_codecvt_facet.hpp"
//#include "boost/archive/codecvt_null.hpp"
//#include "boost/filesystem/fstream.hpp"

#include "gui.h"
#include "globals.h"
#include "parser/SimpleIni.h"




// apply in interface
int GUI::apply_configs()
{

    if (checkSoldat())
    {
        mPlaceSoldat->setText(SOL_PATH);
        mStatusField->setText((CEGUI::utf8*)_("Found !"));
        showInterfaces();
        CEGUI::EventArgs ev;
        mDeathmatch->setSelected(true);
        onDeathClick(ev);
    }
    else
    {
        mStatusField->setText((CEGUI::utf8*)_("Not found !"));
        mInterfaces->resetList();
        mTab->setSelectedTabAtIndex(2);
        FOLDER_SELECTOR = true;
    }

    mPlayerName->setText(PLAYER_NAME);

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

