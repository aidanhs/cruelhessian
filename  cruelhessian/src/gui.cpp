/*   gui.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84@gmail.com>
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
#include <sstream>
#include <fstream>
#include <locale.h>
#include <libintl.h>
#include "SDL.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/regex.hpp"
#include "gui.h"
#include "globals.h"
#include "worldmap.h"

#define _(string) gettext(string)


bool must_quit = false;
char song[30];
char solpath[100];

CEGUI::Window *myRoot;
CEGUI::OpenGLRenderer *renderer;
CEGUI::Combobox *mSchemes;

CEGUI::TabControl *mTab;

// maps & exit
CEGUI::GroupBox *mMapType;
CEGUI::PushButton* mStartGameButton;
CEGUI::Listbox *mMapList;
//CEGUI::PushButton* mDolaczGameButton;
//CEGUI::Editbox* mIPNumberBox;
//CEGUI::Editbox* mPortNumberBox;
CEGUI::PushButton *mExitButton;
CEGUI::MultiLineEditbox *mDescription;
CEGUI::Editbox *mDesc1;
CEGUI::Editbox *mDesc2;
CEGUI::Spinner *mSpinn1;
CEGUI::Spinner *mSpinn2;
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
CEGUI::Editbox* mInfoRestart;

// music
CEGUI::GroupBox* mGroupQuality;
CEGUI::Editbox* mSoundsDesc;
CEGUI::Editbox* mMusicDesc;
CEGUI::Editbox* mMusicSongDesc;
CEGUI::Checkbox* mIsSounds;
CEGUI::Checkbox* mIsMusic;
CEGUI::Scrollbar* mSoundsSlider;
CEGUI::Scrollbar* mMusicSlider;
CEGUI::Listbox* mMusicList;
CEGUI::RadioButton* mLowQuality;
CEGUI::RadioButton* mMediumQuality;
CEGUI::RadioButton* mHighQuality;


// options
//CEGUI::Combobox* mLanguage;
CEGUI::Editbox *mPlaceSoldat;
CEGUI::Editbox *mStatusField;
//CEGUI::PushButton *mPlaceSoldatButton;


//CEGUI::MultiColumnList* mServersList;


class MyListItem : public CEGUI::ListboxTextItem
{
public:
    MyListItem (const CEGUI::String& text) : CEGUI::ListboxTextItem(text)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};

void showMaps(string mask)
{
    boost::regex re(SOL_PATH+"Maps/"+mask+".+.PMS");
    boost::filesystem::directory_iterator end;
    std::string str;

    if ( !boost::filesystem::exists(SOL_PATH+"Maps/") )
    {
        cout << "'Maps' directory doesn't exist !";
        exit(EXIT_FAILURE);
    }

    mMapList->resetList();

    for (boost::filesystem::directory_iterator iter(SOL_PATH+"Maps/"); iter != end; ++iter)
    {
        if (boost::regex_match(iter->path().string(), re))
        {
            str.assign(iter->path().string().begin()+SOL_PATH.length()+5, iter->path().string().end()-4);
            mMapList->addItem(new MyListItem(str));
        }
    }
    mMapList->setItemSelectState(size_t(0), true);
}

bool showMusic()
{
    //boost::regex re(SOL_PATH+"Mp3/.+.[(mp3)(ogg)(wav)]");
    boost::regex re(SOL_PATH+"Mp3/.+.[(ogg)(wav)]");
    boost::filesystem::directory_iterator end;
    std::string str;

    for (boost::filesystem::directory_iterator iter(SOL_PATH+"Mp3/"); iter != end; ++iter)
    {
        if (boost::regex_match(iter->path().string(), re))
        {
            str.assign(iter->path().string().begin()+SOL_PATH.length()+4, iter->path().string().end());
            mMusicList->addItem(new MyListItem(str));
        }
    }
    //mMusicList->setItemSelectState(size_t(0), true);

    return true;
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


void inject_input (bool & must_quit)
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

void setStateMus(bool state)
{
    mLowQuality->setEnabled(state);
    mMediumQuality->setEnabled(state);
    mHighQuality->setEnabled(state);
    mGroupQuality->setEnabled(state);
}


bool onSoundBox(const CEGUI::EventArgs& pEventArgs)
{
    if (mIsSounds->isSelected() == false)
    {
        mSoundsSlider->setEnabled(false);
        mSoundsDesc->setEnabled(false);
        if (mIsMusic->isSelected() == false) setStateMus(false);
    }
    else
    {
        mSoundsSlider->setEnabled(true);
        mSoundsDesc->setEnabled(true);
        setStateMus(true);
    }
    return true;
}


bool onMusicBox(const CEGUI::EventArgs& pEventArgs)
{
    if (mIsMusic->isSelected() == false)
    {
        mMusicSlider->setEnabled(false);
        mMusicDesc->setEnabled(false);
        mMusicSongDesc->setEnabled(false);
        mMusicList->resetList();
        mMusicList->setEnabled(false);
        if (mIsSounds->isSelected() == false) setStateMus(false);
    }
    else
    {
        mMusicSlider->setEnabled(true);
        mMusicDesc->setEnabled(true);
        mMusicSongDesc->setEnabled(true);
        mMusicList->setEnabled(true);
        showMusic();
        setStateMus(true);
    }
    return true;
}


void main_loop ()
{

    double last_time_pulse = 0.001*static_cast<double>(SDL_GetTicks());

    while (!must_quit)
    {
        inject_input(must_quit);
        inject_time_pulse(last_time_pulse);
        glClear(GL_COLOR_BUFFER_BIT);
        CEGUI::System::getSingleton().renderGUI();
        SDL_GL_SwapBuffers();
        while (GAME_LOOP) world->game_loop();
    }
}


bool onLeftClickStart(const CEGUI::EventArgs& pEventArgs)
{
    if (mMapList->getItemCount() > 0)
    {
        std::string str = mMapList->getFirstSelectedItem()->getText().c_str();
        std::string nnn = SOL_PATH+"Maps/"+str+".PMS";
        MAP = nnn.c_str();
        init_all2();
        start_server();
        GAME_LOOP = true;
        world->CHOICE_ESC = false;
    }
    return true;
}

//const char *host;
//int port;
/*
bool onLeftClickDolacz(const CEGUI::EventArgs& pEventArgs)
{

    string str = mMapList->getFirstSelectedItem()->getText().c_str();
    string nnn = "Maps/"+str+".PMS";
    MAP = nnn.c_str();
    host = mIPNumberBox->getText().c_str();
    stringstream oss(mPortNumberBox->getText().c_str());
    oss >> port;
    init_all2();
    start_client();
    GAME_LOOP = true;
    world->CHOICE_ESC = false;

    return true;
}
*/
bool onLeftClickExit(const CEGUI::EventArgs& pEventArgs)
{
    must_quit = true;
    save_configs();
    return true;
}

