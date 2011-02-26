/*   WindowGuns.cpp
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


#include "WindowGuns.h"
#include "WeaponManager.h"
#include <cstdio>
#include "FontManager.h"
#include "ParserManager.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



WindowGuns::WindowGuns() :
    m_mouse_pos(0.0f, 0.0f),
    x(Parser.MAX_WIDTH/4),
    y(Parser.MAX_HEIGHT/7),
    w(300),
    h(350),
    m_number(-1)
  //  m_cChar(new char)
{

}


WindowGuns::~WindowGuns()
{
  //  delete m_cChar;
}


void WindowGuns::draw_help(const Tex& texture, float dx, float dy) const
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


void WindowGuns::Update(const TVector2D& pos)
{
    m_mouse_pos = pos;
}


void WindowGuns::Draw()
{

    bool stan1 = false;

    float delta = 20.0f;

    if ((m_mouse_pos.x > x + 5) && (m_mouse_pos.x < x + w - 5))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();

    // wewnetrzny prostokat
    glColor4ub(247, 247, 247, 70);
    glRectf(x, y, x + w, y + h);

    // obramowanie
    glColor4ub(0, 80, 140, 0);
    glBegin(GL_QUADS);
    glVertex2f(x - 1, y - 1);
    glVertex2f(x + w + 1, y - 1);
    glVertex2f(x + w + 1, y + h + 1);
    glVertex2f(x - 1, y + h + 1);
    glEnd();
    glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

    // tekst
    TVector2D start = TVector2D(x + 10, y + 10);
    Fonts.printText(Fonts.font[1], Fonts.FontConsole, "Primary Weapon:", Fonts.textGunColor, start.x, start.y);

    m_number = -1;

    std::string str;
    char m_cChar[2];
    start.x += 85.0f;
    for (int i = 1; i <= 10; ++i)
    {
        start.y += delta;
        if (Parser.WEAPON[i-1])
        {
            draw_help(Weapons[i].textureGun, start.x-85, start.y);
            sprintf(m_cChar, "%i", i%10);
            str = m_cChar;
            str.append(" " + Weapons[i].name);

            if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
            {
                Fonts.printText(Fonts.font[1], Fonts.FontConsole, str, Fonts.textColorGunOnTouch, start.x, start.y);
                m_number = i;
            }
            else
            {
                Fonts.printText(Fonts.font[1], Fonts.FontConsole, str, Fonts.textColorGunNormal, start.x, start.y);
            }

        }
    }


    start.y += 30.0f;
    Fonts.printText(Fonts.font[1], Fonts.FontConsole, "Secondary Weapon:", Fonts.textGunColor, start.x-85, start.y);

    start.y += delta;
    if (Parser.WEAPON[10])
    {
        draw_help(Weapons[0].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[0].name, Fonts.textColorGunOnTouch, start.x, start.y);
            m_number = 0;
        }
        else
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[0].name, Fonts.textColorGunNormal, start.x, start.y);
        }
    }

    start.y += delta;
    if (Parser.WEAPON[11])
    {
        draw_help(Weapons[14].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[14].name, Fonts.textColorGunOnTouch, start.x, start.y);
            m_number = 14;
        }
        else
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[14].name, Fonts.textColorGunNormal, start.x, start.y);
        }
    }

    start.y += delta;
    if (Parser.WEAPON[12])
    {
        draw_help(Weapons[15].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[15].name, Fonts.textColorGunOnTouch, start.x, start.y);
            m_number = 15;
        }
        else
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[15].name, Fonts.textColorGunNormal, start.x, start.y);
        }
    }

    start.y += delta;
    if (Parser.WEAPON[13])
    {
        draw_help(Weapons[16].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[16].name, Fonts.textColorGunOnTouch, start.x, start.y);
            m_number = 16;
        }
        else
        {
            Fonts.printText(Fonts.font[1], Fonts.FontConsole, Weapons[16].name, Fonts.textColorGunNormal, start.x, start.y);
        }
    }

}



int WindowGuns::select() const
{
    return m_number;
}
