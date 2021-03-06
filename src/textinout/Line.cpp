/*   Line.cpp
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


#include "Line.h"
#include "../WorldMap.h"
#include "../FontManager.h"
#include "../ParserManager.h"


const std::string& Line::GetInput() const
{
    return input;
}


const std::string Line::GetInputEntered() const
{
    if (lastResult == ENTERED)
    {
        //    if (lastInput[0] == '/')
        //      lastInput.erase(0, 1);
        return lastInput;
    }
    else return "";
}


void Line::clearCache()
{
    lastResult = NOTHING;
    lastInput = "";
}


Result Line::Query(const sf::Key::Code& lastKey)
{
    if (lastResult == ENTERED)
    {
        input.clear();
    }

    if (lastKey == sf::Key::Return)
    {
        lastInput = input;
        input.clear();
        lastResult = ENTERED;
    }
    else if (lastKey >= sf::Key::A && lastKey <= sf::Key::Z)
    {
        input += static_cast<char>(lastKey);
        lastResult = NOTHING;
    }
    else if (lastKey == sf::Key::Back)
    {
        lastResult = BACKSPACED;
        if (input.length() > 0)
            input.erase(input.end()-1);
    }

    return lastResult;
}


bool Line::isEscaped() const
{
    return (lastResult == BACKSPACED && input.length() == 0);
}
