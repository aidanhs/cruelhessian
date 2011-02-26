/*   GUI.cpp
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

#include <iostream>

#include "GUI.h"
#include "CEGUIDefaultResourceProvider.h"
//#include "CEGUILua.h"

#include "../Game.h"
#include "../ParserManager.h"




bool GUI::handleEvent(sf::Event& Event)
{
    switch (Event.Type)
    {
    case sf::Event::TextEntered:
        return mSystem->injectChar(Event.Text.Unicode);
    case sf::Event::KeyPressed:
        return mSystem->injectKeyDown(toCEGUIKey(Event.Key.Code));
    case sf::Event::KeyReleased:
        return mSystem->injectKeyUp(toCEGUIKey(Event.Key.Code));
    case sf::Event::MouseMoved:
        return mSystem->injectMousePosition(static_cast<float>(mInput->GetMouseX()), static_cast<float>(mInput->GetMouseY()));
    case sf::Event::MouseButtonPressed:
        return mSystem->injectMouseButtonDown(toCEGUIMouseButton(Event.MouseButton.Button));
    case sf::Event::MouseButtonReleased:
        return mSystem->injectMouseButtonUp(toCEGUIMouseButton(Event.MouseButton.Button));
    case sf::Event::MouseWheelMoved:
        return mSystem->injectMouseWheelChange(static_cast<float>(Event.MouseWheel.Delta));
    }
    return false;
}


CEGUI::Key::Scan GUI::toCEGUIKey(sf::Key::Code Code)
{
    if (mKeyMap.find(Code) == mKeyMap.end())
        return (CEGUI::Key::Scan)0;

    return mKeyMap[Code];
}

CEGUI::MouseButton GUI::toCEGUIMouseButton(sf::Mouse::Button Button)
{
    if (mMouseButtonMap.find(Button) == mMouseButtonMap.end())
        return (CEGUI::MouseButton)0;

    return mMouseButtonMap[Button];
}

//Auto-generated (phew...), edited by hand
void GUI::initializeMaps()
{
    mKeyMap[sf::Key::Escape]          = CEGUI::Key::Escape       ;
    mKeyMap[sf::Key::Num1]            = CEGUI::Key::One          ;
    mKeyMap[sf::Key::Num2]            = CEGUI::Key::Two          ;
    mKeyMap[sf::Key::Num3]            = CEGUI::Key::Three        ;
    mKeyMap[sf::Key::Num4]            = CEGUI::Key::Four         ;
    mKeyMap[sf::Key::Num5]            = CEGUI::Key::Five         ;
    mKeyMap[sf::Key::Num6]            = CEGUI::Key::Six          ;
    mKeyMap[sf::Key::Num7]            = CEGUI::Key::Seven        ;
    mKeyMap[sf::Key::Num8]            = CEGUI::Key::Eight        ;
    mKeyMap[sf::Key::Num9]            = CEGUI::Key::Nine         ;
    mKeyMap[sf::Key::Num0]            = CEGUI::Key::Zero         ;
    mKeyMap[sf::Key::Dash]            = CEGUI::Key::Minus        ;
    mKeyMap[sf::Key::Equal]           = CEGUI::Key::Equals       ;
    mKeyMap[sf::Key::Back]            = CEGUI::Key::Backspace    ;
    mKeyMap[sf::Key::Tab]             = CEGUI::Key::Tab          ;
    mKeyMap[sf::Key::Q]               = CEGUI::Key::Q            ;
    mKeyMap[sf::Key::W]               = CEGUI::Key::W            ;
    mKeyMap[sf::Key::E]               = CEGUI::Key::E            ;
    mKeyMap[sf::Key::R]               = CEGUI::Key::R            ;
    mKeyMap[sf::Key::T]               = CEGUI::Key::T            ;
    mKeyMap[sf::Key::Y]               = CEGUI::Key::Y            ;
    mKeyMap[sf::Key::U]               = CEGUI::Key::U            ;
    mKeyMap[sf::Key::I]               = CEGUI::Key::I            ;
    mKeyMap[sf::Key::O]               = CEGUI::Key::O            ;
    mKeyMap[sf::Key::P]               = CEGUI::Key::P            ;
    mKeyMap[sf::Key::LBracket]        = CEGUI::Key::LeftBracket  ;
    mKeyMap[sf::Key::RBracket]        = CEGUI::Key::RightBracket ;
    mKeyMap[sf::Key::Return]          = CEGUI::Key::Return       ;
    mKeyMap[sf::Key::LControl]        = CEGUI::Key::LeftControl  ;
    mKeyMap[sf::Key::A]               = CEGUI::Key::A            ;
    mKeyMap[sf::Key::S]               = CEGUI::Key::S            ;
    mKeyMap[sf::Key::D]               = CEGUI::Key::D            ;
    mKeyMap[sf::Key::F]               = CEGUI::Key::F            ;
    mKeyMap[sf::Key::G]               = CEGUI::Key::G            ;
    mKeyMap[sf::Key::H]               = CEGUI::Key::H            ;
    mKeyMap[sf::Key::J]               = CEGUI::Key::J            ;
    mKeyMap[sf::Key::K]               = CEGUI::Key::K            ;
    mKeyMap[sf::Key::L]               = CEGUI::Key::L            ;
    mKeyMap[sf::Key::SemiColon]       = CEGUI::Key::Semicolon    ;
    mKeyMap[sf::Key::LShift]          = CEGUI::Key::LeftShift    ;
    mKeyMap[sf::Key::BackSlash]       = CEGUI::Key::Backslash    ;
    mKeyMap[sf::Key::Z]               = CEGUI::Key::Z            ;
    mKeyMap[sf::Key::X]               = CEGUI::Key::X            ;
    mKeyMap[sf::Key::C]               = CEGUI::Key::C            ;
    mKeyMap[sf::Key::V]               = CEGUI::Key::V            ;
    mKeyMap[sf::Key::B]               = CEGUI::Key::B            ;
    mKeyMap[sf::Key::N]               = CEGUI::Key::N            ;
    mKeyMap[sf::Key::M]               = CEGUI::Key::M            ;
    mKeyMap[sf::Key::Comma]           = CEGUI::Key::Comma        ;
    mKeyMap[sf::Key::Period]          = CEGUI::Key::Period       ;
    mKeyMap[sf::Key::Slash]           = CEGUI::Key::Slash        ;
    mKeyMap[sf::Key::RShift]          = CEGUI::Key::RightShift   ;
    mKeyMap[sf::Key::Multiply]        = CEGUI::Key::Multiply     ;
    mKeyMap[sf::Key::LAlt]            = CEGUI::Key::LeftAlt      ;
    mKeyMap[sf::Key::Space]           = CEGUI::Key::Space        ;
    mKeyMap[sf::Key::F1]              = CEGUI::Key::F1           ;
    mKeyMap[sf::Key::F2]              = CEGUI::Key::F2           ;
    mKeyMap[sf::Key::F3]              = CEGUI::Key::F3           ;
    mKeyMap[sf::Key::F4]              = CEGUI::Key::F4           ;
    mKeyMap[sf::Key::F5]              = CEGUI::Key::F5           ;
    mKeyMap[sf::Key::F6]              = CEGUI::Key::F6           ;
    mKeyMap[sf::Key::F7]              = CEGUI::Key::F7           ;
    mKeyMap[sf::Key::F8]              = CEGUI::Key::F8           ;
    mKeyMap[sf::Key::F9]              = CEGUI::Key::F9           ;
    mKeyMap[sf::Key::F10]             = CEGUI::Key::F10          ;
    mKeyMap[sf::Key::Numpad7]         = CEGUI::Key::Numpad7      ;
    mKeyMap[sf::Key::Numpad8]         = CEGUI::Key::Numpad8      ;
    mKeyMap[sf::Key::Numpad9]         = CEGUI::Key::Numpad9      ;
    mKeyMap[sf::Key::Subtract]        = CEGUI::Key::Subtract     ;
    mKeyMap[sf::Key::Numpad4]         = CEGUI::Key::Numpad4      ;
    mKeyMap[sf::Key::Numpad5]         = CEGUI::Key::Numpad5      ;
    mKeyMap[sf::Key::Numpad6]         = CEGUI::Key::Numpad6      ;
    mKeyMap[sf::Key::Add]             = CEGUI::Key::Add          ;
    mKeyMap[sf::Key::Numpad1]         = CEGUI::Key::Numpad1      ;
    mKeyMap[sf::Key::Numpad2]         = CEGUI::Key::Numpad2      ;
    mKeyMap[sf::Key::Numpad3]         = CEGUI::Key::Numpad3      ;
    mKeyMap[sf::Key::Numpad0]         = CEGUI::Key::Numpad0      ;
    mKeyMap[sf::Key::F11]             = CEGUI::Key::F11          ;
    mKeyMap[sf::Key::F12]             = CEGUI::Key::F12          ;
    mKeyMap[sf::Key::F13]             = CEGUI::Key::F13          ;
    mKeyMap[sf::Key::F14]             = CEGUI::Key::F14          ;
    mKeyMap[sf::Key::F15]             = CEGUI::Key::F15          ;
    mKeyMap[sf::Key::RControl]        = CEGUI::Key::RightControl ;
    mKeyMap[sf::Key::Divide]          = CEGUI::Key::Divide       ;
    mKeyMap[sf::Key::RAlt]            = CEGUI::Key::RightAlt     ;
    mKeyMap[sf::Key::Pause]           = CEGUI::Key::Pause        ;
    mKeyMap[sf::Key::Home]            = CEGUI::Key::Home         ;
    mKeyMap[sf::Key::Up]              = CEGUI::Key::ArrowUp      ;
    mKeyMap[sf::Key::PageUp]          = CEGUI::Key::PageUp       ;
    mKeyMap[sf::Key::Left]            = CEGUI::Key::ArrowLeft    ;
    mKeyMap[sf::Key::Right]           = CEGUI::Key::ArrowRight   ;
    mKeyMap[sf::Key::End]             = CEGUI::Key::End          ;
    mKeyMap[sf::Key::Down]            = CEGUI::Key::ArrowDown    ;
    mKeyMap[sf::Key::PageDown]        = CEGUI::Key::PageDown     ;
    mKeyMap[sf::Key::Insert]          = CEGUI::Key::Insert       ;
    mKeyMap[sf::Key::Delete]          = CEGUI::Key::Delete       ;

    mMouseButtonMap[sf::Mouse::Left]		= CEGUI::LeftButton;
    mMouseButtonMap[sf::Mouse::Middle]		= CEGUI::MiddleButton;
    mMouseButtonMap[sf::Mouse::Right]		= CEGUI::RightButton;
    mMouseButtonMap[sf::Mouse::XButton1]	= CEGUI::X1Button;
    mMouseButtonMap[sf::Mouse::XButton2]	= CEGUI::X2Button;
}



// when user pressed OK button in Folder Selector
bool GUI::FolderSelectorClick(const CEGUI::EventArgs &)
{

    const unsigned int max_string = 25;
    std::string temp = Parser.GAME_PATH = Parser.RepairPath(fs.getResult());

    if (temp.length() > max_string)
    {
        temp.replace(max_string, temp.length() - max_string, "...");
    }

    //if (Parser.MODE == 0)
    //{
        Parser.SOLDAT_PATH = Parser.GAME_PATH;
        mPlaceSoldat->setText(temp);
        checkSoldat(0);
    //}

    return true;

}


// when pressed Cancel button in Window Updater
bool GUI::WindowUpdaterClick(const CEGUI::EventArgs &)
{
    checkSoldat(1);

    return true;
}


bool GUI::KeySelectorClick(const CEGUI::EventArgs &)
{

    return true;

}


// apply in GUI
int GUI::ApplyConfigs()
{

    WINDOW_NO_SOLDAT = false;
    mPlaceSoldat->setText(Parser.SOLDAT_PATH);

    CEGUI::EventArgs ev;

    // soldat mode
    if (Parser.MODE == 0)
    {
        mSoldatRadio->setSelected(true);
        onSoldatModeClick(ev);
    }
    // cruel hessian mode
    else
    {
        mCHRadio->setSelected(true);
        onCHModeClick(ev);
    }

    mSpinn2->setCurrentValue(static_cast<int>(Parser.LIMIT_TIME/60));
    mRandomBotsSpinner->setCurrentValue(Parser.RANDOM_BOTS);
    mAlphaSpinner->setCurrentValue(Parser.RANDOM_BOTS_1);
    mBravoSpinner->setCurrentValue(Parser.RANDOM_BOTS_2);
    mCharlieSpinner->setCurrentValue(Parser.RANDOM_BOTS_3);
    mDeltaSpinner->setCurrentValue(Parser.RANDOM_BOTS_4);

    mPlayerName->setText(Parser.PLAYER_NAME);

    mIsFullscreen->setSelected(Parser.FULLSCREEN);

    // resolution
    if (Parser.MAX_WIDTH == 640 && Parser.MAX_HEIGHT == 480)
        mResol640->setSelected(true);
    else if (Parser.MAX_WIDTH == 800 && Parser.MAX_HEIGHT == 600)
        mResol800->setSelected(true);
    else if (Parser.MAX_WIDTH == 1024 && Parser.MAX_HEIGHT == 768)
        mResol1024->setSelected(true);


    // deep
    if (Parser.MAX_BPP == 16)
        mDeep16->setSelected(true);
    else if (Parser.MAX_BPP == 32)
        mDeep32->setSelected(true);


    // sounds
    if (Parser.SOUNDS_VOL == 0)
    {
        mIsSounds->setSelected(false);
        mSoundsSpinner->setEnabled(false);
    }
    else if (Parser.SOUNDS_VOL > 0 && Parser.SOUNDS_VOL <= 100)
    {
        mIsSounds->setSelected(true);
        mSoundsSpinner->setEnabled(true);
        mSoundsSpinner->setCurrentValue(Parser.SOUNDS_VOL);
    }

    // music
    if (Parser.MUSIC_VOL == 0)
    {
        mIsMusic->setSelected(false);
        mMusicSpinner->setEnabled(false);
//        mMusicList->setEnabled(false);
    }
    else if (Parser.MUSIC_VOL > 0 && Parser.MUSIC_VOL <= 100)
    {
        mIsMusic->setSelected(true);
        mMusicSpinner->setEnabled(true);
        mMusicSpinner->setCurrentValue(Parser.MUSIC_VOL);
//        mMusicList->setEnabled(true);
//        mMusicList->setItemSelectState((size_t)0, true);
    }

    if (Parser.AUDIO_QUAL == 11025)
    {
        mLowQuality->setSelected(true);
    }
    else if (Parser.AUDIO_QUAL == 22050)
    {
        mMediumQuality->setSelected(true);
    }
    else if (Parser.AUDIO_QUAL == 44100)
    {
        mHighQuality->setSelected(true);
    }

    if (Parser.SOUNDS_VOL + Parser.MUSIC_VOL == 0)
    {
        setMusicStates(false);
    }
    /*
    MyListItem * my = new MyListItem("test");
    CEGUI::colour col;
    col.setRGB(1,1,1);
    CEGUI::ColourRect rect(0, 0, 5 ,5 );
    rect.setColours(col);
    CEGUI::Vector3 vec(0, 0, 0);*/
