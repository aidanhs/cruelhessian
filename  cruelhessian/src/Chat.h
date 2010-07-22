/*   Chat.h
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

#ifndef CHAT_H
#define CHAT_H


#include <iostream>
#include <list>
#include "SDL.h"


class Chat
{
    typedef std::pair<std::string, std::string> string_pair;
    std::list<string_pair> chat_list;
    Uint32 lastOutChatEntity;

public:
    Chat();
    ~Chat();

    void addMessage(const std::string& part1, const std::string& part2);
    void addMessage(const std::string& part);

    void update();
    void draw();
};


#endif
