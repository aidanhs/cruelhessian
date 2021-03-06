/*   Arrow.cpp
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


#include "Arrow.h"
#include "InterfaceBaseManager.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


Arrow::Arrow() :
    m_xTexture(InterfaceBase.text_arrow),
    m_xPosition(0.0f, 0.0f)
{
}


void Arrow::Update(const TVector2D& bot_pos)
{
    m_xPosition.x = bot_pos.x;
    m_xPosition.y = bot_pos.y - 50;
}


void Arrow::Draw() const
{
	glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(m_xPosition.x, m_xPosition.y, 0.0f);
    glBindTexture(GL_TEXTURE_2D, m_xTexture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2i(0, 0);
    glTexCoord2i(1, 1);
    glVertex2i(m_xTexture.w, 0);
    glTexCoord2i(1, 0);
    glVertex2i(m_xTexture.w, m_xTexture.h);
    glTexCoord2i(0, 0);
    glVertex2i(0, m_xTexture.h);
    glEnd();

    glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

