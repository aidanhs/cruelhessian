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
#include "window_scores.h"
#include "globals.h"


WindowScores::WindowScores(const Tex& tex) : texture(tex)
{

    column_names.push_back("Player:");
    if (CURRENT_GAME_MODE == DM)
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

    float xx = MAX_WIDTH / static_cast<float>(column_names.size());

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

    // aktualizacja statystyk
    scores.assign(bb.begin(), bb.end());
    std::sort(scores.begin(), scores.end(), cmp);

}


void WindowScores::draw_help(const Tex& tex, float dx, float dy) const
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(bgX + dx, bgY + dy, 0.0f);

    glBindTexture(GL_TEXTURE_2D, tex.tex);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(tex.w, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(tex.w, tex.h);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, tex.h);
    glEnd();

    glPopMatrix();

}



void WindowScores::draw() const
{

    float offset = 20;
    std::ostringstream oss;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(bgX, bgY, 0.0f);

    // obramowanie
    glColor4ub(247, 247, 247, 20);
    glRectf(offset-1, offset-1, MAX_WIDTH-offset+1, list_long-1);

    // tlo
    glColor4ub(60, 60, 60, 180);
    glBegin(GL_QUADS);
    glVertex2f(offset, offset);
    glVertex2f(MAX_WIDTH-offset, offset);
    glVertex2f(MAX_WIDTH-offset, list_long);
    glVertex2f(offset, list_long);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    // show columns names
    for (unsigned int i = 0; i < column_names.size(); ++i)
        printText(font1_16, column_names[i], textColorGunOnTouch, off[i], offset+10);

    offset += 40;

    if (CURRENT_GAME_MODE == CTF || CURRENT_GAME_MODE == HTF || CURRENT_GAME_MODE == INF || CURRENT_GAME_MODE == TM)
    {
        for (unsigned int j = 0; j < scores.size(); ++j)
        {
            // is dead ?
            if (scores[j]->isKilled)
                draw_help(texture, off[0]-10, offset+j*15+5);
            printText(font2_12, scores[j]->name, textCol[scores[j]->team], off[0], offset+j*15);
            oss << scores[j]->points;
            printText(font2_12, oss.str(), textCol[scores[j]->team], off[1], offset+j*15);
            oss.str("");
            oss << scores[j]->deaths;
            printText(font2_12, oss.str(), textCol[scores[j]->team], off[2], offset+j*15);
            oss.str("");
            oss << scores[j]->ping;
            printText(font2_12, oss.str(), textCol[scores[j]->team], off[3], offset+j*15);
            oss.str("");
        }

    }
    else if (CURRENT_GAME_MODE == DM || CURRENT_GAME_MODE == PM || CURRENT_GAME_MODE == RM)
    {
        for (unsigned int j = 0; j < scores.size(); ++j)
        {
            // is dead ?
            if (scores[j]->isKilled)
                draw_help(texture, off[0]-10, offset+j*15+5);
            printText(font2_12, scores[j]->name, scores[j]->color[SHIRT], off[0], offset+j*15);
            oss << scores[j]->killedNr;
            printText(font2_12, oss.str(), scores[j]->color[SHIRT], off[1], offset+j*15);
            oss.str("");
            oss << scores[j]->deaths;
            printText(font2_12, oss.str(), scores[j]->color[SHIRT], off[2], offset+j*15);
            oss.str("");
            oss << scores[j]->ping;
            printText(font2_12, oss.str(), scores[j]->color[SHIRT], off[3], offset+j*15);
            oss.str("");
        }
    }

    glDisable(GL_TEXTURE_2D);

}