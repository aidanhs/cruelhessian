/*   gui.cpp
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
#include "CEGUIDefaultResourceProvider.h"

#include "Game.h"
#include "GUI.h"
#include "Parser.h"


//#define _(string) gettext(string)


void handle_mouse_down(Uint8 button)
{

    switch (button)
    {
    case SDL_BUTTON_LEFT:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
        break;
    case SDL_BUTTON_MIDDLE:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        break;
    case SDL_BUTTON_RIGHT:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
        break;

    case SDL_BUTTON_WHEELDOWN:
        CEGUI::System::getSingleton().injectMouseWheelChange(-1);
        break;
    case SDL_BUTTON_WHEELUP:
        CEGUI::System::getSingleton().injectMouseWheelChange(+1);
        break;
    default:
        break;
    }

}


void handle_mouse_up(Uint8 button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
        break;
    case SDL_BUTTON_MIDDLE:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
        break;
    case SDL_BUTTON_RIGHT:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
        break;
    default:
        break;
    }
}


void GUI::inject_input(bool & must_quitt)
{
    SDL_Event e;
    /* go through all available events */
    while (SDL_PollEvent(&e))
    {
        /* we use a switch to determine the event type */
        switch (e.type)
        {
            /* mouse motion handler */
        case SDL_MOUSEMOTION:
            /* we inject the mouse position directly. */
            CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(e.motion.x),static_cast<float>(e.motion.y));
            break;

            /* mouse down handler */
        case SDL_MOUSEBUTTONDOWN:
            /* let a special function handle the mouse button down event */
            handle_mouse_down (e.button.button);
            break;

            /* mouse up handler */
        case SDL_MOUSEBUTTONUP:
            /* let a special function handle the mouse button up event */
            handle_mouse_up (e.button.button);
            break;

            /* key down */
        case SDL_KEYDOWN:
            /* to tell CEGUI that a key was pressed, we inject the scancode. */
            CEGUI::System::getSingleton().injectKeyDown(e.key.keysym.scancode);
            /* as for the character it's a litte more complicated.
             * we'll use for translated unicode value.
             * this is described in more detail below.
             */
            if ((e.key.keysym.unicode & 0xFF80) == 0)
            {
                CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode & 0x7F);
            }
            GUI_KEY_PRESSED = e.key.keysym.unicode;

            break;

            /* key up */
        case SDL_KEYUP:
            /* like before we inject the scancode directly. */
            CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
            break;

            /* WM quit event occured */
        case SDL_QUIT:
            must_quitt = true;
            break;

        case SDL_VIDEORESIZE:
            renderer->grabTextures();
            //your resize code here, including the SDL_SetVideoMode call
            renderer->restoreTextures();
            renderer->setDisplaySize(CEGUI::Size(static_cast<float>(e.resize.w), static_cast<float>(e.resize.h)));
            break;

        default:
            break;
        }
    }
}



void inject_time_pulse(float& last_time_pulse)
{
    /* get current "run-time" in seconds */
    float t = 0.001f * SDL_GetTicks();
    /* inject the time that passed since the last call */
    CEGUI::System::getSingleton().injectTimePulse(t - last_time_pulse);
    /* store the new time as the last time */
    last_time_pulse = t;
}


// when pressed OK button in Folder Selector
bool GUI::FolderSelectorClick(const CEGUI::EventArgs &)
{

    const unsigned int max_string = 25;
    std::string temp = game.SOL_PATH = fs.getResult();
    if (game.SOL_PATH[game.SOL_PATH.length()-1] != '/')
    {
        game.SOL_PATH += '/';
    }

    if (temp.length() > max_string)
    {
        temp.replace(max_string, temp.length() - max_string, "...");
    }

    if (game.MODE == 0)
    {
        game.SOL_PATH_0 = game.SOL_PATH;
        mPlaceSoldat->setText(temp);
        checkSoldat();
    }
    else
    {
        game.SOL_PATH_1 = game.SOL_PATH;
        mPlaceCH->setText(temp);
        checkCH();
    }

    return true;

}


