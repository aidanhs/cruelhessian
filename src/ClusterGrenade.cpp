/*   ClusterGrenade.cpp
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

#include "ClusterGrenade.h"
#include "ParserManager.h"
#include "WorldMap.h"
#include "AudioManager.h"
#include "WeaponManager.h"
#include "TVector2D.h"
#include "TexturesLoader.h"
#include "WorldMap.h"
#include <cmath>
#include "ContactListener.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



ClusterGrenade::ClusterGrenade(const TVector2D& src, const TVector2D& velocity, unsigned int _owner) :
    m_iOwner(_owner),
    m_iCurrentFrame(0),
    m_fTimerChangeFrame(0.0f),
    m_fTimerThrow(world.getCurrentTime),
    m_bPlayOnlyOnce(true),
    killMyself(false),
    m_iBouncingCount(0)
{

    m_xTexture = &WeaponManager::GetSingleton().text_clustergrenade[0];

    m_fHalfHeight = m_xTexture->h/2.0f;
    m_fHalfWidth = m_xTexture->w/2.0f;

    Set(src, velocity, 8, m_fHalfHeight, m_fHalfWidth, 0.01f);
    SetCollisionCallback(HandleContact);
    GetInvInertia() = 0.0f;
    GetInertia() = 0.0f;
    GetAngVelocity() = 0.0f;
    SetOrientation(0.0f);
    type = TYPE_CLUSTERGRENADE;

}


void ClusterGrenade::Update()
{

    if (m_iBouncingCount >= 2)
    {
        // explode if touch something
        if (Parser.SOUNDS_VOL > 0)
        {
            Audio.Play(Audio.cluster_explosion);
            m_bPlayOnlyOnce = false;
        }

        if (world.getCurrentTime - m_fTimerChangeFrame >= 20)
        {
            m_fTimerChangeFrame = world.getCurrentTime;
            if (m_iCurrentFrame < 16)
            {
                ++m_iCurrentFrame;
                m_xTexture = &WeaponManager::GetSingleton().text_clustergrenade[m_iCurrentFrame];
                m_fHalfHeight = m_xTexture->h/2.0f;
                m_fHalfWidth = m_xTexture->w/2.0f;
            }
            // remove from the list
            else
            {
                //std::cout << "DF\n";
                killMyself = true;
            }
        }

    }


}

void ClusterGrenade::Draw() const
{
    glPushMatrix();

    glTranslatef(GetPosition().x, GetPosition().y, 0.0f);
    glRotatef(RadiansToDegrees(GetAngle()), 0, 0, 1);
    glBindTexture(GL_TEXTURE_2D, m_xTexture->tex);

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

