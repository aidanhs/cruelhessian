/*   gui.h
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

#ifndef GUI_H
#define GUI_H

#include "CEGUI.h"
#include "RendererModules/OpenGLGUIRenderer/openglrenderer.h"
#include "CEGUIDefaultResourceProvider.h"
#include "worldmap.h"


class CH_GUI// : public WorldMap
{

public:
    void init();
    void run();
    void quit();

    //CH_GUI() {};
    //~CH_GUI() {};

private:

    SDL_Surface *screen;
    bool must_quit;
    class MyListItem : public CEGUI::ListboxTextItem
    {
    public:
        MyListItem (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
        {
            setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
        }
    };

    bool FULLSCREEN;

    int MAX_BPP;

    void defaults();
    int read_configs();
    int apply_configs();
    int save_configs();
    void showMaps(const char* mask);
    void showInterfaces();
    void inject_input(bool & must_quit);
    void setBotStates(bool a, bool b, bool c, bool d);
    void setMusicStates(bool state);
    int readM3U();
    bool onMapListClicked(const CEGUI::EventArgs& );
    bool onMapPlayListClicked(const CEGUI::EventArgs& );
    bool onMusicBoxChanged(const CEGUI::EventArgs& );
    bool onMusicSpinnerChanged(const CEGUI::EventArgs& );
    bool onSoundBoxChanged(const CEGUI::EventArgs& );
    bool onSoundSpinnerChanged(const CEGUI::EventArgs& );
    bool onAudioQualityChanged(const CEGUI::EventArgs& );
    bool onLeftClickStart(const CEGUI::EventArgs& );
    bool onLeftClickExit(const CEGUI::EventArgs& );
    bool onDeathClick(const CEGUI::EventArgs& );
    bool onPointClick(const CEGUI::EventArgs& );
    bool onTeamClick(const CEGUI::EventArgs& );
    bool onRamboClick(const CEGUI::EventArgs& );
    bool onCTFClick(const CEGUI::EventArgs& );
    bool onHTFClick(const CEGUI::EventArgs& );
    bool onINFClick(const CEGUI::EventArgs& );
    bool checkSoldat();
    bool onSearchSoldat(const CEGUI::EventArgs& );
    bool onGraphicsChanged(const CEGUI::EventArgs& );


    CEGUI::Window *myRoot;
    CEGUI::OpenGLRenderer *renderer;
    CEGUI::Combobox *mSchemes;

    CEGUI::TabControl *mTab;

// maps & exit
    CEGUI::GroupBox *mMapType;
    CEGUI::PushButton* mStartGameButton;
    CEGUI::Listbox *mMapList;
    CEGUI::Listbox *mMapPlayList;
    CEGUI::Editbox *mMapListDesc;
    CEGUI::Editbox *mMapPlayListDesc;

//CEGUI::PushButton* mDolaczGameButton;
//CEGUI::Editbox* mIPNumberBox;
//CEGUI::Editbox* mPortNumberBox;
    CEGUI::PushButton *mExitButton;
    CEGUI::MultiLineEditbox *mDescription;
    CEGUI::Editbox *mDesc1;
    CEGUI::Editbox *mDesc2;
    CEGUI::Spinner *mSpinn1;
    CEGUI::Spinner *mSpinn2;
    CEGUI::Spinner *mAlphaSpinner;
    CEGUI::Spinner *mBravoSpinner;
    CEGUI::Spinner *mCharlieSpinner;
    CEGUI::Spinner *mDeltaSpinner;
    CEGUI::Editbox *mAlphaDesc;
    CEGUI::Editbox *mBravoDesc;
    CEGUI::Editbox *mCharlieDesc;
    CEGUI::Editbox *mDeltaDesc;
    CEGUI::RadioButton *mCTFButton;
    CEGUI::RadioButton *mHTFButton;
    CEGUI::RadioButton *mINFButton;
    CEGUI::RadioButton *mDeathmatch;
    CEGUI::RadioButton *mPointmatch;
    CEGUI::RadioButton *mRambomatch;
    CEGUI::RadioButton *mTeammatch;

// graphics
    CEGUI::Checkbox *mIsFullscreen;
    CEGUI::GroupBox *mDeep;
    CEGUI::GroupBox *mResol;
    CEGUI::RadioButton *mDeep16;
    CEGUI::RadioButton *mDeep32;
    CEGUI::RadioButton *mResol640;
    CEGUI::RadioButton *mResol800;
    CEGUI::RadioButton *mResol1024;
    //CEGUI::Editbox* mInfoRestart;

// music
    CEGUI::GroupBox* mGroupQuality;
    CEGUI::Editbox* mSoundsDesc;
    CEGUI::Editbox* mMusicDesc;
    CEGUI::Editbox* mMusicSongDesc;
    CEGUI::Checkbox* mIsSounds;
    CEGUI::Checkbox* mIsMusic;
    CEGUI::Spinner* mSoundsSpinner;
    CEGUI::Spinner* mMusicSpinner;
 //   CEGUI::Listbox* mMusicList;
    CEGUI::RadioButton* mLowQuality;
    CEGUI::RadioButton* mMediumQuality;
    CEGUI::RadioButton* mHighQuality;


// options
//CEGUI::Combobox* mLanguage;
    CEGUI::Combobox* mInterfaces;
    CEGUI::Editbox* mInterfacesDesc;
    CEGUI::Editbox *mPlaceSoldat;
    CEGUI::Editbox *mPlaceSoldatDesc;
//CEGUI::Combobox *mPlaceSoldat;
    CEGUI::Editbox *mStatusField;

};


#endif
