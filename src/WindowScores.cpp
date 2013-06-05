/*   WindowScores.cpp
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


#include <algorithm>
#include <cstdio>
#include "WindowScores.h"
#include "WorldMap.h"
#include "Bot.h"
#include "Game.h"
#include "FontManager.h"
#include "InterfaceBaseManager.h"
#include "ParserManager.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


WindowScores::WindowScores()
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

    float xx = Parser.MAX_WIDTH / static_cast<float>(column_names.size());

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



//void WindowScores::update(const std::vector<Bot *>& bb)
void WindowScores::Update()
{

    list_long = static_cast<float>(world.bot.size()*15+80);

    // refresh the statistics
    scores.assign(world.bot.begin(), world.bot.end());
    std::sort(scores.begin(), scores.end(), cmp);

}


void WindowScores::draw_help(const Tex& texture, float dx, float dy) const
{
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(dx, dy, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2i(0, 0);
    glTexCoord2i(1, 1);
    glVertex2i(texture.w, 0);
    glTexCoord2i(1, 0);
    glVertex2i(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2i(0, texture.h);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}


inline static std::string IntToString(int num)
{
    char chars[128];
    sprintf(chars, "%i", num);
    return std::string(chars);
}


void WindowScores::Draw(void) const
{

    float offset = 20;

    glPushMatrix();
    glLoadIdentity();

    // obramowanie
    glColor4ub(247, 247, 247, 20);
    glRectf(offset-1, offset-1, Parser.MAX_WIDTH-offset+1, list_long-1);

    // tlo
    glColor4ub(60, 60, 60, 180);
    glBegin(GL_QUADS);
    glVertex2f(offset, offset);
    glVertex2f(Parser.MAX_WIDTH-offset, offset);
    glVertex2f(Parser.MAX_WIDTH-offset, list_long);
    glVertex2f(offset, list_long);
    glEnd();
    glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

    // show columns names
    for (unsigned int i = 0; i < column_names.size(); ++i)
        Fonts.printText(Fonts.font[0], Fonts.FontMenu, column_names[i], Fonts.textColorGunOnTouch, off[i], offset+10);

    offset += 40;

    if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == HTF || game.CURRENT_GAME_MODE == INF || game.CURRENT_GAME_MODE == TM)
    {
        for (unsigned int j = 0; j < scores.size(); ++j)
        {
            // is it me ?
            if (scores[j]->number == world.MY_BOT_NR)
                draw_help(InterfaceBase.text_smalldot, off[0]-10, offset+j*15+5);

            // is dead ?
            else if (scores[j]->isKilled)
                draw_help(InterfaceBase.text_deaddot, off[0]-10, offset+j*15+5);

            Fonts.printText(Fonts.font[1], Fonts.FontConsole, scores[j]->name, Fonts.textCol[scores[j]->team], off[0], offset+j*15);
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(scores[j]->points), Fonts.textCol[scores[j]->team], off[1], offset+j*15);
			Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(scores[j]->deaths), Fonts.textCol[scores[j]->team], off[2], offset+j*15);
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(scores[j]->ping), Fonts.textCol[scores[j]->team], off[3], offset+j*15);
        }

    }
    else if (game.CURRENT_GAME_MODE == DM || game.CURRENT_GAME_MODE == PM || game.CURRENT_GAME_MODE == RM)
    {
        for (unsigned int j = 0; j < scores.size(); ++j)
        {
            // is it me ?
            if (scores[j]->number == world.MY_BOT_NR)
                draw_help(InterfaceBase.text_smalldot, off[0]-10, offset+j*15+5);

            // is dead ?
            else if (scores[j]->isKilled)
                draw_help(InterfaceBase.text_deaddot, off[0]-10, offset+j*15+5);

            Fonts.printText(Fonts.font[1], Fonts.FontConsole, scores[j]->name, scores[j]->color[SHIRT], off[0], offset+j*15);
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(scores[j]->killedNr), scores[j]->color[SHIRT], off[1], offset+j*15);
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(scores[j]->deaths), scores[j]->color[SHIRT], off[2], offset+j*15);
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, IntToString(scores[j]->ping), scores[j]->color[SHIRT], off[3], offset+j*15);
        }
    }

}
