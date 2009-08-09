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
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <locale.h>
#include <libintl.h>
#include <sys/stat.h>
#include "regex.h"
#include "CEGUI.h"
#include "RendererModules/OpenGLGUIRenderer/openglrenderer.h"
#include "CEGUIDefaultResourceProvider.h"

#include "gui.h"
#include "globals.h"
#include "worldmap.h"

//#include "CEGUICommonFileDialog.h"

#define _(string) gettext(string)
//#define _(string) (string)





void CH_GUI::showMaps(const char* mask)
{
    struct dirent *dp;
    std::string str, temp;
    regex_t preg;
    regcomp(&preg, mask, REG_EXTENDED);
    std::string fold = SOL_PATH + "Maps/";
    DIR *dirp = opendir(fold.c_str());

    if (dirp == NULL)
    {
        std::cout << "'Maps' directory doesn't exist !" << std::endl;
        mStartGameButton->setEnabled(false);
        closedir(dirp);
        regfree(&preg);
        return;
    }

    mMapList->resetList();

    while ((dp = readdir(dirp)) != NULL)
    {
        if (regexec(&preg, dp->d_name, 0, NULL, 0) == 0)
        {
            temp.assign(dp->d_name);
            str.assign(temp.begin(), temp.end()-4);
            mMapList->addItem(new MyListItem(str));
        }
    }

    regfree(&preg);
    closedir(dirp);
}


int CH_GUI::readM3U()
{
    struct dirent *dp;
    const char *mask = ".+.(m3u|M3U)$";
    std::string fold = SOL_PATH + "Mp3/", buffer;
    regex_t preg;
    regcomp(&preg, mask, REG_EXTENDED);
    DIR *dirp = opendir(fold.c_str());
    std::vector<std::string> gMusicFiles;

    gMusicList.clear();

    if (dirp == NULL)
    {
        mMusicSongDesc->setText("'Mp3' directory doesn't exist !");
        closedir(dirp);
        return 1;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        if (regexec(&preg, dp->d_name, 0, NULL, 0) == 0)
        {
            // m3u file was found
            gMusicFiles.push_back(dp->d_name);
        }
    }

    if (gMusicFiles.empty())
    {
        mMusicSongDesc->setText("'Mp3' directory doesn't contain m3u files !");
        return 1;
    }
    else
    {
        std::ostringstream oss;
        oss << gMusicFiles.size();
        std::string text = "Found ";
        text += oss.str();
        text += " playlist(s)";
        mMusicSongDesc->setText(text);
    }

    for (unsigned int i = 0; i < gMusicFiles.size(); ++i)
    {
        std::ifstream file((fold + gMusicFiles[i]).c_str());

        if (!file.is_open())
        {
            std::cerr << "ERROR opening file " << fold + gMusicFiles[i] << std::endl;
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



void CH_GUI::showInterfaces()
{
    struct dirent *dp;
    std::string str;
    std::string fold_in = SOL_PATH + "Interface-gfx/";
    std::string fold_cus = SOL_PATH + "Custom-Interfaces/";
    DIR *dirp_in, *dirp_cus;

    if ((dirp_in = opendir(fold_in.c_str())) == NULL)
    {
        std::cout << "'Interface-gfx' directory doesn't exist !" << std::endl;
        closedir(dirp_in);
        //return false;
    }

    if ((dirp_cus = opendir(fold_cus.c_str())) == NULL)
    {
        std::cout << "'Custom-Interfaces' directory doesn't exist !" << std::endl;
        closedir(dirp_cus);
        //return false;
    }

    mInterfaces->addItem(new MyListItem("Default"));
    while ((dp = readdir(dirp_cus)) != NULL)
    {
        str.assign(dp->d_name);
        if ((str != ".") && (str != ".."))
            mInterfaces->addItem(new MyListItem(str));
    }

    //mInterfaces->setSortingEnabled(true);
    mInterfaces->setItemSelectState(size_t(0), true);

    closedir(dirp_in);
    closedir(dirp_cus);
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
    }
}


void CH_GUI::inject_input(bool & must_quit)
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
            renderer->setDisplaySize(CEGUI::Size(e.resize.w, e.resize.h));
            break;
        }
    }
}