//std::string la = "QWE";
//my->draw(vec, 0.5f, rect);
//my->setUserData(&la);
    //      mHairCombo->addItem(my);
    // show colors in Player tab
    for (unsigned int i = 0; i < game.CL_COLOR.size(); ++i)
    {
        mHairCombo->addItem(new MyListItem(game.CL_COLOR[i].name));
        mPantsCombo->addItem(new MyListItem(game.CL_COLOR[i].name));
        mFlameCombo->addItem(new MyListItem(game.CL_COLOR[i].name));
        mShirtCombo->addItem(new MyListItem(game.CL_COLOR[i].name));
    }
    mHairCombo->addItem(new MyListItem("Custom"));
    mPantsCombo->addItem(new MyListItem("Custom"));
    mFlameCombo->addItem(new MyListItem("Custom"));
    mShirtCombo->addItem(new MyListItem("Custom"));

    // select color in interface
    CEGUI::ListboxItem *temp = NULL;

    // hair
    mHairCombo->findItemWithText(Parser.COLOR_HAIR_NAME, NULL);
    if (temp)
    {
        mHairCombo->setItemSelectState(temp, true);
    }
    else
    {
        mHairCombo->addItem(new MyListItem(Parser.COLOR_HAIR_NAME));
        mHairCombo->setItemSelectState(mHairCombo->getItemCount()-1, true);
    }

    // pants
    mPantsCombo->findItemWithText(Parser.COLOR_PANTS_NAME, NULL);
    if (temp)
    {
        mPantsCombo->setItemSelectState(temp, true);
    }
    else
    {
        mPantsCombo->addItem(new MyListItem(Parser.COLOR_PANTS_NAME));
        mPantsCombo->setItemSelectState(mPantsCombo->getItemCount()-1, true);
    }

    // flame
    mFlameCombo->findItemWithText(Parser.COLOR_JET_NAME, NULL);
    if (temp)
    {
        mFlameCombo->setItemSelectState(temp, true);
    }
    else
    {
        mFlameCombo->addItem(new MyListItem(Parser.COLOR_JET_NAME));
        mFlameCombo->setItemSelectState(mFlameCombo->getItemCount()-1, true);
    }

    // shirt
    mShirtCombo->findItemWithText(Parser.COLOR_SHIRT_NAME, NULL);
    if (temp)
    {
        mShirtCombo->setItemSelectState(temp, true);
    }
    else
    {
        mShirtCombo->addItem(new MyListItem(Parser.COLOR_SHIRT_NAME));
        mShirtCombo->setItemSelectState(mShirtCombo->getItemCount()-1, true);
    }

    return 0;
}



