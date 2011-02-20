/*   GUIOptions.cpp
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


#include "boost/filesystem/fstream.hpp"

#include "../ParserManager.h"
#include "../AudioManager.h"
#include "GUI.h"
#include "../Game.h"


// wybrano radio button 'soldat mode'
bool GUI::onSoldatModeClick(const CEGUI::EventArgs& )
{

    //mPlaceSoldatDesc->setEnabled(true);
    mPlaceSoldat->setEnabled(true);
    mStatusSoldatField->setEnabled(true);
//    mGroupOpt1->setEnabled(true);

    //mPlaceCHDesc->setEnabled(false);
    //mPlaceCHDesc->setVisible(false);
    //mPlaceCH->setEnabled(false);
    //mStatusCHField->setEnabled(false);
    mUpdateButton->setEnabled(false);
//    mGroupOpt2->setEnabled(false);

    Parser.MODE = 0;
    Parser.SOL_PATH = Parser.SOL_PATH_[Parser.MODE];
	mPlaceSoldat->setText(Parser.SOL_PATH_[0]);

    checkSoldat();

    return true;

}


// wybrano radio button 'cruel hessian mode'
bool GUI::onCHModeClick(const CEGUI::EventArgs& )
{
//std::cout << "DHDH\n";
    //mPlaceCHDesc->setEnabled(true);
    //mPlaceCHDesc->setVisible(true);
    //mPlaceCH->setEnabled(true);
	mPlaceSoldat->setEnabled(true);
    //mStatusCHField->setEnabled(true);
	mStatusSoldatField->setEnabled(true);
    mUpdateButton->setEnabled(true);
 //   mGroupOpt2->setEnabled(true);

    //mPlaceSoldatDesc->setEnabled(false);
    mPlaceSoldat->setEnabled(true);
    mStatusSoldatField->setEnabled(false);
   // mGroupOpt1->setEnabled(false);

    Parser.MODE = 1;
    Parser.SOL_PATH = Parser.SOL_PATH_[Parser.MODE];
	mPlaceSoldat->setText(Parser.SOL_PATH_[1]);

    checkCH();

    return true;

}



void GUI::checkSoldat()
{

    mInterfaces->resetList();

    if (game.checkSoldatStart())
    {
        mStatusSoldatField->setText((CEGUI::utf8*)_("Found !"));
        game.CreateManagers();
        showInterfaces();
        selectInterface();
        if (AudioManager::GetSingletonPtr() != NULL)
            readM3U();
        CEGUI::EventArgs ev;
        onInterfaceListClick(ev);
        // nie wiadomo, czy DM
        mDeathmatch->setSelected(true);
        onDeathClick(ev);
//        game.LoadMapsList();
        for (unsigned int i = 0; i < game.mapsListFromFile.size(); ++i)
        {
            //std::cout << " DL " << game.mapsListFromFile[i].c_str() << std::endl;
            mMapPlayList->addItem(new MyListItem(game.mapsListFromFile[i]));
            if (mStartGameButton->isDisabled())
                mStartGameButton->setEnabled(true);
        }
    }
    else if (Parser.SOL_PATH == "")
    {
        mStatusSoldatField->setText((CEGUI::utf8*)_("Not selected !"));
        mPlaceSoldat->setText("Click to select directory with Soldat files");
        mMapList->resetList();
        mMapPlayList->resetList();

        if (ONLY_ON_START)
        {
            mTab->setSelectedTabAtIndex(2);
            WINDOW_NO_SOLDAT = true;
        }
    }
    else
    {
        mStatusSoldatField->setText((CEGUI::utf8*)_("Not found !"));
        mMapList->resetList();
        mMapPlayList->resetList();

        mIsMusic->setSelected(false);
        mMusicSpinner->setEnabled(false);
        mIsSounds->setSelected(false);
        mMusicSpinner->setEnabled(false);
        setMusicStates(false);

        if (ONLY_ON_START)
        {
            mTab->setSelectedTabAtIndex(2);
            WINDOW_NO_SOLDAT = true;
        }

    }

    ONLY_ON_START = false;

}


void GUI::checkCH()
{

    mInterfaces->resetList();

    if (game.checkSoldatStart())
    {
        //mStatusCHField->setText((CEGUI::utf8*)_("Found !"));
		mStatusSoldatField->setText((CEGUI::utf8*)_("Found !"));
        game.CreateManagers();
        showInterfaces();
        selectInterface();
        readM3U();
        CEGUI::EventArgs ev;
        onInterfaceListClick(ev);
        mDeathmatch->setSelected(true);
        onDeathClick(ev);
//        game.LoadMapsList();
        for (unsigned int i = 0; i < game.mapsListFromFile.size(); ++i)
        {
            //std::cout << " DL " << game.mapsListFromFile[i].length() << std::endl;
            mMapPlayList->addItem(new MyListItem(game.mapsListFromFile[i]));
            if (mStartGameButton->isDisabled())
                mStartGameButton->setEnabled(true);
        }
        mUpdateButton->setEnabled(false);
    }
    else if (Parser.SOL_PATH == "")
    {

        //mStatusCHField->setText((CEGUI::utf8*)_("Not selected !"));
		mStatusSoldatField->setText((CEGUI::utf8*)_("Not selected !"));
        //mPlaceCH->setText("Click to select EMPTY directory for CH files");
        //mPlaceCH->setText("Doesn't work, select Soldat mode");
		mPlaceSoldat->setText("Doesn't work, select Soldat mode");
        mUpdateButton->setEnabled(false);
        mMapList->resetList();
        mMapPlayList->resetList();

        if (ONLY_ON_START)
        {
            mTab->setSelectedTabAtIndex(2);
            WINDOW_NO_SOLDAT = true;
        }
    }
    else
    {
        //mStatusCHField->setText((CEGUI::utf8*)_("Not found !"));
		mStatusSoldatField->setText((CEGUI::utf8*)_("Not found !"));
        mMapList->resetList();
        mMapPlayList->resetList();
        mUpdateButton->setEnabled(true);

        mIsMusic->setSelected(false);
        mMusicSpinner->setEnabled(false);
        mIsSounds->setSelected(false);
        mMusicSpinner->setEnabled(false);
        setMusicStates(false);

        if (ONLY_ON_START)
        {
            mTab->setSelectedTabAtIndex(2);
            WINDOW_NO_SOLDAT = true;
        }

    }

    ONLY_ON_START = false;

}



// przeszukuje interfejsy z dysku i wstawia je do listy mInterface
void GUI::showInterfaces()
{

    std::string str;
    std::string fold_in = Parser.SOL_PATH + "Interface-gfx/";
    std::string fold_cus = Parser.SOL_PATH + "Custom-Interfaces/";
    boost::filesystem::directory_iterator end;

    if (!boost::filesystem::exists(fold_in))
    {
        std::cout << "'Interface-gfx' directory doesn't exist !" << std::endl;
        exit(EXIT_FAILURE);
    }

    mInterfaces->addItem(new MyListItem("--None--"));
    mInterfaces->addItem(new MyListItem("Default"));

    if (!boost::filesystem::exists(fold_cus))
    {
        std::cout << "'Custom-Interfaces' directory doesn't exist !" << std::endl;
    }
    else
    {
        for (boost::filesystem::directory_iterator iter(fold_cus); iter != end; ++iter)
        {
            str.assign(iter->path().string().begin() + fold_cus.length(), iter->path().string().end());
            mInterfaces->addItem(new MyListItem(str));
        }
    }

}

// zaznacza interfejs, ktory znajduje sie w zmiennej INTERFACE_NAME[MODE] (INTERFACE_NAME[MODE] variable has to be correct)
void GUI::selectInterface()
{

    if ((Parser.INTERFACE_NAME_[Parser.MODE] == "--None--") || (boost::filesystem::exists(Parser.INTERFACE_PATH_[Parser.MODE])))
    {

        CEGUI::ListboxItem *temp = mInterfaces->findItemWithText(Parser.INTERFACE_NAME_[Parser.MODE], NULL);

        if (temp)
        {
            mInterfaces->setItemSelectState(temp, true);
            std::cout << "Set interface to : " << Parser.INTERFACE_NAME_[Parser.MODE] << std::endl;
        }
        else
        {
            std::cout << "Cannot found : " << Parser.INTERFACE_NAME_[Parser.MODE] << std::endl;
        }

    }
    else
    {
        std::cout << "Wrong interface : " << Parser.INTERFACE_NAME_[Parser.MODE] << " in path : " << Parser.INTERFACE_PATH_[Parser.MODE] << std::endl;
    }

}

// ustawia zmienna INTERFACE_NAME i INTERFACE_PATH na podstawie wybranej pozycji w mInterface
bool GUI::onInterfaceListClick(const CEGUI::EventArgs& )
{

    if (mInterfaces->getSelectedItem() == NULL)
    {
        std::cout << "Cannot found " << Parser.INTERFACE_NAME_[Parser.MODE] << " interface, restoring default" << std::endl;
        Parser.INTERFACE_NAME = Parser.INTERFACE_NAME_[Parser.MODE] = "Default";
        mInterfaces->setItemSelectState(size_t(1), true);
    }
    else
    {
        Parser.INTERFACE_NAME = Parser.INTERFACE_NAME_[Parser.MODE] = mInterfaces->getSelectedItem()->getText().c_str();
    }


    if (Parser.INTERFACE_NAME == "--None--" || Parser.INTERFACE_NAME == "Default")
    {
        Parser.INTERFACE_PATH = Parser.SOL_PATH + "Interface-gfx/";
    }
    else if (boost::filesystem::exists(Parser.SOL_PATH + "Custom-Interfaces/" + Parser.INTERFACE_NAME + "/"))
    {
        Parser.INTERFACE_PATH = Parser.SOL_PATH + "Custom-Interfaces/" + Parser.INTERFACE_NAME + "/";
    }
    else
    {
        std::cerr << "Some error in 'onInterfaceListClick' function" << std::endl;
    }

    return true;

}
