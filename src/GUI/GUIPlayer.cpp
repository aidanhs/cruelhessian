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

#include "../ParserManager.h"
#include "GUI.h"
#include "../Game.h"


bool GUI::onKeySelectorEnter(const CEGUI::EventArgs& )
{
    mKeyDesc->setText((CEGUI::utf8*)_("Current key (click to change) : "));
    //n.
    mKey->setText("ix");
    return true;
}


bool GUI::onKeySelectorLeave(const CEGUI::EventArgs& )
{
    mKeyDesc->setText((CEGUI::utf8*)_("Select key"));
    mKey->setText("");
    return true;
}


// to jako nowy watek !!!!!!!!!!!!!
int GUI::catchKey(CEGUI::PushButton *whon)
{
/*
    setKeysState(false);
    whon->setEnabled(true);

    SDL_Event e;
    int key_temp = 0;

    while (key_temp == 0 || key_temp == 99999)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:

                CEGUI::System::getSingleton().injectKeyDown(e.key.keysym.scancode);

                if ((e.key.keysym.unicode & 0xFF80) == 0)
                {
                    CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode & 0x7F);
                }
                key_temp = e.key.keysym.unicode;
                break;

            default:
                break;
//            case SDL_MOUSEBUTTONDOWN:
//std::cout << "HAHA" << std::endl;
//                key_temp = 99999;
//                break;
            }
        }

    }

    return key_temp;*/
	return 1;
}

bool GUI::setKeysState(bool state)
{

    mFireButton->setEnabled(state);
    mFlyButton->setEnabled(state);
    mJumpButton->setEnabled(state);
    mMoveLeftButton->setEnabled(state);
    mMoveRightButton->setEnabled(state);
    mCrouchButton->setEnabled(state);
    mProneButton->setEnabled(state);
    mThrowGrenadeButton->setEnabled(state);
    mDropWeaponButton->setEnabled(state);
    mChatButton->setEnabled(state);
    mWeaponsMenuButton->setEnabled(state);
    mChangeWeaponButton->setEnabled(state);
    mReloadWeaponButton->setEnabled(state);
    mTeamChatButton->setEnabled(state);
    mRadioMenuButton->setEnabled(state);

    return true;

}


bool GUI::onJumpButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mJumpButton);
    if (key != 27 && key != 99999) Parser.KEY_UP = key;
    return true;
}


bool GUI::onMoveLeftButton(const CEGUI::EventArgs &)
{
    // wylacz wszystkie kliki
    // zmien napis w keyDesc
    int key = catchKey(mMoveLeftButton);
    if (key != 27 && key != 99999) Parser.KEY_LEFT = key;

    return true;
}

bool GUI::onMoveRightButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mMoveRightButton);
    if (key != 27 && key != 99999) Parser.KEY_RIGHT = key;
    return true;
}

bool GUI::onCrouchButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mCrouchButton);
    if (key != 27 && key != 99999) Parser.KEY_DOWN = key;
    return true;
}

bool GUI::onReloadWeaponButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mReloadWeaponButton);
    if (key != 27 && key != 99999) Parser.KEY_RELOAD = key;
    return true;
}

bool GUI::onThrowGrenadeButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mThrowGrenadeButton);
    if (key != 27 && key != 99999) Parser.KEY_GRENADE = key;
    return true;
}

bool GUI::onChatButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mChatButton);
    if (key != 27 && key != 99999) Parser.KEY_CHAT = key;
    return true;
}

bool GUI::onTeamChatButton(const CEGUI::EventArgs &)
{
    int key = catchKey(mTeamChatButton);
    if (key != 27 && key != 99999) Parser.KEY_TEAMCHAT = key;
    return true;
}


bool GUI::onPlayerName(const CEGUI::EventArgs& )
{

    Parser.PLAYER_NAME = mPlayerName->getText().c_str();
    return true;

}


bool GUI::onHairComboClick(const CEGUI::EventArgs& )
{

    Parser.COLOR_HAIR_NAME = mHairCombo->getSelectedItem()->getText().c_str();
    Parser.COLOR_HAIR  = game.tcolor2rgb(Parser.COLOR_HAIR_NAME);

    return true;
}

bool GUI::onFlameComboClick(const CEGUI::EventArgs& )
{

    Parser.COLOR_JET_NAME = mFlameCombo->getSelectedItem()->getText().c_str();
    Parser.COLOR_JET  = game.tcolor2rgb(Parser.COLOR_JET_NAME);

    return true;
}

bool GUI::onPantsComboClick(const CEGUI::EventArgs& )
{

    Parser.COLOR_PANTS_NAME = mPantsCombo->getSelectedItem()->getText().c_str();
    Parser.COLOR_PANTS  = game.tcolor2rgb(Parser.COLOR_PANTS_NAME);

    return true;
}

bool GUI::onShirtComboClick(const CEGUI::EventArgs& )
{

    Parser.COLOR_SHIRT_NAME = mShirtCombo->getSelectedItem()->getText().c_str();
    Parser.COLOR_SHIRT  = game.tcolor2rgb(Parser.COLOR_SHIRT_NAME);

    return true;
}
