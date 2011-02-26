/*   ChatLine.cpp
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


#include "ChatLine.h"
#include "../WorldMap.h"
#include "../Bot.h"
#include "TextOutput.h"
#include "../FontManager.h"
#include "../ParserManager.h"


ChatLine::ChatLine()
{
    lastResult=NOTHING;
    input="";
    lastInput="";
}


void ChatLine::Draw()
{

    Fonts.printText(Fonts.font[1], Fonts.FontConsole, "Say : " + GetInput(), Fonts.textCol[3], 15.0f, 0.8f*Parser.MAX_HEIGHT);

    if (GetInputEntered() != "")
    {
        world.text_output->AddMessage(world.bot[world.MY_BOT_NR]->name, GetInputEntered());
        delete this;
    }

    else if (isEscaped())
    {
        delete this;
    }

}

ChatLine::~ChatLine()
{
    clearCache();
}
