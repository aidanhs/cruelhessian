/*   Background.cpp
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


#include "Background.h"
#include "ParserManager.h"
#include "SDL.h"



Background::Background(float startX, float startY, const TVector2D& bot_pos)
{
    move.x = startX;
    move.y = startY;
    old_pos = bot_pos;
    flag = false;
}


void Background::setPosition(float startX, float startY)
{
    move.x = startX;
    move.y = startY;
    flag = true;
}


void Background::update(const TVector2D& mouse_pos, const TVector2D& bot_pos)
{

    if (!flag)
    {
        move += old_pos - bot_pos;
        old_pos = bot_pos;

        if (mouse_pos.x < bot_pos.x + Parser.MAX_WIDTH &&
                mouse_pos.x > bot_pos.x - Parser.MAX_WIDTH &&
                mouse_pos.y < bot_pos.y + Parser.MAX_HEIGHT &&
                mouse_pos.y > bot_pos.y - Parser.MAX_HEIGHT)
        {
            int rel_mouse_x, rel_mouse_y;
            SDL_GetRelativeMouseState(&rel_mouse_x, &rel_mouse_y);
            move.x += -static_cast<GLfloat>(rel_mouse_x);
            move.y += -static_cast<GLfloat>(rel_mouse_y);
        }
    }
    else
    {
        old_pos = bot_pos;
        flag = false;
    }

}


void Background::draw(const Map& map)
{

    glTranslatef(move.x, move.y, 0.0f);

    glBegin(GL_POLYGON);
    glColor4ub(static_cast<GLubyte>(map.bgColorTop.red), static_cast<GLubyte>(map.bgColorTop.green), static_cast<GLubyte>(map.bgColorTop.blue), static_cast<GLubyte>(map.bgColorTop.alpha));
    glVertex2f(map.leftoffs - Parser.MAX_WIDTH, -map.topoffs);
    glVertex2f(map.rightoffs + Parser.MAX_WIDTH, -map.topoffs);
    glColor4ub(static_cast<GLubyte>(map.bgColorBottom.red), static_cast<GLubyte>(map.bgColorBottom.green), static_cast<GLubyte>(map.bgColorBottom.blue), static_cast<GLubyte>(map.bgColorBottom.alpha));
    glVertex2f(map.rightoffs + Parser.MAX_WIDTH, -map.bottomoffs);
    glVertex2f(map.leftoffs - Parser.MAX_WIDTH, -map.bottomoffs);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    move.x = move.y = 0;

}
