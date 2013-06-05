/*   WritingLine.h
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

#ifndef WRITING_LINE_H
#define WRITING_LINE_H


#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>


    enum Result
    {
        NOTHING = 0,
        ENTERED,
        //ESCAPED
        BACKSPACED
    };


class WritingLine
{
private:
    typedef std::list<std::string> InputList;
    Result lastResult;
    std::string input, lastInput;

public:
    WritingLine() : lastResult(NOTHING), input(""), lastInput("") {};
    ~WritingLine() {};
    const std::string& GetInput() const;
    const std::string GetInputEntered() const;
    bool IsEscaped() const;
    void ClearCache();
    Result Query(const sf::Key::Code& lastKey);

};


#endif