void GUI::run()
{

    sf::Event Event;

	game.App.ShowMouseCursor(false);

    while (!must_quit)
    {
        while (game.App.GetEvent(Event))
        {
            //See if CEGUI should handle it
            if (handleEvent(Event))
                continue;

            switch (Event.Type)
            {
            case sf::Event::Closed:
                //Stop();
                break;
            case sf::Event::KeyPressed:
                if (Event.Key.Code == sf::Key::Escape)
                    mTab->setSelectedTabAtIndex(5);

                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        mSystem->renderGUI();

        if (WINDOW_NO_SOLDAT)
        {
            wns.show(CEGUI::System::getSingleton().getGUISheet());

            WINDOW_NO_SOLDAT = false;
        }
        game.App.Display();
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

/*
bool GUI::onKeySelector(const CEGUI::EventArgs& )
{
    wks.show(CEGUI::System::getSingleton().getGUISheet());
    //CEGUI::WindowManager::getSingleton().getWindow("KeySelector/Frame/Cancel")->
    //subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::KeySelectorClick, this));

    return true;
}
*/

GUI::~GUI()
{
    delete mSystem;
    delete mRenderer;
}


GUI::GUI()
{

    std::cout << "Loading GUI" << std::endl;

    mInput = &game.App.GetInput();

    ONLY_ON_START = true;

    initializeMaps();
    must_quit = false;

    mRenderer = new CEGUI::OpenGLRenderer(0, static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT));
	CEGUI::OpenGLRenderer::setDefaultImageCodecName("TGAImageCodec");
    //CEGUI::LuaScriptModule* script_module = new CEGUI::LuaScriptModule();


    //mRenderer = new CEGUI::OpenGLRenderer(0, App::Width, App::Height);
    mSystem = new CEGUI::System(mRenderer);


    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Errors);

    CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes",   game.CH_DATA_DIRECTORY + "schemes/");
    rp->setResourceGroupDirectory("imagesets", game.CH_DATA_DIRECTORY + "imagesets/");
    rp->setResourceGroupDirectory("fonts",     game.CH_DATA_DIRECTORY + "fonts/");
    rp->setResourceGroupDirectory("layouts",   game.CH_DATA_DIRECTORY + "layouts/");
    rp->setResourceGroupDirectory("looknfeel", game.CH_DATA_DIRECTORY + "looknfeel/");
//    rp->setResourceGroupDirectory("scripts",   game.CH_DATA_DIRECTORY + "scripts/");

    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    //CEGUI::ScriptModule::setDefaultResourceGroup("scripts");

    try
    {

        CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

        CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

        if (!CEGUI::FontManager::getSingleton().isFontPresent("DejaVuSans-10"))
            CEGUI::FontManager::getSingleton().createFont("DejaVuSans-10.font");

        CEGUI::Window* sheet = winMgr.createWindow("DefaultWindow", "root_wnd");
        CEGUI::System::getSingleton().setGUISheet(sheet);
        //CEGUI::System::getSingleton().executeScriptFile("gui.lua");

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

        mPlayerNameDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/playerNameDesc"));
        mPlayerNameDesc->setText((CEGUI::utf8*)_("Player name : "));

        mPlayerName = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/playerName"));
        mPlayerName->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&GUI::onPlayerName, this));

        mColorsDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/colorsDesc"));
        mColorsDesc->setText((CEGUI::utf8*)_("Select colors"));

        mHairDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/hairDesc"));
        mHairDesc->setText((CEGUI::utf8*)_("Hair"));
        mHairCombo = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/player/hairCombo"));
        mHairCombo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUI::onHairComboClick, this));

        mPantsDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/pantsDesc"));
        mPantsDesc->setText((CEGUI::utf8*)_("Pants"));
        mPantsCombo = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/player/pantsCombo"));
        mPantsCombo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUI::onPantsComboClick, this));
        //mPantsCombo->

        mFlameDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/flameDesc"));
        mFlameDesc->setText((CEGUI::utf8*)_("Flame"));
        mFlameCombo = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/player/flameCombo"));
        mFlameCombo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUI::onFlameComboClick, this));

        mShirtDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/shirtDesc"));
        mShirtDesc->setText((CEGUI::utf8*)_("Shirt"));
        mShirtCombo = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/player/shirtCombo"));
        mShirtCombo->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUI::onShirtComboClick, this));

        mKeyDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/keyDesc"));
        mKeyDesc->setText((CEGUI::utf8*)_("Click to select key"));

        mKey = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/player/key"));

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
        mJumpButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&GUI::onKeySelectorEnter, this));
        mJumpButton->subscribeEvent(CEGUI::PushButton::EventMouseLeaves, CEGUI::Event::Subscriber(&GUI::onKeySelectorLeave, this));
        mJumpButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onJumpButton, this));

        mMoveLeftButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/moveLeftButton"));
        mMoveLeftButton->setText((CEGUI::utf8*)_("Move Left"));
        mMoveLeftButton->setEnabled(false);
        mMoveLeftButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&GUI::onKeySelectorEnter, this));
        mMoveLeftButton->subscribeEvent(CEGUI::PushButton::EventMouseLeaves, CEGUI::Event::Subscriber(&GUI::onKeySelectorLeave, this));
        mMoveLeftButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onMoveLeftButton, this));

        mMoveRightButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/moveRightButton"));
        mMoveRightButton->setText((CEGUI::utf8*)_("Move Right"));
        mMoveRightButton->setEnabled(false);
        mMoveRightButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&GUI::onKeySelectorEnter, this));
        mMoveRightButton->subscribeEvent(CEGUI::PushButton::EventMouseLeaves, CEGUI::Event::Subscriber(&GUI::onKeySelectorLeave, this));
        mMoveRightButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onMoveRightButton, this));

        mCrouchButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/crouchButton"));
        mCrouchButton->setText((CEGUI::utf8*)_("Crouch"));
        mCrouchButton->setEnabled(false);
        mCrouchButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&GUI::onKeySelectorEnter, this));
        mCrouchButton->subscribeEvent(CEGUI::PushButton::EventMouseLeaves, CEGUI::Event::Subscriber(&GUI::onKeySelectorLeave, this));
        mCrouchButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onCrouchButton, this));

        mProneButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/proneButton"));
        mProneButton->setText((CEGUI::utf8*)_("Prone"));
        mProneButton->setEnabled(false);
        mProneButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onLeftClickStart, this));

        mThrowGrenadeButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/player/throwGrenadeButton"));
        mThrowGrenadeButton->setText((CEGUI::utf8*)_("Throw Grenade"));
        mThrowGrenadeButton->setEnabled(false);
        mThrowGrenadeButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&GUI::onKeySelectorEnter, this));
        mThrowGrenadeButton->subscribeEvent(CEGUI::PushButton::EventMouseLeaves, CEGUI::Event::Subscriber(&GUI::onKeySelectorLeave, this));
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

