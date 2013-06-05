/*   Cluster.cpp
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

#include "Cluster.h"
#include "ClusterGrenade.h"
#include "PhysicsManager.h"
#include "WorldMap.h"
#include "AudioManager.h"
#include "WeaponManager.h"
#include "ParserManager.h"
#include "TVector2D.h"
#include "TexturesLoader.h"
//#include "Body.h"
#include <cmath>
#include "ContactListener.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



Cluster::Cluster(const TVector2D& src, const TVector2D& velocity, unsigned int _owner) :
    m_iOwner(_owner),
    currentFrame(0),
    timerChangeFrame(0),
    timerThrow(0),
    m_xTexture(WeaponManager::GetSingleton().text_cluster),
    killMyself(false)
{

    m_fHalfHeight = m_xTexture.h/2.0f;
    m_fHalfWidth = m_xTexture.w/2.0f;

    Set(src, velocity, 8, m_fHalfHeight, m_fHalfWidth, 0.01f);
    SetCollisionCallback(HandleContact);
 //   GetInvInertia() = 0.0f;
 //   GetInertia() = 0.0f;
 //   GetAngVelocity() = 0.0f;
 //   SetOrientation(0.0f);
    type = TYPE_CLUSTER;

}


void Cluster::Update()
{


}

void Cluster::Draw() const
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


void Cluster::ThrowFive()
{
    if (Parser.SOUNDS_VOL > 0)
        Audio.Play(Audio.cluster);

    for (int i = 0; i < 5; ++i)
    {
        //std::cout << "VEC " << GetLinVelocity().x << " "<< GetLinVelocity().y << std::endl;
        TVector2D test(GetLinVelocity().x+10*i, GetLinVelocity().y+10*i);
        ClusterGrenade *cl_gren = new ClusterGrenade(GetPosition(), test, m_iOwner);
		Physics.m_movingObj.push_back(cl_gren);
    }

    killMyself = true;
}
