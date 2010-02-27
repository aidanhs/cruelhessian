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


#include "window_exit.h"
#include "globals.h"
#include "SDL_opengl.h"


WindowExit::WindowExit()
    : x(MAX_WIDTH/4), y(MAX_HEIGHT/7), w(200), h(80), m_number(0)
{
    /*
        x = MAX_WIDTH/4;
        y = MAX_HEIGHT/7;
        w = 200;
        h = 80;
    */
}


void WindowExit::draw(const TVector2D& pos)
{

    m_mouse_pos = pos;
    bool stan1 = false;

    if ((m_mouse_pos.x > x + 5) && (m_mouse_pos.x < x + w - 5))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(bgX, bgY, 0.0f);

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
    m_number = 0;

    if (stan1 && m_mouse_pos.y > start.y-7 && m_mouse_pos.y < start.y+7)
    {
        printText(font1_16, "Exit", textColorGunOnTouch, start.x, start.y);
        m_number = 1;
    }
    else
    {
        printText(font1_16, "Exit", textColorGunNormal, start.x, start.y);
    }

    start.y += 25;
    printText(font2_12, "Esc - back to game", textColorGunNormal, start.x, start.y);

}


// return 1 if CHOICE_EXIT should be true
int WindowExit::select() const
{
    return m_number;
}
