/*   window_scores.cpp
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


#include <algorithm>
#include <sstream>
#include "WindowScores.h"
#include "Game.h"


WindowScores::WindowScores(const Tex& tex_d, const Tex& tex_s, unsigned int nr) : texture_d(tex_d), texture_s(tex_s), my_bot_nr(nr)
{

    column_names.push_back("Player:");
    if (game.CURRENT_GAME_MODE == DM)
    {
        column_names.push_back("Kills:");
    }
    else
    {
        column_names.push_back("Points:");
    }
    column_names.push_back("Deaths:");
    column_names.push_back("Ping:");

        // distance between columns
    off.resize(column_names.size());

    float xx = game.MAX_WIDTH / static_cast<float>(column_names.size());

    for (unsigned int i = 0; i < column_names.size(); ++i)
        off[i] = xx*(0.5f + i) - 40;

}


WindowScores::~WindowScores()
{
    scores.clear();
    column_names.clear();
    off.clear();
}


inline bool cmp(const Bot* bot1, const Bot* bot2)
{
    return (bot1->points > bot2->points);
}



void WindowScores::update(const std::vector<Bot *>& bb)
{

    list_long = static_cast<float>(bb.size()*15+80);

    // refresh the statistics
    scores.assign(bb.begin(), bb.end());
    std::sort(scores.begin(), scores.end(), cmp);

}


void WindowScores::draw_help(const Tex& texture, float dx, float dy) const
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(dx, dy, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(texture.w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, texture.h);
    glEnd();

    glPopMatrix();

}



void WindowScores::draw() const
{

    float offset = 20;
    std::ostringstream oss;

    glPushMatrix();
    glLoadIdentity();

    // obramowanie
    glColor4ub(247, 247, 247, 20);
    glRectf(offset-1, offset-1, game.MAX_WIDTH-offset+1, list_long-1);

    // tlo
    glColor4ub(60, 60, 60, 180);
    glBegin(GL_QUADS);
    glVertex2f(offset, offset);
    glVertex2f(game.MAX_WIDTH-offset, offset);
    glVertex2f(game.MAX_WIDTH-offset, list_long);
    glVertex2f(offset, list_long);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    // show columns names
    for (unsigned int i = 0; i < column_names.size(); ++i)
        world.printText(world.font[0][game.FontMenuSize], column_names[i], world.textColorGunOnTouch, off[i], offset+10);

    offset += 40;

    if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == HTF || game.CURRENT_GAME_MODE == INF || game.CURRENT_GAME_MODE == TM)
    {
        for (unsigned int j = 0; j < scores.size(); ++j)
        {
            // is it me ?
            if (scores[j]->number == my_bot_nr)
                draw_help(texture_s, off[0]-10, offset+j*15+5);

            // is dead ?
            else if (scores[j]->isKilled)
                draw_help(texture_d, off[0]-10, offset+j*15+5);

            world.printText(world.font[1][game.FontConsoleSize], scores[j]->name, world.textCol[scores[j]->team], off[0], offset+j*15);
            oss << scores[j]->points;
            world.printText(world.font[1][game.FontConsoleSize], oss.str(), world.textCol[scores[j]->team], off[1], offset+j*15);
            oss.str("");
            oss << scores[j]->deaths;
            world.printText(world.font[1][game.FontConsoleSize], oss.str(), world.textCol[scores[j]->team], off[2], offset+j*15);
            oss.str("");
            oss << scores[j]->ping;
            world.printText(world.font[1][game.FontConsoleSize], oss.str(), world.textCol[scores[j]->team], off[3], offset+j*15);
            oss.str("");
        }

    }
    else if (game.CURRENT_GAME_MODE == DM || game.CURRENT_GAME_MODE == PM || game.CURRENT_GAME_MODE == RM)
    {
        for (unsigned int j = 0; j < scores.size(); ++j)
        {
            // is it me ?
            if (scores[j]->number == my_bot_nr)
                draw_help(texture_s, off[0]-10, offset+j*15+5);

            // is dead ?
            else if (scores[j]->isKilled)
                draw_help(texture_d, off[0]-10, offset+j*15+5);

            world.printText(world.font[1][game.FontConsoleSize], scores[j]->name, scores[j]->color[SHIRT], off[0], offset+j*15);
            oss << scores[j]->killedNr;
            world.printText(world.font[1][game.FontConsoleSize], oss.str(), scores[j]->color[SHIRT], off[1], offset+j*15);
            oss.str("");
            oss << scores[j]->deaths;
            world.printText(world.font[1][game.FontConsoleSize], oss.str(), scores[j]->color[SHIRT], off[2], offset+j*15);
            oss.str("");
            oss << scores[j]->ping;
            world.printText(world.font[1][game.FontConsoleSize], oss.str(), scores[j]->color[SHIRT], off[3], offset+j*15);
            oss.str("");
        }
    }

    glDisable(GL_TEXTURE_2D);

}