//        mGroupOpt1 = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/options/groupOpt1"));
//        mGroupOpt2 = static_cast<CEGUI::GroupBox*>(winMgr.getWindow("root/options/groupOpt2"));

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

        //mPlaceCHDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/placeCHDesc"));
        //mPlaceCHDesc->setText((CEGUI::utf8*)_("Directory with CH files :"));

        //mPlaceCH = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/placeCH"));
        //mPlaceCH->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onFolderSelector, this));

        //mStatusCHField = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/statusCHField"));

        mUpdateButton = static_cast<CEGUI::PushButton*>(winMgr.getWindow("root/options/updateButton"));
        mUpdateButton->setText((CEGUI::utf8*)_("Check for update"));
        mUpdateButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::onWindowUpdater, this));


        // Interface midtab
        mInterfacesDesc = static_cast<CEGUI::Editbox*>(winMgr.getWindow("root/options/interfacesDesc"));
        mInterfacesDesc->setText((CEGUI::utf8*)_("Interface type :"));

        mInterfaces = static_cast<CEGUI::Combobox*>(winMgr.getWindow("root/options/InterfacesBox"));
        mInterfaces->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUI::onInterfaceListClick, this));



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

        ApplyConfigs();

        // activate DM mode on default
        //CEGUI::EventArgs ev;
        //mDeathmatch->setSelected(true);
        //onDeathClick(ev);

       // std::cout << " DONE" << std::endl;

        run();

    }
    catch (CEGUI::Exception &e)
    {
        std::cerr << "Error initializing Cruel Hessian : " << e.getMessage().c_str() << std::endl;
    }

}
