/*   chat.cpp
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


#include "Chat.h"
#include "WorldMap.h"
#include "Game.h"


void Chat::addMessage(const std::string& part1, const std::string& part2)
{
    chat_list.push_back(std::make_pair(part1, part2));
}

void Chat::addMessage(const std::string& part)
{
    chat_list.push_back(std::make_pair("", part));
}


void Chat::draw()
{

    float j = 0.0f;

    for (std::list<string_pair>::iterator i = chat_list.begin(); i != chat_list.end(); ++i)
    {
        if ((*i).first != "")
            world.printText(world.font[1][game.FontConsoleSize], "[" + (*i).first + "] " + (*i).second, world.textColorGunOnTouch, 10.0f, (j++)*15.0f + 15.0f);
        else
            world.printText(world.font[1][game.FontConsoleSize], (*i).second, world.textColorGunNormal, 10.0f, (j++)*15.0f + 15.0f);
    }
}


void Chat::update()
{
    // if chat_list isn't empty ...
    // ... delete the first entry after each 7 seconds
    if (world.getCurrentTime - lastOutChatEntity >= 7000)
    {
        if (!chat_list.empty())
            chat_list.pop_front();
        lastOutChatEntity = world.getCurrentTime;
    }
}

Chat::Chat()
    : lastOutChatEntity(world.getCurrentTime)
{
    //lastOutChatEntity = getCurrentTime;
}

Chat::~Chat()
{
    chat_list.clear();
}
