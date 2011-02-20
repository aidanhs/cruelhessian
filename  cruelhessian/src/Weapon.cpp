/*   Weapon.cpp
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


// based on openSoldat code
// by Gregor A. Cieslak <gregor.a.cieslak@gmail.com>


#include <cmath>

#include "Weapon.h"
#include "TexturesLoader.h"
#include "WeaponManager.h"
#include "Bullet.h"
#include "PhysicsManager.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



Weapon::Weapon(const TVector2D& pos, unsigned int model, unsigned int _owner) :
    m_xTexture(Weapons[model].textureGun),
    m_xPosition(pos),
    m_iOwner(_owner),
    m_iModel(model),
    m_fAngle(0.0f),
    m_fHalfWidth(m_xTexture.w / 2.0f),
    m_fHalfHeight(m_xTexture.h / 2.0f)
{

}


Weapon::~Weapon()
{
//    delete axVertices;
}



void Weapon::Update(const TVector2D& pos, const TVector2D& aim)
{
    m_xPosition = pos;
    float divider = (aim.y - m_xPosition.y) / (aim.x - m_xPosition.x);
    float inverted = (aim.x < m_xPosition.x) ? 180.0f : 0.0f;    // if mouse on left add 180 degrees
    m_fAngle = atan(divider) * RAD_TO_DEG + inverted;
}


void Weapon::Draw() const
{
    glPushMatrix();

    glTranslatef(m_xPosition.x, m_xPosition.y, 0.0f);
    glRotatef(m_fAngle, 0, 0, 1);
    glScalef(0.7f, 0.7f, 0.0);
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


void Weapon::Shot(const TVector2D& aim)
{

	float divider = (aim.y - m_xPosition.y) / (aim.x - m_xPosition.x);
    float inverted = (aim.x < m_xPosition.x) ? 180.0f : 0.0f;    // if mouse on left add 180 degrees
    m_fAngle = atan(divider) * RAD_TO_DEG + inverted;

    const float cosa = cos(m_fAngle * DEG_TO_RAD);
    const float sina = sin(m_fAngle * DEG_TO_RAD);

    TVector2D p = TVector2D(cosa, sina);
    TVector2D v = p*Weapons[m_iModel].speed*3;

    p += m_xPosition;

    Bullet* bullet = new Bullet(p, v, m_fAngle* DEG_TO_RAD, m_iModel, m_iOwner);

	Physics.m_movingObj.push_back(bullet);
}
