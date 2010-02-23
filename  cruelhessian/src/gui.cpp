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
#include <string>
#include <sstream>
#include <fstream>
//#include <locale.h>
//#include <libintl.h>
#include "boost/filesystem/fstream.hpp"
#include "boost/regex.hpp"
#include "CEGUIDefaultResourceProvider.h"

#include "gui.h"
#include "globals.h"
#include "worldmap.h"

//#define _(string) gettext(string)


void GUI::showMaps(const char* mask)
{
    std::string str, temp;
    boost::regex re(SOL_PATH + "Maps/" + mask);
    boost::filesystem::directory_iterator end;

    if (!boost::filesystem::exists(SOL_PATH + "Maps/"))
    {
        std::cout << "'Maps' directory doesn't exist !" << std::endl;
        mStartGameButton->setEnabled(false);
        return;
    }

    mMapList->resetList();

    for (boost::filesystem::directory_iterator iter(SOL_PATH + "Maps/"); iter != end; ++iter)
    {
        if (boost::regex_match(iter->path().string(), re))
        {
            str.assign(iter->path().string().begin() + SOL_PATH.length() + 5, iter->path().string().end() - 4);
            mMapList->addItem(new MyListItem(str));
        }
    }
}


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



void GUI::showInterfaces()
{

    std::string str;
    std::string fold_in = SOL_PATH + "Interface-gfx/";
    std::string fold_cus = SOL_PATH + "Custom-Interfaces/";
    boost::filesystem::directory_iterator end;

    if (!boost::filesystem::exists(fold_in))
    {
        std::cout << "'Interface-gfx' directory doesn't exist !" << std::endl;
        //return false;
    }

    if (!boost::filesystem::exists(fold_cus))
    {
        std::cout << "'Custom-Interfaces' directory doesn't exist !" << std::endl;
        //return false;
    }

    mInterfaces->addItem(new MyListItem("Default"));
    for (boost::filesystem::directory_iterator iter(fold_cus); iter != end; ++iter)
    {
        str.assign(iter->path().string().begin() + fold_cus.length(), iter->path().string().end());
        mInterfaces->addItem(new MyListItem(str));
        //  str.assign(dp->d_name);
        // if ((str != ".") && (str != ".."))
        //   mInterfaces->addItem(new MyListItem(str));
    }

    //mInterfaces->setSortingEnabled(true);
    mInterfaces->setItemSelectState(size_t(0), true);

    //return true;
}


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


void GUI::inject_input(bool & must_quit)
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
            must_quit = true;
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


bool GUI::handleClick(const CEGUI::EventArgs &)
{

    const unsigned int max_string = 25;
    std::string temp = SOL_PATH = fs.getResult();
    if (SOL_PATH[SOL_PATH.length()-1] != '/')
	{
		SOL_PATH += '/';
	}

    if (temp.length() > max_string)
    {
        temp.replace(max_string, temp.length() - max_string, "...");
    }
    mPlaceSoldat->setText(temp);

    if (checkSoldat())
    {
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
    }

    return true;

}

