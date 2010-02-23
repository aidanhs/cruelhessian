/*   window_guns.cpp
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


#include <sstream>
#include "window_guns.h"
#include "globals.h"
#include "SDL_opengl.h"


WindowGuns::WindowGuns(const std::vector<WeaponBase>& wb)
    : x(MAX_WIDTH/4), y(MAX_HEIGHT/7), w(300), h(350), m_weapon(wb)
{
    /*
         x = MAX_WIDTH/4;
         y = MAX_HEIGHT/7;
        w = 300;
        h = 350;
    */
}


void WindowGuns::draw_help(const Tex& tex, float dx, float dy) const
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


void WindowGuns::draw(const TVector2D& pos)
{

    m_mouse_pos = pos;
    bool stan1 = false;

    float delta = 20;

    std::ostringstream oss;

    if ((m_mouse_pos.x > x + 5) && (m_mouse_pos.x < x + w - 5))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(bgX, bgY, 0.0f);

    // obramowanie
    glColor4ub(247, 247, 247, 50);
    glRectf(x, y, x + w, y + h);

    // inner rectangle
    glColor4ub(0, 80, 140, 25);
    glBegin(GL_QUADS);
    glVertex2f(x - 1, y - 1);
    glVertex2f(x + w + 1, y - 1);
    glVertex2f(x + w + 1, y + h + 1);
    glVertex2f(x - 1, y + h + 1);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    // tekst
    TVector2D start = TVector2D(x+10, y+10);
    printText(font2_12, "Primary Weapon:", textGunColor, start.x, start.y);

    m_number = -1;

    start.x += 85;
    for (int i = 1; i <= 10; ++i)
    {
        start.y += delta;
        draw_help(m_weapon[i].textureGun, start.x-85, start.y);
        oss << i%10;
        oss << " " + m_weapon[i].name;
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            printText(font2_12, oss.str(), textColorGunOnTouch, start.x, start.y);
            m_number = i;
        }
        else
        {
            printText(font2_12, oss.str(), textColorGunNormal, start.x, start.y);
        }

        oss.str("");
    }

    start.y += 30;
    printText(font2_12, "Secondary Weapon:", textGunColor, start.x-85, start.y);

    start.y += delta;
    draw_help(m_weapon[0].textureGun, start.x-85, start.y);
    if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
    {
        printText(font2_12, m_weapon[0].name, textColorGunOnTouch, start.x, start.y);
        m_number = 0;
    }
    else
    {
        printText(font2_12, m_weapon[0].name, textColorGunNormal, start.x, start.y);
    }

    start.y += delta;
    draw_help(m_weapon[14].textureGun, start.x-85, start.y);
    if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
    {
        printText(font2_12, m_weapon[14].name, textColorGunOnTouch, start.x, start.y);
        m_number = 14;
    }
    else
    {
        printText(font2_12, m_weapon[14].name, textColorGunNormal, start.x, start.y);
    }

    start.y += delta;
    draw_help(m_weapon[15].textureGun, start.x-85, start.y);
    if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
    {
        printText(font2_12, m_weapon[15].name, textColorGunOnTouch, start.x, start.y);
        m_number = 15;
    }
    else
    {
        printText(font2_12, m_weapon[15].name, textColorGunNormal, start.x, start.y);
    }

    start.y += delta;
    draw_help(m_weapon[16].textureGun, start.x-85, start.y);
    if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
    {
        printText(font2_12, m_weapon[16].name, textColorGunOnTouch, start.x, start.y);
        m_number = 16;
    }
    else
    {
        printText(font2_12, m_weapon[16].name, textColorGunNormal, start.x, start.y);
    }

    glDisable(GL_TEXTURE_2D);

}



int WindowGuns::select() const
{
    return m_number;
}