bool onDeathClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("ctf_");
    mDescription->setText((CEGUI::utf8*)(_("Kill everything that moves.")));
    mDesc1->setText((CEGUI::utf8*)(_("Kill limit")));
    mSpinn1->setCurrentValue(30);
    return true;
}

bool onPointClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("ctf_");
    mDescription->setText((CEGUI::utf8*)(_("Get 1 point for kill. If you carry the Yellow Flag you get 2 points for kill. Also you get multipoints for multikills.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(30);
    return true;
}

bool onTeamClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("ctf_");
    mDescription->setText((CEGUI::utf8*)(_("Up to 4 teams fight against each other.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(60);
    return true;
}

bool onRamboClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("ctf_");
    mDescription->setText((CEGUI::utf8*)(_("'First Blood' style. Whoever owns the Rambo Bow gains super Rambo powers. Only Rambo gets points for kill.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(30);
    return true;
}

bool onCTFClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("ctf_");
    mDescription->setText((CEGUI::utf8*)(_("Capture the enemy flag and bring it to your base to score. 20 points for capture.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(5);
    return true;
}

bool onHTFClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("htf_");
    mDescription->setText((CEGUI::utf8*)(_("Get the yellow flag and hold it with your team for as long as possible. The team earns 1 point every 5 seconds of holding.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(80);
    return true;
}

bool onINFClick(const CEGUI::EventArgs& pEventArgs)
{
    showMaps("inf_");
    mDescription->setText((CEGUI::utf8*)(_("Red team gets 30 points for retreiving the black flag. Blue team gets 1 point every 5 seconds if the flag is in base.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(90);
    return true;
}

bool find_file(const boost::filesystem::path &dir_path,
               const std::string &file_name,
               boost::filesystem::path &path_found)
{
    //mStatusField->setText((CEGUI::utf8*)_("Searching ..."));
    if ( !exists( dir_path ) ) return false;
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for ( boost::filesystem::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
    {
        /*if ( is_directory(itr->status()) )
        {
            mPlaceSoldat->setText(path_found.string());
            if ( find_file( itr->path(), file_name, path_found ) ) return true;
        }
        else*/
        if ( itr->leaf() == file_name ) // see below
        {
            //path_found = itr->path();
            path_found = dir_path;
            return true;
        }
    }
    return false;
}

bool checkSoldat()
{
    boost::filesystem::path path_found;

    if (find_file(mPlaceSoldat->getText().c_str(), "Soldat.exe", path_found) == true)
    {
        SOL_PATH = path_found.string();
        if (SOL_PATH[SOL_PATH.length()-1] != '/')
        {
            SOL_PATH += '/';
        }
        mStatusField->setText((CEGUI::utf8*)_("Found !"));
        return true;
    }
    else
    {
        mStatusField->setText((CEGUI::utf8*)_("Not found !"));
    }
    return false;
}


bool onSearchSoldat(const CEGUI::EventArgs& pEventArgs)
{
    checkSoldat();
    return true;
}


int main()
{
    setlocale(LC_ALL, "");
    bindtextdomain("pl", "locale");
    textdomain("pl");

    read_configs();

    init_all();

    renderer = new CEGUI::OpenGLRenderer(0, MAX_WIDTH, MAX_HEIGHT);
    new CEGUI::System(renderer);

    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

    CEGUI::DefaultResourceProvider *rp = reinterpret_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    //rp->setResourceGroupDirectory("schemes", "../data/cegui-folder/schemes/");
    rp->setResourceGroupDirectory("fonts", "../data/fonts/");
    rp->setResourceGroupDirectory("imagesets", "../data/imagesets/");
    rp->setResourceGroupDirectory("looknfeel", "../data/looknfeel/");
    /*rp->setResourceGroupDirectory("schemes", "../data/cegui-folder/");
    rp->setResourceGroupDirectory("fonts", "../data/cegui-folder/");
    rp->setResourceGroupDirectory("imagesets", "c../data/egui-folder/");
    rp->setResourceGroupDirectory("looknfeel", "../data/cegui-folder/");
    */
    //CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");

    //CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font");
    //CEGUI::FontManager::getSingleton().createFont("DejaVuSans-10.font");
    //CEGUI::FontManager::getSingleton().createFont("Bgothl-10.font");
    //CEGUI::FontManager::getSingleton().createFont("Lucon-10.font");
    //CEGUI::FontManager::getSingleton().createFont("Times.font");


    try
    {
        // Retrieve the window manager
        CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

        // Load the TaharezLook scheme and set up the default mouse cursor and font
        //CEGUI::Scheme* currentScheme = CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
        CEGUI::SchemeManager::getSingleton().loadScheme("../data/TaharezLook.scheme");
        //CEGUI::ImagesetManager::getSingleton().createImageset("cegui-folder/TaharezLook.imageset");
        //CEGUI::Scheme* currentScheme = CEGUI::SchemeManager::getSingleton().loadScheme("SleekSpace.scheme");
        //CEGUI::Scheme* currentScheme = CEGUI::SchemeManager::getSingleton().loadScheme("WindowsLook.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

        if (! CEGUI::FontManager::getSingleton().isFontPresent( "DejaVuSans-10" ) )
            CEGUI::FontManager::getSingleton().createFont( "DejaVuSans-10.font" );
        // CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );


        // Set the GUI Sheet
        CEGUI::Window* sheet = winMgr.createWindow("DefaultWindow", "root_wnd");
        CEGUI::System::getSingleton().setGUISheet(sheet);

        // Load a layout
        //CEGUI::Window* guiLayout = winMgr.loadWindowLayout("PropertyFinder.layout");
        //CEGUI::Window* guiLayout = winMgr.loadWindowLayout("cegui-folder/layouts/hessian.layout");
        CEGUI::Window* guiLayout = winMgr.loadWindowLayout("../data/chessian.layout");
        //CEGUI::Window* guiLayout = winMgr.loadWindowLayout("cegui-folder/layouts/FontDemo.layout");
        sheet->addChildWindow(guiLayout);

        //CEGUI::Imageset* set = CEGUI::ImagesetManager::getSingleton().createImagesetFromImageFile("IconNO","icon_no.gif");

        mTab = static_cast<CEGUI::TabControl*>(winMgr.getWindow("root/1"));

        // Maps
        mTab->getTabContentsAtIndex(0)->setText((CEGUI::utf8*)_("Maps"));

        //mMap = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/maps"));
        //mMap->setText((CEGUI::utf8*)_("Maps"));

        mMapList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/maps/MapList"));
        mMapList->setSortingEnabled(true);

        mSpinn1 = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/Spinn1"));
        mSpinn2 = static_cast<CEGUI::Spinner*>(winMgr.getWindow("root/maps/Spinn2"));

        mDescription = static_cast<CEGUI::MultiLineEditbox*>(winMgr.getWindow("root/maps/Description"));

        mDesc1 = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/Desc1"));
        mDesc2 = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/maps/Desc2"));
        mDesc2->setText((CEGUI::utf8*)((CEGUI::utf8*)_("Time limit")));

        mMapType = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/maps/Type"));
        mMapType->setText((CEGUI::utf8*)_("Type"));

        mDeathmatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/DeathButton"));
        //mDeathmatch->setSelected(true);
        mDeathmatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onDeathClick));

        mPointmatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/PointButton"));
        mPointmatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onPointClick));

        mRambomatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/RamboButton"));
        mRambomatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onRamboClick));

        mTeammatch = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/TeamButton"));
        mTeammatch->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onTeamClick));

        mCTFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/CTFButton"));
        mCTFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onCTFClick));

        mHTFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/HTFButton"));
        mHTFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onHTFClick));

        mINFButton = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/maps/INFButton"));
        mINFButton->subscribeEvent(CEGUI::RadioButton::EventActivated, CEGUI::Event::Subscriber(&onINFClick));

        mStartGameButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/maps/StartGameButton"));
        mStartGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&onLeftClickStart));


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
        //mPlayer->setText((CEGUI::utf8*)_("Player"));


        // Options
        mTab->getTabContentsAtIndex(2)->setText((CEGUI::utf8*)_("Options"));
        //mOptions = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/options"));
        //mOptions->setText((CEGUI::utf8*)_("Options"));

        /*mLanguage = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/Language"));
        mLanguage->addItem(new MyListItem("English"));
        mLanguage->addItem(new MyListItem("Polish"));
        mLanguage->setSortingEnabled(true);
        mLanguage->setReadOnly(true);
        mLanguage->setItemSelectState(size_t(0), true);*/
        mPlaceSoldat = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeSoldat"));
        mPlaceSoldat->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&onSearchSoldat));

        /*mPlaceSoldatButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/placeSoldatButton"));
        mPlaceSoldatButton->setText((CEGUI::utf8*)_("Find in this directory"));
        mPlaceSoldatButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&onSearchSoldat));
        */
        mStatusField = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/StatusField"));


        // Graphics
        mTab->getTabContentsAtIndex(3)->setText((CEGUI::utf8*)_("Graphics"));
        //mGraphics = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/graphics"));
        //mGraphics->setText((CEGUI::utf8*)_("Graphics"));

        mIsFullscreen = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/graphics/IsFullscreen"));
        mIsFullscreen->setText((CEGUI::utf8*)_("Fullscreen"));
        mDeep = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/graphics/Deep"));
        mDeep->setText((CEGUI::utf8*)_("Deep"));
        mDeep16 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Deep16"));
        mDeep32 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Deep32"));
        mResol = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/graphics/Resol"));
        mResol->setText((CEGUI::utf8*)_("Resolution"));
        mResol640 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol640"));
        mResol800 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol800"));
        mResol1024 = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/graphics/Resol1024"));
        mInfoRestart = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/graphics/InfoRestart"));
        mInfoRestart->setText((CEGUI::utf8*)_("Restart to apply changes"));


        // Music
        mTab->getTabContentsAtIndex(4)->setText((CEGUI::utf8*)_("Music"));

        mIsSounds = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/music/IsSounds"));
        mIsSounds->setText((CEGUI::utf8*)_("Sounds"));
        mIsSounds->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&onSoundBox));
        mSoundsDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/SoundsDesc"));
        mSoundsDesc->setText((CEGUI::utf8*)_("Sounds Volume (%)"));
        mSoundsSlider = static_cast<CEGUI::Scrollbar*>(winMgr.getWindow("root/music/SoundsSlider"));

        mIsMusic = static_cast<CEGUI::Checkbox*>(winMgr.getWindow("root/music/IsMusic"));
        mIsMusic->setText((CEGUI::utf8*)_("Music"));
        mIsMusic->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&onMusicBox));
        mMusicDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/MusicDesc"));
        mMusicDesc->setText((CEGUI::utf8*)_("Music Volume (%)"));
        mMusicSlider = static_cast<CEGUI::Scrollbar*>(winMgr.getWindow("root/music/MusicSlider"));

        mMusicSongDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/music/MusicSongDesc"));
        mMusicSongDesc->setText((CEGUI::utf8*)_("In-game music"));
        mMusicList = static_cast<CEGUI::Listbox*>(winMgr.getWindow("root/music/MusicList"));

        mGroupQuality = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/music/GroupQuality"));
        mGroupQuality->setText((CEGUI::utf8*)_("Sounds Quality"));
        mLowQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/LowQuality"));
        mLowQuality->setText((CEGUI::utf8*)_("Low"));
        mMediumQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/MediumQuality"));
        mMediumQuality->setText((CEGUI::utf8*)_("Medium"));
        mHighQuality = static_cast<CEGUI::RadioButton*>(winMgr.getWindow("root/music/HighQuality"));
        mHighQuality->setText((CEGUI::utf8*)_("High"));


        // Exit
        mTab->getTabContentsAtIndex(5)->setText((CEGUI::utf8*)_("Exit"));
        //mExit = static_cast<CEGUI::FrameWindow*>(winMgr.getWindow("root/exit"));
        //mExit->setText((CEGUI::utf8*)_("Exit"));

        mExitButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/exit/ExitButton"));
        mExitButton->setText((CEGUI::utf8*)_("Save & Exit"));
        mExitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&onLeftClickExit));

    }
    catch (CEGUI::Exception &e)
    {
        std::cerr << "Error initializing the demo:" << e.getMessage().c_str() << "\n";
    }

    apply_configs();

    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    main_loop();

    return 0;
}

