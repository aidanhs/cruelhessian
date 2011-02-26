/*   GUIMaps.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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
#include "boost/filesystem/fstream.hpp"

#include "../Regex.h"
#include "GUI.h"
#include "../Game.h"
#include "../WorldMap.h"
#include "../ParserManager.h"


void GUI::showMaps(const std::string& mask)
{

    if (!boost::filesystem::exists(Parser.GAME_PATH + "Maps/"))
    {
        std::cout << "'Maps' directory doesn't exist !" << std::endl;
        mStartGameButton->setEnabled(false);
        return;
    }

    mMapList->resetList();

    std::string str;
    boost::filesystem::directory_iterator end;

    for (boost::filesystem::directory_iterator iter(Parser.GAME_PATH + "Maps/"); iter != end; ++iter)
    {
        if (regex_match(iter->leaf(), mask))
        {
            str.assign(iter->path().string().begin() + Parser.GAME_PATH.length() + 5, iter->path().string().end() - 4);
            mMapList->addItem(new MyListItem(str));
        }
    }
}


bool GUI::onLeftClickStart(const CEGUI::EventArgs& )
{

    if (mMapPlayList->getItemCount() > 0)
    {

        std::string xmap = Parser.GAME_PATH + "Maps/" + mMapPlayList->getListboxItemFromIndex(0)->getText().c_str() + ".PMS";

        Parser.FIRST_LIMIT = static_cast<int>(mSpinn1->getCurrentValue());
        Parser.LIMIT_TIME = 60*static_cast<int>(mSpinn2->getCurrentValue());

//
        if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == HTF || game.CURRENT_GAME_MODE == INF || game.CURRENT_GAME_MODE == TM)
        {

            game.worldMgr = new WorldMap(xmap);
            game.worldMgr->SetPlayers(4, mAlphaSpinner->isDisabled() ? 0 : Parser.RANDOM_BOTS_1,
                                      mBravoSpinner->isDisabled() ? 0 : Parser.RANDOM_BOTS_2,
                                      mCharlieSpinner->isDisabled() ? 0 : Parser.RANDOM_BOTS_3,
                                      mDeltaSpinner->isDisabled() ? 0 : Parser.RANDOM_BOTS_4);
        }
        else
        {

            game.worldMgr = new WorldMap(xmap);
            game.worldMgr->SetPlayers(1, Parser.RANDOM_BOTS);

        }

        delete game.worldMgr;

        mMapPlayList->resetList();
        mStartGameButton->setEnabled(false);
    }

    return true;
}


bool GUI::onLeftClickExit(const CEGUI::EventArgs& )
{
    Parser.SaveConfigs();
    must_quit = true;
    return true;
}

bool GUI::onMapListClicked(const CEGUI::EventArgs& )
{
    mMapPlayList->addItem(new MyListItem(mMapList->getFirstSelectedItem()->getText()));
    if (mStartGameButton->isDisabled())
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
    showMaps("[^(ctf_|htf_|inf_)].+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("Kill everything that moves.")));
    mDesc1->setText((CEGUI::utf8*)(_("Kill limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_DEATHMATCH));
    mMapPlayList->resetList();
    setBotStates(false, false, false, false);
    game.CURRENT_GAME_MODE = DM;
    return true;
}

bool GUI::onPointClick(const CEGUI::EventArgs& )
{
    showMaps("[^(ctf_|htf_|inf_)].+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("Get 1 point for kill. If you carry the Yellow Flag you get 2 points for kill. Also you get multipoints for multikills.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_POINTMATCH));
    mMapPlayList->resetList();
    setBotStates(false, false, false, false);
    game.CURRENT_GAME_MODE = PM;
    return true;
}

bool GUI::onTeamClick(const CEGUI::EventArgs& )
{
    showMaps("[^(ctf_|htf_|inf_)].+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("Up to 4 teams fight against each other.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_TEAMMATCH));
    mCharlieDesc->setEnabled(true);
    mCharlieSpinner->setEnabled(true);
    mDeltaDesc->setEnabled(true);
    mDeltaSpinner->setEnabled(true);
    setBotStates(true, true, true, true);
    mMapPlayList->resetList();
    game.CURRENT_GAME_MODE = TM;
    return true;
}


bool GUI::onRamboClick(const CEGUI::EventArgs& )
{
    showMaps("[^(ctf_|htf_|inf_)].+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("'First Blood' style. Whoever owns the Rambo Bow gains super Rambo powers. Only Rambo gets points for kill.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_RAMBOMATCH));
    mMapPlayList->resetList();
    setBotStates(false, false, false, false);
    game.CURRENT_GAME_MODE = RM;
    return true;
}

bool GUI::onCTFClick(const CEGUI::EventArgs& )
{
    showMaps("ctf_.+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("Capture the enemy flag and bring it to your base to score. 20 points for capture.")));
    mDesc1->setText((CEGUI::utf8*)(_("Capture limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_CAPTURE));
    mMapPlayList->resetList();
    setBotStates(true, true, false, false);
    game.CURRENT_GAME_MODE = CTF;
    return true;
}

bool GUI::onHTFClick(const CEGUI::EventArgs& )
{
    showMaps("htf_.+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("Get the yellow flag and hold it with your team for as long as possible. The team earns 1 point every 5 seconds of holding.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_HOLD));
    mMapPlayList->resetList();
    setBotStates(true, true, false, false);
    game.CURRENT_GAME_MODE = HTF;
    return true;
}

bool GUI::onINFClick(const CEGUI::EventArgs& )
{
    showMaps("inf_.+.pms$");
    mDescription->setText((CEGUI::utf8*)(_("Red team gets 30 points for retreiving the black flag. Blue team gets 1 point every 5 seconds if the flag is in base.")));
    mDesc1->setText((CEGUI::utf8*)(_("Point limit")));
    mSpinn1->setCurrentValue(static_cast<float>(Parser.LIMIT_INFILTRATION));
    mMapPlayList->resetList();
    setBotStates(true, true, false, false);
    game.CURRENT_GAME_MODE = INF;
    return true;
}


bool GUI::onSpinner1Changed(const CEGUI::EventArgs& )
{
    int sel = static_cast<int>(mSpinn1->getCurrentValue());

    switch (game.CURRENT_GAME_MODE)
    {
    case CTF :
        Parser.LIMIT_CAPTURE = sel;
        break;

    case HTF :
        Parser.LIMIT_HOLD = sel;
        break;

    case INF :
        Parser.LIMIT_INFILTRATION = sel;
        break;

    case PM :
        Parser.LIMIT_POINTMATCH = sel;
        break;

    case RM :
        Parser.LIMIT_RAMBOMATCH = sel;
        break;

    case DM :
        Parser.LIMIT_DEATHMATCH = sel;
        break;

    case TM :
        Parser.LIMIT_TEAMMATCH = sel;
        break;

    default :
        break;
    }

    return true;
}


bool GUI::onSpinner2Changed(const CEGUI::EventArgs& )
{
    Parser.LIMIT_TIME = 60*static_cast<int>(mSpinn2->getCurrentValue());
    return true;
}

bool GUI::onSpinnerRandomBotsChanged(const CEGUI::EventArgs& )
{
    Parser.RANDOM_BOTS = static_cast<int>(mRandomBotsSpinner->getCurrentValue());
    return true;
}

bool GUI::onSpinnerAlphaBotsChanged(const CEGUI::EventArgs& )
{
    Parser.RANDOM_BOTS_1 = static_cast<int>(mAlphaSpinner->getCurrentValue());
    return true;
}

bool GUI::onSpinnerBravoBotsChanged(const CEGUI::EventArgs& )
{
    Parser.RANDOM_BOTS_2 = static_cast<int>(mBravoSpinner->getCurrentValue());
    return true;
}

bool GUI::onSpinnerCharlieBotsChanged(const CEGUI::EventArgs& )
{
    Parser.RANDOM_BOTS_3 = static_cast<int>(mCharlieSpinner->getCurrentValue());
    return true;
}

bool GUI::onSpinnerDeltaBotsChanged(const CEGUI::EventArgs& )
{
    Parser.RANDOM_BOTS_4 = static_cast<int>(mDeltaSpinner->getCurrentValue());
    return true;
}
