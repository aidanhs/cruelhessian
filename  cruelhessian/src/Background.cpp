/*   Background.cpp
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


#include "Background.h"
#include "ParserManager.h"
#include "WorldMap.h"
#include "TVector2D.h"
#include "Map.h"
#include "Mouse.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


Background::Background(const TVector2D& start, const TVector2D& bot_pos) :
    m_xMove(start),
    m_xOldPos(bot_pos),
    m_bFlag(false)
{

}


void Background::SetPosition(const float startX, const float startY)
{
    m_xMove.x = startX;
    m_xMove.y = startY;
    m_bFlag = true;
}


void Background::Update(const TVector2D& mouse_pos, const TVector2D& bot_pos)
{

    if (!m_bFlag)
    {
        m_xMove += m_xOldPos - bot_pos;
        m_xOldPos = bot_pos;

        if (mouse_pos.x < bot_pos.x + Parser.MAX_WIDTH &&
                mouse_pos.x > bot_pos.x - Parser.MAX_WIDTH &&
                mouse_pos.y < bot_pos.y + Parser.MAX_HEIGHT &&
                mouse_pos.y > bot_pos.y - Parser.MAX_HEIGHT)
        {

            m_xMove.x += world.mouse->mRelative.x;
            m_xMove.y += world.mouse->mRelative.y;

        }
    }
    else
    {
        m_xOldPos = bot_pos;
        m_bFlag = false;
    }

}


void Background::Draw()
{

	glClear(GL_COLOR_BUFFER_BIT);

    glTranslatef(m_xMove.x, m_xMove.y, 0.0f);

    glBegin(GL_POLYGON);
    glColor4ub(static_cast<GLubyte>(world.map->bgColorTop.red), static_cast<GLubyte>(world.map->bgColorTop.green), static_cast<GLubyte>(world.map->bgColorTop.blue), static_cast<GLubyte>(world.map->bgColorTop.alpha));
    glVertex2f(world.map->leftoffs - Parser.MAX_WIDTH, -world.map->topoffs);
    glVertex2f(world.map->rightoffs + Parser.MAX_WIDTH, -world.map->topoffs);
    glColor4ub(static_cast<GLubyte>(world.map->bgColorBottom.red), static_cast<GLubyte>(world.map->bgColorBottom.green), static_cast<GLubyte>(world.map->bgColorBottom.blue), static_cast<GLubyte>(world.map->bgColorBottom.alpha));
    glVertex2f(world.map->rightoffs + Parser.MAX_WIDTH, -world.map->bottomoffs);
    glVertex2f(world.map->leftoffs - Parser.MAX_WIDTH, -world.map->bottomoffs);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    m_xMove.x = m_xMove.y = 0;

}