bool GUI::onJumpButton(const CEGUI::EventArgs &)
{
    KEY_UP = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onMoveLeftButton(const CEGUI::EventArgs &)
{
    KEY_LEFT = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onMoveRightButton(const CEGUI::EventArgs &)
{
    KEY_RIGHT = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onCrouchButton(const CEGUI::EventArgs &)
{
    KEY_DOWN = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onReloadWeaponButton(const CEGUI::EventArgs &)
{
    KEY_RELOAD = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onThrowGrenadeButton(const CEGUI::EventArgs &)
{
    KEY_GRENADE = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onChatButton(const CEGUI::EventArgs &)
{
    KEY_CHAT = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onTeamChatButton(const CEGUI::EventArgs &)
{
    KEY_TEAMCHAT = GUI_KEY_PRESSED;
    return true;
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


void GUI::setMusicStates(bool state)
{

    mLowQuality->setEnabled(state);
    mMediumQuality->setEnabled(state);
    mHighQuality->setEnabled(state);
    mGroupQuality->setEnabled(state);

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

        if (FOLDER_SELECTOR)
        {
            fs.show(CEGUI::System::getSingleton().getGUISheet());
            CEGUI::WindowManager::getSingleton().getWindow("FolderSelector/Frame/Ok")->
            subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::handleClick, this));

            FOLDER_SELECTOR = false;
        }

        SDL_GL_SwapBuffers();
    }

}


bool GUI::onPlayerName(const CEGUI::EventArgs& )
{

    PLAYER_NAME = mPlayerName->getText().c_str();
    return true;

}


bool GUI::onLeftClickStart(const CEGUI::EventArgs& )
{

    if (mMapPlayList->getItemCount() > 0)
    {
        WorldMap *newworld;
        std::string xmap = SOL_PATH + "Maps/" + mMapPlayList->getListboxItemFromIndex(0)->getText().c_str() + ".PMS";

        if (mInterfaces->getSelectedItem()->getText() == "Default")
        {
            INTERF_PATH = SOL_PATH + "Interface-gfx/";
        }
        else
        {
            INTERF_PATH = SOL_PATH + "Custom-Interfaces/" + mInterfaces->getSelectedItem()->getText().c_str() + "/";
        }

        FIRST_LIMIT = static_cast<int>(mSpinn1->getCurrentValue());
        TIME_LIMIT = 60*static_cast<int>(mSpinn2->getCurrentValue());

        if (CURRENT_GAME_MODE == CTF || CURRENT_GAME_MODE == HTF || CURRENT_GAME_MODE == INF || CURRENT_GAME_MODE == TM)
        {
            newworld = new WorldMap(xmap, mAlphaSpinner->isDisabled() ? 0 : static_cast<int>(mAlphaSpinner->getCurrentValue()),
                                    mBravoSpinner->isDisabled() ? 0 : static_cast<int>(mBravoSpinner->getCurrentValue()),
                                    mCharlieSpinner->isDisabled() ? 0 : static_cast<int>(mCharlieSpinner->getCurrentValue()),
                                    mDeltaSpinner->isDisabled() ? 0 : static_cast<int>(mDeltaSpinner->getCurrentValue()));
        }
        else
        {
            newworld = new WorldMap(xmap, static_cast<int>(mRandomBotsSpinner->getCurrentValue()));
        }

        newworld->run();

        delete newworld;

        mMapPlayList->resetList();
        mStartGameButton->setEnabled(false);
    }

    return true;
}


bool GUI::onLeftClickExit(const CEGUI::EventArgs& )
{
    save_configs();
    must_quit = true;
    return true;
}

bool GUI::onMapListClicked(const CEGUI::EventArgs& )
{
    mMapPlayList->addItem(new MyListItem(mMapList->getFirstSelectedItem()->getText()));
    mStartGameButton->setEnabled(true);
    return true;
}

bool GUI::onMapPlayListClicked(const CEGUI::EventArgs& )
{
    mMapPlayList->removeItem(mMapPlayList->getFirstSelectedItem());
    if (mMapPlayList->getItemCount() == 0)
        mStartGameButton->setEnabled(false);
    return true;
}

void GUI::setBotStates(bool a, bool b, bool c, bool d)
{
    mAlphaDesc->setEnabled(a);
    mAlphaSpinner->setEnabled(a);
    mBravoDesc->setEnabled(b);
    mBravoSpinner->setEnabled(b);
    mCharlieDesc->setEnabled(c);
    mCharlieSpinner->setEnabled(c);
    mDeltaDesc->setEnabled(d);
    mDeltaSpinner->setEnabled(d);
}


bool GUI::onDeathClick(const CEGUI::EventArgs& )
{
    showMaps("[A-Z].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Kill everything that moves.")));
    mDesc1->setText((CEGUI::utf8*)(_("Kill limit")));
    mSpinn1->setCurrentValue(30);
    mMapPlayList->resetList();
    setBotStates(false, false, false, false);
    CURRENT_GAME_MODE = DM;
    return true;
}

bool GUI::onPointClick(const CEGUI::EventArgs& )
{
    showMaps("[A-Z].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Get 1 point for kill. If you carry the Yellow Flag you get 2 points for kill. Also you get multipoints for multikills.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(30);
    mMapPlayList->resetList();
    setBotStates(false, false, false, false);
    CURRENT_GAME_MODE = PM;
    return true;
}

bool GUI::onTeamClick(const CEGUI::EventArgs& )
{
    showMaps("[A-Z].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Up to 4 teams fight against each other.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(60);
    mCharlieDesc->setEnabled(true);
    mCharlieSpinner->setEnabled(true);
    mDeltaDesc->setEnabled(true);
    mDeltaSpinner->setEnabled(true);
    setBotStates(true, true, true, true);
    mMapPlayList->resetList();
    CURRENT_GAME_MODE = TM;
    return true;
}


bool GUI::onRamboClick(const CEGUI::EventArgs& )
{
    showMaps("[A-Z].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("'First Blood' style. Whoever owns the Rambo Bow gains super Rambo powers. Only Rambo gets points for kill.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(30);
    mMapPlayList->resetList();
    setBotStates(false, false, false, false);
    CURRENT_GAME_MODE = RM;
    return true;
}

bool GUI::onCTFClick(const CEGUI::EventArgs& )
{
    showMaps("ctf_.+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Capture the enemy flag and bring it to your base to score. 20 points for capture.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(5);
    mMapPlayList->resetList();
    setBotStates(true, true, false, false);
    CURRENT_GAME_MODE = CTF;
    return true;
}

bool GUI::onHTFClick(const CEGUI::EventArgs& )
{
    showMaps("htf_.+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Get the yellow flag and hold it with your team for as long as possible. The team earns 1 point every 5 seconds of holding.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(80);
    mMapPlayList->resetList();
    setBotStates(true, true, false, false);
    CURRENT_GAME_MODE = HTF;
    return true;
}

bool GUI::onINFClick(const CEGUI::EventArgs& )
{
    showMaps("inf_.+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Red team gets 30 points for retreiving the black flag. Blue team gets 1 point every 5 seconds if the flag is in base.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(90);
    mMapPlayList->resetList();
    setBotStates(true, true, false, false);
    CURRENT_GAME_MODE = INF;
    return true;
}




bool GUI::onSearchSoldat(const CEGUI::EventArgs& )
{
    FOLDER_SELECTOR = true;
    return true;
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


bool GUI::onGraphicsChanged(const CEGUI::EventArgs& )
{
    if (mDeep16->isSelected())
        MAX_BPP = 16;
    else if (mDeep32->isSelected())
        MAX_BPP = 32;

    if (mResol640->isSelected())
    {
        MAX_WIDTH = 640.0f;
        MAX_HEIGHT = 480.0f;
    }
    else if (mResol800->isSelected())
    {
        MAX_WIDTH = 800.0f;
        MAX_HEIGHT = 600.0f;
    }
    else if (mResol1024->isSelected())
    {
        MAX_WIDTH = 1024.0f;
        MAX_HEIGHT = 768.0f;
    }

    glViewport(0, 0, static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, MAX_WIDTH, MAX_HEIGHT, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mIsFullscreen->isSelected())
    {
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
        FULLSCREEN = true;
    }
    else
    {
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);
        FULLSCREEN = false;
    }

    return true;
}


GUI::~GUI()
{
    delete renderer;
//    CEGUI::System::destroy();
//    CEGUI::OpenGLRenderer::destroy(*renderer);
}



GUI::GUI()
{
    read_configs();

    FOLDER_SELECTOR = false;
    must_quit = false;

    setSDL();
    /*
    glEnable(GL_CULL_FACE);
    glDisable(GL_FOG);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glViewport(0,0,static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, MAX_WIDTH/MAX_HEIGHT, 0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    */

    renderer = new CEGUI::OpenGLRenderer(0, static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT));
    new CEGUI::System(renderer);

	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Errors);

    CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", CH_DATA_DIRECTORY + "schemes/");
    rp->setResourceGroupDirectory("imagesets", CH_DATA_DIRECTORY + "imagesets/");
    rp->setResourceGroupDirectory("fonts", CH_DATA_DIRECTORY + "fonts/");
    rp->setResourceGroupDirectory("layouts", CH_DATA_DIRECTORY + "layouts/");
    rp->setResourceGroupDirectory("looknfeel", CH_DATA_DIRECTORY + "looknfeel/");

    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");

    try
    {
        CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

        CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

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
        mSpinn2 = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/Spinn2"));

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
        mBotsGroup = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/maps/BotsGroup"));
        mBotsGroup->setText((CEGUI::utf8*)_("Bots team"));
        mAlphaDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/AlphaDesc"));
        mBravoDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/BravoDesc"));
        mCharlieDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/CharlieDesc"));
        mCharlieDesc->setEnabled(false);
        mDeltaDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/DeltaDesc"));
        mDeltaDesc->setEnabled(false);

        mAlphaSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/AlphaSpinner"));
        mBravoSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/BravoSpinner"));
        mCharlieSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/CharlieSpinner"));
        mCharlieSpinner->setEnabled(false);
        mDeltaSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/DeltaSpinner"));
        mDeltaSpinner->setEnabled(false);

        // Connections
        /*   mServersList = static_cast<CEGUI::MultiColumnList*>(winMgr.getWindow("root/connections/ServersList"));
           mDolaczGameButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/connections/DolaczGameButton"));
           mDolaczGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&onLeftClickDolacz));

           mIPNumberBox = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/connections/IPNumberBox"));
           mPortNumberBox = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/connections/PortNumberBox"));
           //mIPNumberButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&onLeftClickDolacz));
        */

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

        /*mLanguage = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/Language"));
        mLanguage->addItem(new MyListItem("English"));
        mLanguage->addItem(new MyListItem("Polish"));
        mLanguage->setSortingEnabled(true);
        mLanguage->setReadOnly(true);
        mLanguage->setItemSelectState(size_t(0), true);*/
        //mPlaceSoldat = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldat"));

        mPlaceSoldatDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldatDesc"));
        mPlaceSoldatDesc->setText((CEGUI::utf8*)_("Directory with Soldat files :"));

        mPlaceSoldat = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/placeSoldat"));
        mPlaceSoldat->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onSearchSoldat, this));

        mInterfacesDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/interfacesDesc"));
        mInterfacesDesc->setText((CEGUI::utf8*)_("Interface type :"));

        mInterfaces = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/InterfacesBox"));

        mStatusField = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/StatusField"));

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
    }
    catch (CEGUI::Exception &e)
    {
        std::cerr << "Error initializing Cruel Hessian : " << e.getMessage().c_str() << std::endl;
    }

}
