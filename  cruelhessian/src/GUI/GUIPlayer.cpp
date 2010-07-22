/*   GUIPlayer.cpp
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

#include "../GUI.h"
#include "../Game.h"



bool GUI::onJumpButton(const CEGUI::EventArgs &)
{
    Parser.KEY_UP = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onMoveLeftButton(const CEGUI::EventArgs &)
{
    Parser.KEY_LEFT = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onMoveRightButton(const CEGUI::EventArgs &)
{
    Parser.KEY_RIGHT = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onCrouchButton(const CEGUI::EventArgs &)
{
    Parser.KEY_DOWN = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onReloadWeaponButton(const CEGUI::EventArgs &)
{
    std::cout << "PRESS " << GUI_KEY_PRESSED << std::endl;
   // IF_PRESSED = false;
    Parser.KEY_RELOAD = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onThrowGrenadeButton(const CEGUI::EventArgs &)
{
    Parser.KEY_GRENADE = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onChatButton(const CEGUI::EventArgs &)
{
    Parser.KEY_CHAT = GUI_KEY_PRESSED;
    return true;
}

bool GUI::onTeamChatButton(const CEGUI::EventArgs &)
{
    Parser.KEY_TEAMCHAT = GUI_KEY_PRESSED;
    return true;
}



bool GUI::onPlayerName(const CEGUI::EventArgs& )
{

    Parser.PLAYER_NAME = mPlayerName->getText().c_str();
    return true;

}