// music sucks !!!!!
// fix !!!!!!!!!!!!!

int read_configs()
{
    char temp[30];
    boost::filesystem::path path_found;
    FILE *opt_file = fopen("options.ini", "rt");

    if (opt_file == NULL)
    {
        perror("Cannot open options.ini file");
        return 1;
    }

    fscanf(opt_file, "%s %s", temp, solpath);
    if (strcmp(temp, "SoldatPath") != 0)
    {
        cout << "BLAD 1";
        exit(1);
    }
    SOL_PATH.assign(solpath);

  /*  if (find_file(SOL_PATH, "Soldat.exe", path_found) == false)
    {
        cout << "GGGGGGGGGGGG";
        exit(1);
    }
*/
    // mode (window, fullscreen)
    fscanf(opt_file, "%s %d", temp, &FULLSCREEN);
    if (strcmp(temp, "Fullscreen") != 0)
    {
        cout << "BLAD 2";
        exit(1);
    }

    // resolution
    fscanf(opt_file, "%s %d %d", temp, &MAX_WIDTH, &MAX_HEIGHT);
    if (strcmp(temp, "Resolution") != 0)
    {
        cout << "BLAD 3";
        exit(1);
    }

    // deep
    fscanf(opt_file, "%s %d", temp, &MAX_BPP);
    if (strcmp(temp, "Deep") != 0)
    {
        cout << "BLAD 4";
        exit(1);
    }

    // sounds
    fscanf(opt_file, "%s %f", temp, &SOUNDS_VOL);

    // music
    fscanf(opt_file, "%s %f", temp, &MUSIC_VOL);

    if (MUSIC_VOL > 0 && MUSIC_VOL <= 1)
    {
        fscanf(opt_file, "%s %s", temp, song);
        SONG_NAME.assign(song);
    }

    if (SOUNDS_VOL+MUSIC_VOL > 0)
        fscanf(opt_file, "%s %d", temp, &AUDIO_QUAL);

    fclose(opt_file);

    return 0;
}



