/*   TextOutput.cpp
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


#include "TextOutput.h"
#include "../WorldMap.h"
#include "../FontManager.h"
#include "../ParserManager.h"


TextOutput::TextOutput() :
	m_fLastOutChatEntity(world.getCurrentTime)
{

}

TextOutput::~TextOutput()
{
    m_sChatList.clear();
}



void TextOutput::AddMessage(const std::string& part1, const std::string& part2)
{
    if (Parser.CONSOLE_SHOW)
        m_sChatList.push_back(std::make_pair(part1, part2));
}

void TextOutput::AddMessage(const std::string& part)
{
    if (Parser.CONSOLE_SHOW)
        m_sChatList.push_back(std::make_pair("", part));
}


void TextOutput::Draw()
{

    float j = 0.0f;

    for (std::list<m_xStringPair>::iterator i = m_sChatList.begin(); i != m_sChatList.end(); ++i)
    {
        if ((*i).first != "")
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, "[" + (*i).first + "] " + (*i).second, Fonts.textColorGunOnTouch, 10.0f, (j++)*15.0f + 15.0f);
        else
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, (*i).second, Fonts.textColorGunNormal, 10.0f, (j++)*15.0f + 15.0f);
    }
}


void TextOutput::Update()
{
    // if chat_list isn't empty ...
    // ... delete the first entry after each 7 seconds
    if (world.getCurrentTime - m_fLastOutChatEntity >= 7000)
    {
        if (!m_sChatList.empty())
            m_sChatList.pop_front();
        m_fLastOutChatEntity = world.getCurrentTime;
    }
}

