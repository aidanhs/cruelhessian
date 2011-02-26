/*   Bonus.cpp
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


#include "Bonus.h"
#include "Body.h"
#include "BonusManager.h"
#include "TexturesLoader.h"
#include "TVector2D.h"
#include "ContactListener.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



Bonus::Bonus(const TVector2D& pos, int bontype) :
    m_iTypeBonus(bontype),
    m_xTexture(Bonuses.text_bonus[bontype]),
    killMyself(false)
{

    m_xScale = TVector2D(0.8f, 0.8f);
    m_fHalfWidth = m_xScale.x * m_xTexture.w / 2.0f;
    m_fHalfHeight = m_xScale.y * m_xTexture.h / 2.0f;

    std::vector<TVector2D> axVertices;
    axVertices.resize(4);

    axVertices[0] = TVector2D(-m_fHalfWidth, -m_fHalfHeight);
    axVertices[1] = TVector2D(m_fHalfWidth, -m_fHalfHeight);
    axVertices[2] = TVector2D(m_fHalfWidth, m_fHalfHeight);
    axVertices[3] = TVector2D(-m_fHalfWidth, m_fHalfHeight);

    Set(pos, TVector2D(0,0), axVertices, 0.3f);
    SetCollisionCallback(HandleContact);
    type = TYPE_BONUS;

}


Bonus::~Bonus()
{
//    delete axVertices;
}


void Bonus::Update()
{
//    m_translateX = bod->GetPosition().x;
//    m_translateY = bod->GetPosition().y;
}

void Bonus::Draw() const
{
    glPushMatrix();

    glTranslatef(GetPosition().x, GetPosition().y, 0.0f);
    glRotatef(RadiansToDegrees(GetAngle()), 0, 0, 1);
    glScalef(m_xScale.x, m_xScale.y, 0.0);
    glBindTexture(GL_TEXTURE_2D, m_xTexture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(-m_fHalfWidth, -m_fHalfHeight);
    glTexCoord2i(1, 1);
    glVertex2f(m_fHalfWidth, -m_fHalfHeight);
    glTexCoord2i(1, 0);
    glVertex2f(m_fHalfWidth, m_fHalfHeight);
    glTexCoord2i(0, 0);
    glVertex2f(-m_fHalfWidth, m_fHalfHeight);
    glEnd();

    glPopMatrix();
}