void inject_time_pulse(double& last_time_pulse)
{
    /* get current "run-time" in seconds */
    double t = 0.001*SDL_GetTicks();
    /* inject the time that passed since the last call */
    CEGUI::System::getSingleton().injectTimePulse(float(t-last_time_pulse));
    /* store the new time as the last time */
    last_time_pulse = t;
}

void CH_GUI::setMusicStates(bool state)
{
    mLowQuality->setEnabled(state);
    mMediumQuality->setEnabled(state);
    mHighQuality->setEnabled(state);
    mGroupQuality->setEnabled(state);
}



void CH_GUI::run()
{
    double last_time_pulse = 0.001*static_cast<double>(SDL_GetTicks());

    while (!must_quit)
    {
        inject_input(must_quit);
        inject_time_pulse(last_time_pulse);
        glClear(GL_COLOR_BUFFER_BIT);
        CEGUI::System::getSingleton().renderGUI();
        SDL_GL_SwapBuffers();
    }
}


bool CH_GUI::onLeftClickStart(const CEGUI::EventArgs& )
{

    if (mMapPlayList->getItemCount() > 0)
    {
        std::string map = SOL_PATH + "Maps/" + mMapPlayList->getListboxItemFromIndex(0)->getText().c_str() + ".PMS";

        if (mInterfaces->getSelectedItem()->getText() == "Default")
        {
            INTERF_PATH = SOL_PATH + "Interface-gfx/";
        }
        else
        {
            INTERF_PATH = SOL_PATH + "Custom-Interfaces/" + mInterfaces->getSelectedItem()->getText().c_str() + "/";
        }

        WorldMap *newworld = new WorldMap(map, mAlphaSpinner->isDisabled() ? 0 : static_cast<int>(mAlphaSpinner->getCurrentValue()),
                                          mBravoSpinner->isDisabled() ? 0 : static_cast<int>(mBravoSpinner->getCurrentValue()),
                                          mCharlieSpinner->isDisabled() ? 0 : static_cast<int>(mCharlieSpinner->getCurrentValue()),
                                          mDeltaSpinner->isDisabled() ? 0 : static_cast<int>(mDeltaSpinner->getCurrentValue()));
        newworld->run();
        delete newworld;

        mMapPlayList->resetList();
        mStartGameButton->setEnabled(false);
    }

    return true;
}


bool CH_GUI::onLeftClickExit(const CEGUI::EventArgs& )
{
    save_configs();
    must_quit = true;
    return true;
}

bool CH_GUI::onMapListClicked(const CEGUI::EventArgs& )
{
    mMapPlayList->addItem(new MyListItem(mMapList->getFirstSelectedItem()->getText()));
    mStartGameButton->setEnabled(true);
    return true;
}

bool CH_GUI::onMapPlayListClicked(const CEGUI::EventArgs& )
{
    mMapPlayList->removeItem(mMapPlayList->getFirstSelectedItem());
    if (mMapPlayList->getItemCount() == 0)
        mStartGameButton->setEnabled(false);
    return true;
}

void CH_GUI::setBotStates(bool a, bool b, bool c, bool d)
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


bool CH_GUI::onDeathClick(const CEGUI::EventArgs& )
{
    showMaps("^[[:upper:]].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Kill everything that moves.")));
    mDesc1->setText((CEGUI::utf8*)(_("Kill limit")));
    mSpinn1->setCurrentValue(30);
    setBotStates(false, false, false, false);
    return true;
}