// apply in interface
int apply_configs()
{

    mPlaceSoldat->setText(SOL_PATH);
    if (checkSoldat() == false)
    {
        mTab->setSelectedTabAtIndex(2);
        //cout << "WQQQQQQQQQQQ";
        //mOptions->setVisible(true);
        //mOptions->setTitleBarEnabled(true);
        //mOptions->setFrameEnabled(true);
    }

    // tryb (okno, pelny ekran)
    if (FULLSCREEN == 0)
        mIsFullscreen->setSelected(false);
    else if (FULLSCREEN == 1)
        mIsFullscreen->setSelected(true);
    //else
    //  xerror(++line_nr);

    // resolution
    if (MAX_WIDTH == 640 && MAX_HEIGHT == 480)
        mResol640->setSelected(true);
    else if (MAX_WIDTH == 800 && MAX_HEIGHT == 600)
        mResol800->setSelected(true);
    else if (MAX_WIDTH == 1024 && MAX_HEIGHT == 768)
        mResol1024->setSelected(true);
    //else
    //  xerror(++line_nr);

    // deep
    if (MAX_BPP == 16)
        mDeep16->setSelected(true);
    else if (MAX_BPP == 32)
        mDeep32->setSelected(true);
    //else
    //  xerror(++line_nr);

    // sounds
    if (SOUNDS_VOL == 0)
    {
        mIsSounds->setSelected(false);
        mSoundsSlider->setEnabled(false);
    }
    else if (SOUNDS_VOL > 0 && SOUNDS_VOL <= 1)
    {
        mIsSounds->setSelected(true);
        mSoundsSlider->setEnabled(true);
        mSoundsSlider->setScrollPosition(SOUNDS_VOL);
    }
    //else xerror(++line_nr);

    // music
    if (MUSIC_VOL == 0)
    {
        mIsMusic->setSelected(false);
        //dialog->m_staticText121->Enable(false);
        mMusicSlider->setEnabled(false);
        //dialog->m_staticText17->Enable(false);
        mMusicList->setEnabled(false);
    }
    else if (MUSIC_VOL > 0 && MUSIC_VOL <= 1)
    {
        mIsMusic->setSelected(true);
        mMusicSlider->setEnabled(true);
        mMusicSlider->setScrollPosition(MUSIC_VOL);
        mMusicList->setEnabled(true);

    }

    if (SOUNDS_VOL+MUSIC_VOL > 0)
    {
        if (AUDIO_QUAL == 11025)
            mLowQuality->setSelected(true);
        else if (AUDIO_QUAL == 22050)
            mMediumQuality->setSelected(true);
        else if (AUDIO_QUAL == 44100)
            mHighQuality->setSelected(true);
    } else
    {
        setStateMus(false);
    }
    //else xerror(++line_nr);

    return 0;

}