// when pressed Cancel button in Window Updater
bool GUI::WindowUpdaterClick(const CEGUI::EventArgs &)
{
    checkCH();

    return true;
}


void GUI::run()
{

    float last_time_pulse = 0.001f * static_cast<float>(SDL_GetTicks());

    while (!must_quit)
    {
        inject_input(must_quit);
        inject_time_pulse(last_time_pulse);
        glClear(GL_COLOR_BUFFER_BIT);
        CEGUI::System::getSingleton().renderGUI();

        if (WINDOW_NO_SOLDAT)
        {
            wns.show(CEGUI::System::getSingleton().getGUISheet());

            WINDOW_NO_SOLDAT = false;
        }

        SDL_GL_SwapBuffers();
    }

}


bool GUI::onFolderSelector(const CEGUI::EventArgs& )
{
    fs.show(CEGUI::System::getSingleton().getGUISheet());
    CEGUI::WindowManager::getSingleton().getWindow("FolderSelector/Frame/Ok")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::FolderSelectorClick, this));

    return true;
}


bool GUI::onWindowUpdater(const CEGUI::EventArgs& )
{
    wup.show(CEGUI::System::getSingleton().getGUISheet());
    CEGUI::WindowManager::getSingleton().getWindow("WindowUpdater/Frame/Cancel")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::WindowUpdaterClick, this));

    return true;
}


GUI::~GUI()
{
    delete renderer;
}


