/*   Bullet.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at mail.com>
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


#include <cmath>

#include "Bullet.h"
#include "Body.h"
#include "TexturesLoader.h"
#include "TVector2D.h"
#include "WeaponManager.h"
#include "ContactListener.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



Bullet::Bullet(const TVector2D& src, const TVector2D& velocity, float angle, unsigned int gunmodel, unsigned int _owner) :
    m_iOwner(_owner),
    m_iGunModel(gunmodel),
    m_xTexture(Weapons[gunmodel].textureAmmo),
    killMyself(false)
{
    m_fHalfWidth = m_xTexture.w/2.0f;
    m_fHalfHeight = m_xTexture.h/2.0f;

    std::vector<TVector2D> axVertices;
    axVertices.resize(2);

    axVertices[0] = TVector2D(-m_fHalfWidth, 0.0f);
    axVertices[1] = TVector2D(m_fHalfWidth, 0.0f);

    Set(src, velocity, axVertices, 0.01f);
    SetOrientation(angle);
    SetCollisionCallback(HandleContact);
    type = TYPE_BULLET;

}


Bullet::~Bullet()
{
//    delete axVertices;
}



void Bullet::Update()
{
  //  m_translateX = position.x;
  //  m_translateY = position.y;
  //  m_rotate = _180overpi * atan(velocity.y / velocity.x);
}

void Bullet::Draw() const
{
    glPushMatrix();

    glTranslatef(GetPosition().x, GetPosition().y, 0.0f);
    glRotatef(RadiansToDegrees(GetAngle()), 0, 0, 1);
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

