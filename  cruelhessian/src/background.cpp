/*   background.cpp
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


#include "background.h"
#include "globals.h"



Background::Background(float startX, float startY)
{
    moveX = startX;
    moveY = startY;
}

void Background::setPosition(float startX, float startY)
{
    moveX = startX;
    moveY = startY;
}


void Background::update(const TVector2D& mouse_pos, const TVector2D& bot_pos)
{

    if (mouse_pos.x < bot_pos.x + MAX_WIDTH &&
             mouse_pos.x > bot_pos.x - MAX_WIDTH &&
             mouse_pos.y < bot_pos.y + MAX_HEIGHT &&
             mouse_pos.y > bot_pos.y - MAX_HEIGHT)
    {
        int rel_mouse_x, rel_mouse_y;          // roznica od ostatniego polozenia
        SDL_GetRelativeMouseState(&rel_mouse_x, &rel_mouse_y);
        moveX = -static_cast<GLfloat>(rel_mouse_x);
        moveY = -static_cast<GLfloat>(rel_mouse_y);
    }

}


void Background::draw(Map *map)
{

    glTranslatef(moveX, moveY, 0.0f);

    glBegin(GL_POLYGON);
    glColor4ub(static_cast<GLubyte>(map->bgColorTop.red), static_cast<GLubyte>(map->bgColorTop.green), static_cast<GLubyte>(map->bgColorTop.blue), static_cast<GLubyte>(map->bgColorTop.alpha));
    glVertex2f(map->leftoffs - MAX_WIDTH, -map->topoffs);
    glVertex2f(map->rightoffs + MAX_WIDTH, -map->topoffs);
    glColor4ub(static_cast<GLubyte>(map->bgColorBottom.red), static_cast<GLubyte>(map->bgColorBottom.green), static_cast<GLubyte>(map->bgColorBottom.blue), static_cast<GLubyte>(map->bgColorBottom.alpha));
    glVertex2f(map->rightoffs + MAX_WIDTH, -map->bottomoffs);
    glVertex2f(map->leftoffs - MAX_WIDTH, -map->bottomoffs);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    moveX = moveY = 0;

}