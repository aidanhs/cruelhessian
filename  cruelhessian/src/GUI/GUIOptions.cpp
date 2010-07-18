/*   GUIOptions.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawel Konieczny <konp84 at gmail.com>
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


#include "../GUI.h"
#include "../Game.h"


// wybrano radio button 'soldat mode'
bool GUI::onSoldatModeClick(const CEGUI::EventArgs& )
{

    mPlaceSoldatDesc->setEnabled(true);
    mPlaceSoldat->setEnabled(true);
    mStatusSoldatField->setEnabled(true);

    mPlaceCHDesc->setEnabled(false);
    mPlaceCH->setEnabled(false);
    mStatusCHField->setEnabled(false);
    mUpdateButton->setEnabled(false);

    mInterfaces->resetList();

    game.SOL_PATH = game.SOL_PATH_0;
    game.MODE = 0;

    checkSoldat();

    readM3U();

    return true;

}


// wybrano radio button 'cruel hessian mode'
bool GUI::onCHModeClick(const CEGUI::EventArgs& )
{
//std::cout << "POKA" << std::endl;
    mPlaceCHDesc->setEnabled(true);
    mPlaceCH->setEnabled(true);
    mStatusCHField->setEnabled(true);
    mUpdateButton->setEnabled(true);

    mPlaceSoldatDesc->setEnabled(false);
    mPlaceSoldat->setEnabled(false);
    mStatusSoldatField->setEnabled(false);

    mInterfaces->resetList();

    game.SOL_PATH = game.SOL_PATH_1;
    game.MODE = 1;

    checkCH();

    readM3U();

    return true;

}



void GUI::checkSoldat()
{

    if (game.checkSoldat())
    {
        mStatusSoldatField->setText((CEGUI::utf8*)_("Found !"));
        showInterfaces(game.SOL_PATH_0);
        CEGUI::EventArgs ev;
        mDeathmatch->setSelected(true);
        onDeathClick(ev);
    }
    else if (game.SOL_PATH_0 == "")
    {
        mStatusSoldatField->setText((CEGUI::utf8*)_("Not selected !"));
        mPlaceSoldat->setText("Click to select directory with Soldat files");
    }
    else
    {
        mStatusSoldatField->setText((CEGUI::utf8*)_("Not found !"));
        mMapList->resetList();
        mInterfaces->resetList();

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

    if (game.checkSoldat())
    {
        mStatusCHField->setText((CEGUI::utf8*)_("Found !"));
        showInterfaces(game.SOL_PATH_1);
        CEGUI::EventArgs ev;
        mDeathmatch->setSelected(true);
        onDeathClick(ev);
        mUpdateButton->setEnabled(false);
    }
    else if (game.SOL_PATH_1 == "")
    {

        mStatusCHField->setText((CEGUI::utf8*)_("Not selected !"));
        mPlaceCH->setText("Click to select EMPTY directory for CH files");
        mUpdateButton->setEnabled(false);
        mMapList->resetList();

        if (ONLY_ON_START)
        {
            mTab->setSelectedTabAtIndex(2);
            WINDOW_NO_SOLDAT = true;
        }
    }
    else
    {
        mStatusCHField->setText((CEGUI::utf8*)_("Not found !"));
        mMapList->resetList();
        mInterfaces->resetList();
        mUpdateButton->setEnabled(true);

        if (ONLY_ON_START)
        {
            mTab->setSelectedTabAtIndex(2);
            WINDOW_NO_SOLDAT = true;
        }

    }

    ONLY_ON_START = false;

}
