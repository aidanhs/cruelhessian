/*   WindowExit.cpp
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


#include "WindowExit.h"
#include "ParserManager.h"
#include "FontManager.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


WindowExit::WindowExit() :
    m_fX(Parser.MAX_WIDTH/4),
    m_fY(Parser.MAX_HEIGHT/7),
    m_fWidth(200),
    m_fHeight(80),
    m_bNumber(false)
{

}


void WindowExit::Draw(const TVector2D& m_xMousePos)
{

    bool stan1 = false;

    if ((m_xMousePos.x > m_fX + 5) && (m_xMousePos.x < m_fX + m_fWidth - 5))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();

    // obramowanie
    glColor4ub(85, 85, 85, 200);
    glRectf(m_fX, m_fY, m_fX + m_fWidth, m_fY + m_fHeight);

    // inner rectangle
    glColor4ub(100, 100, 100, 200);
    glBegin(GL_QUADS);
    glVertex2f(m_fX - 1, m_fY - 1);
    glVertex2f(m_fX + m_fWidth + 1, m_fY - 1);
    glVertex2f(m_fX + m_fWidth + 1, m_fY + m_fHeight + 1);
    glVertex2f(m_fX - 1, m_fY + m_fHeight + 1);
    glEnd();
    glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

    // tekst
    TVector2D start = TVector2D(m_fX + 10, m_fY + 20);
    m_bNumber = false;

    // opcja jest zaznaczona
    if (stan1 && m_xMousePos.y > start.y-7 && m_xMousePos.y < start.y+7)
    {
        Fonts.printText(Fonts.font[0], Fonts.FontMenuSize, "Exit", Fonts.textColorGunOnTouch, start.x, start.y);
        m_bNumber = true;
    }
    else
    {
        Fonts.printText(Fonts.font[0], Fonts.FontMenuSize, "Exit", Fonts.textColorGunNormal, start.x, start.y);
    }

    start.y += 25;
    Fonts.printText(Fonts.font[1], Fonts.FontConsoleSize, "Esc - back to game", Fonts.textColorGunNormal, start.x, start.y);

}


// return 1 if CHOICE_EXIT should be true
bool WindowExit::Select() const
{
    return m_bNumber;
}
