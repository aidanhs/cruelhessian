#ifndef GUI_H
#define GUI_H

/*   GUI.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
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



#include "CEGUI.h"
#include "RendererModules/OpenGLGUIRenderer/openglrenderer.h"
#include "GUIWindowFolderSelector.h"
//#include "GUI/GUIWindowKeySelector.h"
#include "GUIWindowNoSoldat.h"
#include "GUIWindowUpdater.h"
#include "../Singleton.h"
#include <SFML/Graphics/RenderWindow.hpp>
//#include "SDL.h"

#define _(string) (string)

//void handle_mouse_down(Uint8 button);
//void handle_mouse_up(Uint8 button);
//void inject_time_pulse(float& last_time_pulse);


class GUI : public Singleton<GUI>
{

public:

    GUI();
    virtual ~GUI();

    class MyListItem : public CEGUI::ListboxTextItem
    {
    public:
        MyListItem (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
        {
            setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
        }
    };

private:
void initializeMaps();
    bool ONLY_ON_START;
   // void handle_mouse_down(Uint8 button);
   // void handle_mouse_up(Uint8 button);
   bool handleEvent(sf::Event& Event);
   CEGUI::Key::Scan toCEGUIKey(sf::Key::Code Code);
   CEGUI::MouseButton toCEGUIMouseButton(sf::Mouse::Button Button);
  //  void inject_time_pulse(float& last_time_pulse);
    int catchKey(CEGUI::PushButton *whon);

    bool must_quit;

    void run();

    GUIWindowFolderSelector fs;
    //GUIWindowKeySelector wks;
    GUIWindowNoSoldat wns;
    GUIWindowUpdater wup;

	//sf::RenderWindow* mWindow;
	const sf::Input* mInput;

	CEGUI::System* mSystem;
	CEGUI::OpenGLRenderer* mRenderer;
//	CEGUI::WindowManager* mWindowManager;


    typedef std::map<sf::Key::Code, CEGUI::Key::Scan> KeyMap;
	typedef std::map<sf::Mouse::Button, CEGUI::MouseButton> MouseButtonMap;

	KeyMap mKeyMap;
	MouseButtonMap mMouseButtonMap;

//	void initializeMaps();
//	CEGUI::Key::Scan toCEGUIKey(sf::Key::Code Code);
//	CEGUI::MouseButton toCEGUIMouseButton(sf::Mouse::Button Button);

    bool WINDOW_NO_SOLDAT;

    int ApplyConfigs();
    void showMaps(const std::string& mask);
    void showInterfaces();
    void selectInterface();
 //   void inject_input(bool & must_quit);
    bool FolderSelectorClick(const CEGUI::EventArgs& );
    bool KeySelectorClick(const CEGUI::EventArgs& );
    bool WindowUpdaterClick(const CEGUI::EventArgs& );
    void setBotStates(bool a, bool b, bool c, bool d);
    void setMusicStates(bool state);
    int readM3U();
    bool onMapListClicked(const CEGUI::EventArgs& );
    bool onMapPlayListClicked(const CEGUI::EventArgs& );
    bool onSpinner1Changed(const CEGUI::EventArgs& );
    bool onSpinner2Changed(const CEGUI::EventArgs& );
    bool onSpinnerRandomBotsChanged(const CEGUI::EventArgs& );
    bool onSpinnerAlphaBotsChanged(const CEGUI::EventArgs& );
    bool onSpinnerBravoBotsChanged(const CEGUI::EventArgs& );
    bool onSpinnerCharlieBotsChanged(const CEGUI::EventArgs& );
    bool onSpinnerDeltaBotsChanged(const CEGUI::EventArgs& );
    bool onMusicBoxChanged(const CEGUI::EventArgs& );
    bool onMusicSpinnerChanged(const CEGUI::EventArgs& );
    bool onSoundBoxChanged(const CEGUI::EventArgs& );
    bool onSoundSpinnerChanged(const CEGUI::EventArgs& );
    bool onSoldatModeClick(const CEGUI::EventArgs& );
    bool onCHModeClick(const CEGUI::EventArgs& );
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
    bool onPlayerName(const CEGUI::EventArgs& );
    bool onHairComboClick(const CEGUI::EventArgs& );
    bool onFlameComboClick(const CEGUI::EventArgs& );
    bool onShirtComboClick(const CEGUI::EventArgs& );
    bool onPantsComboClick(const CEGUI::EventArgs& );
    bool onJumpButton(const CEGUI::EventArgs& );
    bool onMoveLeftButton(const CEGUI::EventArgs& );
    bool onMoveRightButton(const CEGUI::EventArgs& );
    bool onCrouchButton(const CEGUI::EventArgs& );
    bool onThrowGrenadeButton(const CEGUI::EventArgs& );
    bool onReloadWeaponButton(const CEGUI::EventArgs& );
    bool onChatButton(const CEGUI::EventArgs& );
    bool onTeamChatButton(const CEGUI::EventArgs& );
    bool onFolderSelector(const CEGUI::EventArgs& );
    bool onWindowUpdater(const CEGUI::EventArgs& );
    bool onKeySelectorEnter(const CEGUI::EventArgs& );
    bool onKeySelectorLeave(const CEGUI::EventArgs& );
    bool onInterfaceListClick(const CEGUI::EventArgs& );
    bool onGraphicsChanged(const CEGUI::EventArgs& );

    bool setKeysState(bool state);

    void checkSoldat();
    void checkCH();


    CEGUI::Window *myRoot;
   // CEGUI::OpenGLRenderer *renderer;
    CEGUI::Combobox *mSchemes;

    CEGUI::TabControl *mTab;
    //CEGUI::GroupBox* mGroupOpt1;
    //CEGUI::GroupBox* mGroupOpt2;

// maps & exit
    CEGUI::GroupBox *mMapType;
    CEGUI::PushButton* mStartGameButton;
    CEGUI::Listbox *mMapList;
    CEGUI::Listbox *mMapPlayList;
    CEGUI::Editbox *mMapListDesc;
    CEGUI::Editbox *mMapPlayListDesc;
    CEGUI::Editbox *mRandomBotsDesc;
    CEGUI::Spinner *mRandomBotsSpinner;

    CEGUI::PushButton *mExitButton;
    CEGUI::MultiLineEditbox *mDescription;
    CEGUI::Editbox *mDesc1;
    CEGUI::Editbox *mDesc2;
    CEGUI::Spinner *mSpinn1;
    CEGUI::Spinner *mSpinn2;
    CEGUI::GroupBox *mBotsGroup;
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

// player
    CEGUI::Editbox *mPlayerName;
    CEGUI::Editbox *mPlayerNameDesc;
    CEGUI::Editbox *mColorsDesc;
    CEGUI::Editbox *mHairDesc;
    CEGUI::Editbox *mPantsDesc;
    CEGUI::Editbox *mFlameDesc;
    CEGUI::Editbox *mShirtDesc;
    CEGUI::Combobox *mHairCombo;
    CEGUI::Combobox *mPantsCombo;
    CEGUI::Combobox *mFlameCombo;
    CEGUI::Combobox *mShirtCombo;
    CEGUI::Editbox *mKey;
    CEGUI::Editbox *mKeyDesc;
    CEGUI::PushButton *mFireButton;
    CEGUI::PushButton *mFlyButton;
    CEGUI::PushButton *mJumpButton;
    CEGUI::PushButton *mMoveLeftButton;
    CEGUI::PushButton *mMoveRightButton;
    CEGUI::PushButton *mCrouchButton;
    CEGUI::PushButton *mProneButton;
    CEGUI::PushButton *mThrowGrenadeButton;
    CEGUI::PushButton *mDropWeaponButton;
    CEGUI::PushButton *mChatButton;
    CEGUI::PushButton *mWeaponsMenuButton;
    CEGUI::PushButton *mChangeWeaponButton;
    CEGUI::PushButton *mReloadWeaponButton;
    CEGUI::PushButton *mTeamChatButton;
    CEGUI::PushButton *mRadioMenuButton;

// graphics
    CEGUI::Checkbox *mIsFullscreen;
    CEGUI::GroupBox *mDeep;
    CEGUI::GroupBox *mResol;
    CEGUI::RadioButton *mDeep16;
    CEGUI::RadioButton *mDeep32;
    CEGUI::RadioButton *mResol640;
    CEGUI::RadioButton *mResol800;
    CEGUI::RadioButton *mResol1024;

// music
    CEGUI::GroupBox* mGroupQuality;
    CEGUI::Editbox* mSoundsDesc;
    CEGUI::Editbox* mMusicDesc;
    CEGUI::Editbox* mMusicSongDesc;
    CEGUI::Checkbox* mIsSounds;
    CEGUI::Checkbox* mIsMusic;
    CEGUI::Spinner* mSoundsSpinner;
    CEGUI::Spinner* mMusicSpinner;
    CEGUI::RadioButton* mLowQuality;
    CEGUI::RadioButton* mMediumQuality;
    CEGUI::RadioButton* mHighQuality;


// options
    CEGUI::RadioButton* mSoldatRadio;
    CEGUI::RadioButton* mCHRadio;
    CEGUI::PushButton *mUpdateButton;
    CEGUI::Combobox* mInterfaces;
    CEGUI::Editbox* mInterfacesDesc;
    CEGUI::PushButton *mPlaceSoldat;
   // CEGUI::PushButton *mPlaceCH;
    CEGUI::Editbox *mPlaceSoldatDesc;
   // CEGUI::Editbox *mPlaceCHDesc;
    CEGUI::Editbox *mStatusSoldatField;
   // CEGUI::Editbox *mStatusCHField;

};

#define gui GUI::GetSingleton()


#endif