GUI::GUI()
{

    read_configs();

    ONLY_ON_START = true;

    //  FOLDER_SELECTOR = WINDOW_UPDATER = false;
    must_quit = false;

    game.setSDL();

    renderer = new CEGUI::OpenGLRenderer(0, static_cast<int>(game.MAX_WIDTH), static_cast<int>(game.MAX_HEIGHT));
    new CEGUI::System(renderer);

    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Errors);

    CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", game.CH_DATA_DIRECTORY + "schemes/");
    rp->setResourceGroupDirectory("imagesets", game.CH_DATA_DIRECTORY + "imagesets/");
    rp->setResourceGroupDirectory("fonts", game.CH_DATA_DIRECTORY + "fonts/");
    rp->setResourceGroupDirectory("layouts", game.CH_DATA_DIRECTORY + "layouts/");
    rp->setResourceGroupDirectory("looknfeel", game.CH_DATA_DIRECTORY + "looknfeel/");

    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");

    try
    {
        CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

        CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
        //CEGUI::SchemeManager::getSingleton().loadScheme("VanillaSkin.scheme");
        //CEGUI::SchemeManager::getSingleton().loadScheme("SleekSpace.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
        //CEGUI::System::getSingleton().setDefaultMouseCursor("Vanilla", "MouseArrow");

        if (!CEGUI::FontManager::getSingleton().isFontPresent("DejaVuSans-10"))
            CEGUI::FontManager::getSingleton().createFont("DejaVuSans-10.font");

        CEGUI::Window* sheet = winMgr.createWindow("DefaultWindow", "root_wnd");
        CEGUI::System::getSingleton().setGUISheet(sheet);

        CEGUI::Window* guiLayout = winMgr.loadWindowLayout("chessian.layout");
        sheet->addChildWindow(guiLayout);

        mTab = static_cast<CEGUI::TabControl*>(winMgr.getWindow("root/1"));

        // Maps
        mTab->getTabContentsAtIndex(0)->setText((CEGUI::utf8*)_("Maps"));

        mMapListDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/MapListDesc"));
        mMapListDesc->setText((CEGUI::utf8*)((CEGUI::utf8*)_("         Map List\nClick once to add to playlist")));
        mMapList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/maps/MapList"));
        mMapList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&GUI::onMapListClicked, this));
        mMapList->setSortingEnabled(true);

        mMapPlayListDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/PlayMapListDesc"));
        mMapPlayListDesc->setText((CEGUI::utf8*)((CEGUI::utf8*)_("         Play List\n  Click once to remove")));
        mMapPlayList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/maps/PlayMapList"));
        mMapPlayList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&GUI::onMapPlayListClicked, this));

        mSpinn1 = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/Spinn1"));
        mSpinn1->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinner1Changed, this));
        mSpinn2 = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/Spinn2"));
        mSpinn2->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinner2Changed, this));

        mDescription = static_cast<CEGUI::MultiLineEditbox*>(winMgr.getWindow("root/maps/Description"));

        mDesc1 = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/Desc1"));
        mDesc2 = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/Desc2"));
        mDesc2->setText((CEGUI::utf8*)((CEGUI::utf8*)_("Time limit")));

        mMapType = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/maps/Type"));
        mMapType->setText((CEGUI::utf8*)_("Type"));

        mDeathmatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/DeathButton"));
        mDeathmatch->setSelected(true);
        mDeathmatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onDeathClick, this));

        mPointmatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/PointButton"));
        mPointmatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onPointClick, this));

        mRambomatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/RamboButton"));
        mRambomatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onRamboClick, this));

        mTeammatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/TeamButton"));
        mTeammatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onTeamClick, this));

        mCTFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/CTFButton"));
        mCTFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onCTFClick, this));

        mHTFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/HTFButton"));
        mHTFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onHTFClick, this));

        mINFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/INFButton"));
        mINFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onINFClick, this));

        mStartGameButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/maps/StartGameButton"));
        mStartGameButton->setEnabled(false);
        mStartGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mRandomBotsDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/RandomBotsDesc"));
        mRandomBotsDesc->setText((CEGUI::utf8*)_("Random bots"));
        mRandomBotsSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/RandomBotsSpinner"));
        mRandomBotsSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinnerRandomBotsChanged, this));
        mBotsGroup = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/maps/BotsGroup"));
        mBotsGroup->setText((CEGUI::utf8*)_("Bots team"));
        mAlphaDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/AlphaDesc"));
        mBravoDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/BravoDesc"));
        mCharlieDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/CharlieDesc"));
        mCharlieDesc->setEnabled(false);
        mDeltaDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/DeltaDesc"));
        mDeltaDesc->setEnabled(false);

        mAlphaSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/AlphaSpinner"));
        mAlphaSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinnerAlphaBotsChanged, this));
        mBravoSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/BravoSpinner"));
        mBravoSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinnerBravoBotsChanged, this));
        mCharlieSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/CharlieSpinner"));
        mCharlieSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinnerCharlieBotsChanged, this));
        mCharlieSpinner->setEnabled(false);
        mDeltaSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/DeltaSpinner"));
        mDeltaSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSpinnerDeltaBotsChanged, this));
        mDeltaSpinner->setEnabled(false);


        // Player
        mTab->getTabContentsAtIndex(1)->setText((CEGUI::utf8*)_("Player"));

        mPlayerName = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/playerName"));
        mPlayerName->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUI::onPlayerName, this));

        mFireButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/fireButton"));
        mFireButton->setText((CEGUI::utf8*)_("Fire"));
        mFireButton->setEnabled(false);
        mFireButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mFlyButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/flyButton"));
        mFlyButton->setText((CEGUI::utf8*)_("Fly"));
        mFlyButton->setEnabled(false);
        mFlyButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mJumpButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/jumpButton"));
        mJumpButton->setText((CEGUI::utf8*)_("Jump"));
        mJumpButton->setEnabled(false);
        mJumpButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onJumpButton, this));

        mMoveLeftButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/moveLeftButton"));
        mMoveLeftButton->setText((CEGUI::utf8*)_("Move Left"));
        mMoveLeftButton->setEnabled(false);
        mMoveLeftButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onMoveLeftButton, this));

        mMoveRightButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/moveRightButton"));
        mMoveRightButton->setText((CEGUI::utf8*)_("Move Right"));
        mMoveRightButton->setEnabled(false);
        mMoveRightButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onMoveRightButton, this));

        mCrouchButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/crouchButton"));
        mCrouchButton->setText((CEGUI::utf8*)_("Crouch"));
        mCrouchButton->setEnabled(false);
        mCrouchButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onCrouchButton, this));

        mProneButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/proneButton"));
        mProneButton->setText((CEGUI::utf8*)_("Prone"));
        mProneButton->setEnabled(false);
        mProneButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mThrowGrenadeButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/throwGrenadeButton"));
        mThrowGrenadeButton->setText((CEGUI::utf8*)_("Throw Grenade"));
        mThrowGrenadeButton->setEnabled(false);
        mThrowGrenadeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onThrowGrenadeButton, this));

        mDropWeaponButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/dropWeaponButton"));
        mDropWeaponButton->setText((CEGUI::utf8*)_("Drop Weapon"));
        mDropWeaponButton->setEnabled(false);
        mDropWeaponButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mChatButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/chatButton"));
        mChatButton->setText((CEGUI::utf8*)_("Chat"));
        mChatButton->setEnabled(false);
        mChatButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onChatButton, this));

        mWeaponsMenuButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/weaponsMenuButton"));
        mWeaponsMenuButton->setText((CEGUI::utf8*)_("Weapons Menu"));
        mWeaponsMenuButton->setEnabled(false);
        mWeaponsMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mChangeWeaponButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/changeWeaponButton"));
        mChangeWeaponButton->setText((CEGUI::utf8*)_("Change Weapon"));
        mChangeWeaponButton->setEnabled(false);
        mChangeWeaponButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mReloadWeaponButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/reloadWeaponButton"));
        mReloadWeaponButton->setText((CEGUI::utf8*)_("Reload Weapon"));
        mReloadWeaponButton->setEnabled(false);
        mReloadWeaponButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onReloadWeaponButton, this));

        mTeamChatButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/teamChatButton"));
        mTeamChatButton->setText((CEGUI::utf8*)_("Team Chat"));
        mTeamChatButton->setEnabled(false);
        mTeamChatButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onTeamChatButton, this));

        mRadioMenuButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/radioMenuButton"));
        mRadioMenuButton->setText((CEGUI::utf8*)_("Radio Menu"));
        mRadioMenuButton->setEnabled(false);
        mRadioMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));


        // Options
        mTab->getTabContentsAtIndex(2)->setText((CEGUI::utf8*)_("Options"));

        // soldat midtab
        mSoldatRadio = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/options/soldatRadio"));
        mSoldatRadio->setText((CEGUI::utf8*)_("Soldat mode"));
        mSoldatRadio->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onSoldatModeClick, this));

        mPlaceSoldatDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldatDesc"));
        mPlaceSoldatDesc->setText((CEGUI::utf8*)_("Directory with Soldat files :"));

        mPlaceSoldat = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/placeSoldat"));
        mPlaceSoldat->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onFolderSelector, this));

        mStatusSoldatField = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/statusSoldatField"));

        // CH midtab
        mCHRadio = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/options/cruelRadio"));
        mCHRadio->setText((CEGUI::utf8*)_("Cruel Hessian mode"));
        mCHRadio->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&GUI::onCHModeClick, this));

        mPlaceCHDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeCHDesc"));
        mPlaceCHDesc->setText((CEGUI::utf8*)_("Directory with CH files :"));

        mPlaceCH = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/placeCH"));
        mPlaceCH->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onFolderSelector, this));

        mStatusCHField = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/statusCHField"));

        mUpdateButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/updateButton"));
        mUpdateButton->setText((CEGUI::utf8*)_("Check for update"));
        mUpdateButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onWindowUpdater, this));


        // Interface midtab
        mInterfacesDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/interfacesDesc"));
        mInterfacesDesc->setText((CEGUI::utf8*)_("Interface type :"));

        mInterfaces = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/InterfacesBox"));




        // Graphics
        mTab->getTabContentsAtIndex(3)->setText((CEGUI::utf8*)_("Graphics"));

        mIsFullscreen = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/graphics/IsFullscreen"));
        mIsFullscreen->setText((CEGUI::utf8*)_("Fullscreen"));
        mIsFullscreen->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&GUI::onGraphicsChanged, this));

        mDeep = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/graphics/Deep"));
        mDeep->setText((CEGUI::utf8*)_("Deep"));
        mDeep16 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Deep16"));
        mDeep16->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onGraphicsChanged, this));
        mDeep32 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Deep32"));
        mDeep32->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onGraphicsChanged, this));

        mResol = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/graphics/Resol"));
        mResol->setText((CEGUI::utf8*)_("Resolution"));
        mResol640 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol640"));
        mResol640->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onGraphicsChanged, this));
        mResol800 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol800"));
        mResol800->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onGraphicsChanged, this));
        mResol1024 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol1024"));
        mResol1024->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onGraphicsChanged, this));

        // Music
        mTab->getTabContentsAtIndex(4)->setText((CEGUI::utf8*)_("Music"));

        mIsSounds = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/music/IsSounds"));
        mIsSounds->setText((CEGUI::utf8*)_("Sounds"));
        mIsSounds->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&GUI::onSoundBoxChanged, this));
        mSoundsDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/SoundsDesc"));
        mSoundsDesc->setText((CEGUI::utf8*)_("Sounds Volume (%)"));
        mSoundsSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/music/SoundsSpinner"));
        mSoundsSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onSoundSpinnerChanged, this));

        mIsMusic = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/music/IsMusic"));
        mIsMusic->setText((CEGUI::utf8*)_("Music"));
        mIsMusic->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&GUI::onMusicBoxChanged, this));
        mMusicDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/MusicDesc"));
        mMusicDesc->setText((CEGUI::utf8*)_("Music Volume (%)"));
        mMusicSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/music/MusicSpinner"));
        mMusicSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&GUI::onMusicSpinnerChanged, this));

        mMusicSongDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/MusicSongDesc"));
        //mMusicSongDesc->setText((CEGUI::utf8*)_("In-game music. First selected will be played"));
        //mMusicList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/music/MusicList"));
        //mMusicList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&GUI::onMusicListChanged, this));

        mGroupQuality = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/music/GroupQuality"));
        mGroupQuality->setText((CEGUI::utf8*)_("Sounds Quality"));
        mLowQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/LowQuality"));
        mLowQuality->setText((CEGUI::utf8*)_("Low"));
        mLowQuality->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onAudioQualityChanged, this));
        mMediumQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/MediumQuality"));
        mMediumQuality->setText((CEGUI::utf8*)_("Medium"));
        mMediumQuality->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onAudioQualityChanged, this));
        mHighQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/HighQuality"));
        mHighQuality->setText((CEGUI::utf8*)_("High"));
        mHighQuality->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUI::onAudioQualityChanged, this));


        // Exit
        mTab->getTabContentsAtIndex(5)->setText((CEGUI::utf8*)_("Exit"));

        mExitButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/exit/ExitButton"));
        mExitButton->setText((CEGUI::utf8*)_("Save & Exit"));
        mExitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickExit, this));

        apply_configs();

        // activate DM mode on default
        CEGUI::EventArgs ev;
        mDeathmatch->setSelected(true);
        onDeathClick(ev);

        run();

    }
    catch (CEGUI::Exception &e)
    {
        std::cerr << "Error initializing Cruel Hessian : " << e.getMessage().c_str() << std::endl;
    }

}
