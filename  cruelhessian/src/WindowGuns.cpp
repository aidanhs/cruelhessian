/*   WindowGuns.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawel Konieczny <konp84 at gmail.com>
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
#include "WeaponManager.h"
#include "WindowGuns.h"
#include "Game.h"



WindowGuns::WindowGuns()
    : x(game.MAX_WIDTH/4), y(game.MAX_HEIGHT/7), w(300), h(350), m_number(-1)
{

}


void WindowGuns::draw_help(const Tex& texture, float dx, float dy) const
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


void WindowGuns::draw(const TVector2D& pos)
{

    m_mouse_pos = pos;
    bool stan1 = false;

    float delta = 20.0f;

    std::ostringstream oss;

    if ((m_mouse_pos.x > x + 5) && (m_mouse_pos.x < x + w - 5))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();

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
    TVector2D start = TVector2D(x + 10, y + 10);
    world.printText(world.font[1][game.FontConsoleSize], "Primary Weapon:", world.textGunColor, start.x, start.y);

    m_number = -1;

    start.x += 85.0f;
    for (int i = 1; i <= 10; ++i)
    {
        start.y += delta;
        if (game.WEAPON[i-1])
        {
            draw_help(Weapons[i].textureGun, start.x-85, start.y);
            oss << i%10;
            oss << " " + Weapons[i].name;
            if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
            {
                world.printText(world.font[1][game.FontConsoleSize], oss.str(), world.textColorGunOnTouch, start.x, start.y);
                m_number = i;
            }
            else
            {
                world.printText(world.font[1][game.FontConsoleSize], oss.str(), world.textColorGunNormal, start.x, start.y);
            }

            oss.str("");
        }

    }

    start.y += 30.0f;
    world.printText(world.font[1][game.FontConsoleSize], "Secondary Weapon:", world.textGunColor, start.x-85, start.y);

    start.y += delta;
    if (game.WEAPON[10])
    {
        draw_help(Weapons[0].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[0].name, world.textColorGunOnTouch, start.x, start.y);
            m_number = 0;
        }
        else
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[0].name, world.textColorGunNormal, start.x, start.y);
        }
    }

    start.y += delta;
    if (game.WEAPON[11])
    {
        draw_help(Weapons[14].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[14].name, world.textColorGunOnTouch, start.x, start.y);
            m_number = 14;
        }
        else
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[14].name, world.textColorGunNormal, start.x, start.y);
        }
    }

    start.y += delta;
    if (game.WEAPON[12])
    {
        draw_help(Weapons[15].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[15].name, world.textColorGunOnTouch, start.x, start.y);
            m_number = 15;
        }
        else
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[15].name, world.textColorGunNormal, start.x, start.y);
        }
    }

    start.y += delta;
    if (game.WEAPON[13])
    {
        draw_help(Weapons[16].textureGun, start.x-85, start.y);
        if (stan1 && m_mouse_pos.y > start.y - 7.0f && m_mouse_pos.y < start.y + 7.0f)
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[16].name, world.textColorGunOnTouch, start.x, start.y);
            m_number = 16;
        }
        else
        {
            world.printText(world.font[1][game.FontConsoleSize], Weapons[16].name, world.textColorGunNormal, start.x, start.y);
        }
    }

    glDisable(GL_TEXTURE_2D);

}



int WindowGuns::select() const
{
    return m_number;
}
