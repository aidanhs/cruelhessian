/*   console.cpp
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


#include "console.h"
#include "worldmap.h"



const std::string& Console::GetInput() const
{
    return input;
}


const std::string Console::GetInputEntered() const
{
    if (lastResult == ENTERED)
    {
    //    if (lastInput[0] == '/')
      //      lastInput.erase(0, 1);
        return lastInput;
    }
    else return "";
}


void Console::clearCache()
{
    lastResult = NOTHING;
    lastInput = "";
}


Console::Result Console::Query(SDLKey lastKey)
{
    if (lastResult == ENTERED)
    {
        input.clear();
    }

    if (lastKey == SDLK_RETURN)
    {
        lastInput = input;
        input.clear();
        lastResult = ENTERED;
    }
    else if (lastKey >= SDLK_SPACE && lastKey <= SDLK_z)
    {
        input += static_cast<char>(lastKey);
        lastResult = NOTHING;
    }
    else if (lastKey == SDLK_BACKSPACE)
    {
        lastResult = BACKSPACED;
        if (input.length() > 0)
            input.erase(input.end()-1);
    }

    return lastResult;
}


bool Console::isEscaped() const
{
    return (lastResult == BACKSPACED && input.length() == 0);
}



void WorldMap::command_line()
{

    printText(font2_12, "Cmd : " + cons.GetInput(), textCol[3], 15.0f, 0.8f*MAX_HEIGHT);

    if (cons.GetInputEntered() == "kill" || cons.GetInputEntered() == "KILL")
    {
        hurt_bot(MY_BOT_NR, MY_BOT_NR, 10000);
    }
    else if (cons.GetInputEntered() == "victory" || cons.GetInputEntered() == "VICTORY")
    {
        bot[MY_BOT_NR]->movementType = CIESZY;
    }
    else if (cons.GetInputEntered() == "addbot" || cons.GetInputEntered() == "ADDBOT")
    {
//        addBot();

    }

    if (cons.GetInputEntered() != "" || cons.isEscaped())
    {
        SHOW_COMMAND_LINE = false;
        cons.clearCache();
    }

}


void WorldMap::mychat_line()
{

    printText(font2_12, "Say : " + myChat.GetInput(), textCol[3], 15.0f, 0.8f*MAX_HEIGHT);

    if (myChat.GetInputEntered() != "")
    {
        chat->addMessage(bot[MY_BOT_NR]->name, myChat.GetInputEntered());
        SHOW_MYCHAT_LINE = false;
	    myChat.clearCache();
    }

	else if (myChat.isEscaped())
	{
	    SHOW_MYCHAT_LINE = false;
	    myChat.clearCache();
	}

}
