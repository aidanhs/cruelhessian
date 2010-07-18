/*   window_exit.cpp
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


#include "WindowExit.h"
#include "Game.h"


WindowExit::WindowExit()
    : x(game.MAX_WIDTH/4), y(game.MAX_HEIGHT/7), w(200), h(80), m_number(false)
{

}


void WindowExit::draw(const TVector2D& pos)
{

    m_mouse_pos = pos;
    bool stan1 = false;

    if ((m_mouse_pos.x > x + 5) && (m_mouse_pos.x < x + w - 5))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();

    // obramowanie
    glColor4ub(85, 85, 85, 200);
    glRectf(x, y, x + w, y + h);

    // inner rectangle
    glColor4ub(100, 100, 100, 200);
    glBegin(GL_QUADS);
    glVertex2f(x - 1, y - 1);
    glVertex2f(x + w + 1, y - 1);
    glVertex2f(x + w + 1, y + h + 1);
    glVertex2f(x - 1, y + h + 1);
    glEnd();
    glPopMatrix();

    // tekst
    TVector2D start = TVector2D(x + 10, y + 20);
    m_number = false;

    if (stan1 && m_mouse_pos.y > start.y-7 && m_mouse_pos.y < start.y+7)
    {
        world.printText(world.font[0][game.FontMenuSize], "Exit", world.textColorGunOnTouch, start.x, start.y);
        m_number = true;
    }
    else
    {
        world.printText(world.font[0][game.FontMenuSize], "Exit", world.textColorGunNormal, start.x, start.y);
    }

    start.y += 25;
    world.printText(world.font[1][game.FontConsoleSize], "Esc - back to game", world.textColorGunNormal, start.x, start.y);

}


// return 1 if CHOICE_EXIT should be true
bool WindowExit::select() const
{
    return m_number;
}