int save_configs()
{
    std::ofstream plik("options.ini");

    plik << "SoldatPath " << SOL_PATH << std::endl;

    // Graphics
    if (mIsFullscreen->isSelected()) plik << "Fullscreen 1\n";
    else plik << "Fullscreen 0\n";

    if (mResol640->isSelected()) plik << "Resolution 640 480\n";
    else if (mResol800->isSelected()) plik << "Resolution 800 600\n";
    else if (mResol1024->isSelected()) plik << "Resolution 1024 768\n";

    if (mDeep16->isSelected()) plik << "Deep 16\n";
    else if (mDeep32->isSelected()) plik << "Deep 32\n";

    // Music
    if (!mIsSounds->isSelected())
        plik << "SoundVolume 0\n";
    else
        plik << "SoundVolume " << mSoundsSlider->getScrollPosition() << std::endl;

    if (!mIsMusic->isSelected())
        plik << "MusicVolume 0\n";
    else
    {
        plik << "MusicVolume " << mMusicSlider->getScrollPosition() << std::endl;
        plik << "MusicFilename " << mMusicList->getFirstSelectedItem()->getText().c_str() << std::endl;
    }

    if (SOUNDS_VOL+MUSIC_VOL > 0)
    {
        if (mLowQuality->isSelected())
            plik << "AudioQuality " << 11025 << std::endl;
        else if (mMediumQuality->isSelected())
            plik << "AudioQuality " << 22050 << std::endl;
        else if (mHighQuality->isSelected())
            plik << "AudioQuality " << 44100 << std::endl;
    }

    plik.close();

    return 0;
}


