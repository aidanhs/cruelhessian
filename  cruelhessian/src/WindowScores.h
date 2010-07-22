#ifndef WINDOW_SCORES_H
#define WINDOW_SCORES_H

/*   WindowScores.h
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

#include "Tex.h"
#include "Bot.h"



class WindowScores
{
    const Tex& texture_d;
    const Tex& texture_s;
    std::vector<std::string> column_names;
    std::vector<float> off;
    float list_long;
    unsigned int my_bot_nr;
    void draw_help(const Tex& tex, float dx, float dy) const;

public:

    std::vector<Bot *> scores;
    WindowScores(const Tex& tex_d, const Tex& tex_s, unsigned int);
    ~WindowScores();

    void draw() const;
    //void update(const std::vector<Bot *>& bb);
    void update();
};


#endif