bool CH_GUI::onPointClick(const CEGUI::EventArgs& )
{
    showMaps("^[[:upper:]].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Get 1 point for kill. If you carry the Yellow Flag you get 2 points for kill. Also you get multipoints for multikills.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(30);
    setBotStates(false, false, false, false);
    return true;
}

bool CH_GUI::onTeamClick(const CEGUI::EventArgs& )
{
    showMaps("^[[:upper:]].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Up to 4 teams fight against each other.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(60);
    mCharlieDesc->setEnabled(true);
    mCharlieSpinner->setEnabled(true);
    mDeltaDesc->setEnabled(true);
    mDeltaSpinner->setEnabled(true);
    setBotStates(true, true, true, true);
    return true;
}


bool CH_GUI::onRamboClick(const CEGUI::EventArgs& )
{

    showMaps("^[[:upper:]].+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("'First Blood' style. Whoever owns the Rambo Bow gains super Rambo powers. Only Rambo gets points for kill.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(30);
    setBotStates(false, false, false, false);
    return true;
}

bool CH_GUI::onCTFClick(const CEGUI::EventArgs& )
{

    showMaps("ctf_.+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Capture the enemy flag and bring it to your base to score. 20 points for capture.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(5);
    setBotStates(true, true, false, false);
    return true;
}

bool CH_GUI::onHTFClick(const CEGUI::EventArgs& )
{
    showMaps("htf_.+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Get the yellow flag and hold it with your team for as long as possible. The team earns 1 point every 5 seconds of holding.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(80);
    setBotStates(true, true, false, false);
    return true;
}

bool CH_GUI::onINFClick(const CEGUI::EventArgs& )
{
    showMaps("inf_.+.(PMS|pms)$");
    mDescription->setText((CEGUI::utf8*)(_("Red team gets 30 points for retreiving the black flag. Blue team gets 1 point every 5 seconds if the flag is in base.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(90);
    setBotStates(true, true, false, false);
    return true;
}


bool CH_GUI::checkSoldat()
{

    struct dirent *dp;
    DIR *dirp = opendir(mPlaceSoldat->getText().c_str());

    if (dirp == NULL)
    {
        mStatusField->setText((CEGUI::utf8*)_("Not found !"));
        return false;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        if (strcmp(dp->d_name, "Soldat.exe") == 0)
        {
            SOL_PATH = mPlaceSoldat->getText().c_str();
            if (SOL_PATH[SOL_PATH.length()-1] != '/')
                SOL_PATH += '/';
            mStatusField->setText((CEGUI::utf8*)_("Found !"));
            showInterfaces();
            //mInterfaces->setItemSelectState(size_t(0), true);
            closedir(dirp);
            CEGUI::EventArgs ev;
            mCTFButton->setSelected(true);
            onCTFClick(ev);
            return true;
        }
    }
    mStatusField->setText((CEGUI::utf8*)_("Not found !"));
    mInterfaces->resetList();
    closedir(dirp);
    return false;
}


bool CH_GUI::onSearchSoldat(const CEGUI::EventArgs& )
{
    checkSoldat();
    return true;
}

bool CH_GUI::onSoundBoxChanged(const CEGUI::EventArgs& )
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

bool CH_GUI::onSoundSpinnerChanged(const CEGUI::EventArgs& )
{
    if (mIsSounds->isSelected())
    {
        SOUNDS_VOL = mSoundsSpinner->getCurrentValue();
    }
    return true;
}


bool CH_GUI::onMusicBoxChanged(const CEGUI::EventArgs& )
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

bool CH_GUI::onMusicSpinnerChanged(const CEGUI::EventArgs& )
{
    if (mIsMusic->isSelected())
    {
        MUSIC_VOL = mMusicSpinner->getCurrentValue();
    }
    return true;
}


bool CH_GUI::onAudioQualityChanged(const CEGUI::EventArgs& )
{
    if (mLowQuality->isSelected())
        AUDIO_QUAL = 11025;
    else if (mMediumQuality->isSelected())
        AUDIO_QUAL = 22050;
    else if (mHighQuality->isSelected())
        AUDIO_QUAL = 44100;
    return true;
}

bool CH_GUI::onGraphicsChanged(const CEGUI::EventArgs& )
{
    if (mDeep16->isSelected())
        MAX_BPP = 16;
    else if (mDeep32->isSelected())
        MAX_BPP = 32;

    if (mResol640->isSelected())
    {
        MAX_WIDTH = 640;
        MAX_HEIGHT = 480;
    }
    else if (mResol800->isSelected())
    {
        MAX_WIDTH = 800;
        MAX_HEIGHT = 600;
    }
    else if (mResol1024->isSelected())
    {
        MAX_WIDTH = 1024;
        MAX_HEIGHT = 768;
    }
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, MAX_WIDTH, MAX_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, MAX_WIDTH, MAX_HEIGHT, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mIsFullscreen->isSelected())
    {
        screen = SDL_SetVideoMode(MAX_WIDTH, MAX_HEIGHT, MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
        FULLSCREEN = true;
    }
    else
    {
        screen = SDL_SetVideoMode(MAX_WIDTH, MAX_HEIGHT, MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);
        FULLSCREEN = false;
    }

    return true;
}



void CH_GUI::quit()
{
    delete renderer;
}



void CH_GUI::init()
{
    read_configs();

    must_quit = false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
    {
        std::cout << "Unable to set attributes SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (FULLSCREEN)
        screen = SDL_SetVideoMode(MAX_WIDTH, MAX_HEIGHT, MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(MAX_WIDTH, MAX_HEIGHT, MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);

    if (!screen)
    {
        std::cout << "Unable to set video mode: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("Cruel Hessian", "");

    renderer = new CEGUI::OpenGLRenderer(0, MAX_WIDTH, MAX_HEIGHT);
    new CEGUI::System(renderer);

    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Errors);

    CEGUI::DefaultResourceProvider *rp = reinterpret_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", CH_DATA_DIRECTORY + "schemes/");
    rp->setResourceGroupDirectory("fonts", CH_DATA_DIRECTORY + "fonts/");
    rp->setResourceGroupDirectory("imagesets", CH_DATA_DIRECTORY + "imagesets/");
    rp->setResourceGroupDirectory("looknfeel", CH_DATA_DIRECTORY + "looknfeel/");
    rp->setResourceGroupDirectory("layouts", CH_DATA_DIRECTORY + "layouts/");

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

        //CEGUI::Window* guiLayout = winMgr.loadWindowLayout(CH_DATA_DIRECTORY + "layouts/chessian.layout");
        CEGUI::Window* guiLayout = winMgr.loadWindowLayout("chessian.layout");
        sheet->addChildWindow(guiLayout);

        mTab = static_cast<CEGUI::TabControl*>(winMgr.getWindow("root/1"));

        // Maps
        mTab->getTabContentsAtIndex(0)->setText((CEGUI::utf8*)_("Maps"));

        mMapListDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/MapListDesc"));
        mMapListDesc->setText((CEGUI::utf8*)((CEGUI::utf8*)_("         Map List\nClick once to add to playlist")));
        mMapList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/maps/MapList"));
        mMapList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CH_GUI::onMapListClicked, this));
        mMapList->setSortingEnabled(true);

        mMapPlayListDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/PlayMapListDesc"));
        mMapPlayListDesc->setText((CEGUI::utf8*)((CEGUI::utf8*)_("         Play List\n  Click once to remove")));
        mMapPlayList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/maps/PlayMapList"));
        mMapPlayList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CH_GUI::onMapPlayListClicked, this));

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
        mDeathmatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onDeathClick, this));

        mPointmatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/PointButton"));
        mPointmatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onPointClick, this));

        mRambomatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/RamboButton"));
        mRambomatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onRamboClick, this));

        mTeammatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/TeamButton"));
        mTeammatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onTeamClick, this));

        mCTFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/CTFButton"));
        mCTFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onCTFClick, this));

        mHTFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/HTFButton"));
        mHTFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onHTFClick, this));

        mINFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/INFButton"));
        mINFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&CH_GUI::onINFClick, this));

        mStartGameButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/maps/StartGameButton"));
        mStartGameButton->setEnabled(false);
        mStartGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CH_GUI::onLeftClickStart, this));

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
        //mPlayer = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/player"));


        // Options
        mTab->getTabContentsAtIndex(2)->setText((CEGUI::utf8*)_("Options"));
        //mOptions = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/options"));


        /*mLanguage = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/Language"));
        mLanguage->addItem(new MyListItem("English"));
        mLanguage->addItem(new MyListItem("Polish"));
        mLanguage->setSortingEnabled(true);
        mLanguage->setReadOnly(true);
        mLanguage->setItemSelectState(size_t(0), true);*/
        //mPlaceSoldat = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldat"));

        mPlaceSoldatDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldatDesc"));
        mPlaceSoldatDesc->setText((CEGUI::utf8*)_("Directory with Soldat files :"));

        mPlaceSoldat = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldat"));
        mPlaceSoldat->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&CH_GUI::onSearchSoldat, this));

        mInterfacesDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/interfacesDesc"));
        mInterfacesDesc->setText((CEGUI::utf8*)_("Interface type :"));

        mInterfaces = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/InterfacesBox"));

        mStatusField = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/StatusField"));

        // Graphics
        mTab->getTabContentsAtIndex(3)->setText((CEGUI::utf8*)_("Graphics"));
        //mGraphics = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/graphics"));
        //mGraphics->setText((CEGUI::utf8*)_("Graphics"));

        mIsFullscreen = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/graphics/IsFullscreen"));
        mIsFullscreen->setText((CEGUI::utf8*)_("Fullscreen"));
        mIsFullscreen->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onGraphicsChanged, this));
        mDeep = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/graphics/Deep"));
        mDeep->setText((CEGUI::utf8*)_("Deep"));
        mDeep16 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Deep16"));
        mDeep16->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onGraphicsChanged, this));
        mDeep32 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Deep32"));
        mDeep32->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onGraphicsChanged, this));
        mResol = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/graphics/Resol"));
        mResol->setText((CEGUI::utf8*)_("Resolution"));
        mResol640 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol640"));
        mResol640->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onGraphicsChanged, this));
        mResol800 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol800"));
        mResol800->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onGraphicsChanged, this));
        mResol1024 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol1024"));
        mResol1024->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onGraphicsChanged, this));
        //mInfoRestart = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/graphics/InfoRestart"));
        //mInfoRestart->setText((CEGUI::utf8*)_("Restart to apply changes"));


        // Music
        mTab->getTabContentsAtIndex(4)->setText((CEGUI::utf8*)_("Music"));

        mIsSounds = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/music/IsSounds"));
        mIsSounds->setText((CEGUI::utf8*)_("Sounds"));
        mIsSounds->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onSoundBoxChanged, this));
        mSoundsDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/SoundsDesc"));
        mSoundsDesc->setText((CEGUI::utf8*)_("Sounds Volume (%)"));
        mSoundsSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/music/SoundsSpinner"));
        mSoundsSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&CH_GUI::onSoundSpinnerChanged, this));

        mIsMusic = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/music/IsMusic"));
        mIsMusic->setText((CEGUI::utf8*)_("Music"));
        mIsMusic->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onMusicBoxChanged, this));
        mMusicDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/MusicDesc"));
        mMusicDesc->setText((CEGUI::utf8*)_("Music Volume (%)"));
        mMusicSpinner = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/music/MusicSpinner"));
        mMusicSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&CH_GUI::onMusicSpinnerChanged, this));

        mMusicSongDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/MusicSongDesc"));
        //mMusicSongDesc->setText((CEGUI::utf8*)_("In-game music. First selected will be played"));
        //mMusicList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/music/MusicList"));
        //mMusicList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&CH_GUI::onMusicListChanged, this));

        mGroupQuality = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/music/GroupQuality"));
        mGroupQuality->setText((CEGUI::utf8*)_("Sounds Quality"));
        mLowQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/LowQuality"));
        mLowQuality->setText((CEGUI::utf8*)_("Low"));
        mLowQuality->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onAudioQualityChanged, this));
        mMediumQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/MediumQuality"));
        mMediumQuality->setText((CEGUI::utf8*)_("Medium"));
        mMediumQuality->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onAudioQualityChanged, this));
        mHighQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/HighQuality"));
        mHighQuality->setText((CEGUI::utf8*)_("High"));
        mHighQuality->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CH_GUI::onAudioQualityChanged, this));


        // Exit
        mTab->getTabContentsAtIndex(5)->setText((CEGUI::utf8*)_("Exit"));

        mExitButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/exit/ExitButton"));
        mExitButton->setText((CEGUI::utf8*)_("Save & Exit"));
        mExitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CH_GUI::onLeftClickExit, this));

    }
    catch (CEGUI::Exception &e)
    {
        std::cerr << "Error initializing the demo:" << e.getMessage().c_str() << "\n";
    }

    apply_configs();


    CURRENT_SONG_NUMBER = 0;

    CEGUI::EventArgs ev;
    mCTFButton->setSelected(true);
    onCTFClick(ev);

    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

}
