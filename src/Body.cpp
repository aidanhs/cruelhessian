/*   Body.cpp
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


// based no Polycolly by Olivier Renault

#include <iostream>
#include "Body.h"
#include "physics/Contact.h"
#include <GL/gl.h>
#include "TVector2D.h"
#include "physics/Polygon.h"
#include "TexturesLoader.h"
#include "WorldMap.h"
#include "Map.h"
//#include "physics/ContactFilter.h"


#define ARGB_A(u) (((u)>>24) & 0x000000FF)
#define ARGB_R(u) (((u)>>16) & 0x000000FF)
#define ARGB_G(u) (((u)>> 8) & 0x000000FF)
#define ARGB_B(u) (((u)>> 0) & 0x000000FF)


static inline float wrapf(const float& x, const float& min, const float& max)
{
    return (x < min)? (x - min) + max : (x > max)? (x - max) + min : x;

}

Body::Body() :
    owner(0),
    m_xScale(1.0f, 1.0f)
{

}


void Body::Set(const TVector2D& xPosition, const TVector2D& xVelocity, std::vector<TVector2D>& axVector, float fMass)
{
    m_fptrHandleContact = NULL;

    m_xNetForce    = TVector2D(0.0f, 0.0f);
    m_fNetTorque   = 0.0f;
    m_xPosition    = xPosition;
    m_xVelocity    = xVelocity;
    m_axVertices   = axVector;
    m_fAngVelocity = 0.0f;

    SetOrientation(0.0f);
    SetMass(fMass);
    //  FindAABB();

}



void Body::Set(const TVector2D& xPosition, const TVector2D& xVelocity, int iNumVertices, float rad_x, float rad_y, float fMass)
{
    m_fptrHandleContact = NULL;

    m_xNetForce    = TVector2D(0.0f, 0.0f);
    m_fNetTorque   = 0.0f;
    m_xPosition    = xPosition;
    m_xVelocity    = xVelocity;
    m_axVertices   = PolyColl::BuildBlob(iNumVertices, rad_x, rad_y);
    m_fAngVelocity = 0.0f;

    SetOrientation(0.0f);
    SetMass(fMass);
    //  FindAABB();
}


//void Body::SetMass

void Body::SetOrientation(float fAngle)
{
    m_fOrientation = fAngle;
    m_xOrientation = TMatrix2D(m_fOrientation);
}


void Body::SetMass(float mass)
{

    m_fMass = mass;

    if (m_fMass > 0.0f)
    {
        m_fInvMass	  = 1.0f / m_fMass;
        m_fInertia	  = PolyColl::CalculateInertia(m_axVertices, m_fMass);
        m_fInvInertia = (m_fInertia > 0.0f)? 1.0f / m_fInertia : 0.0f;
    }
    else
    {
        m_fInvMass	  = 0.0f;
        m_fInertia	  = 0.0f;
        m_fInvInertia = 0.0f;
    }
}

Body::~Body()
{
//    if (m_axVertices)
//    delete m_axVertices;

//    m_axVertices = NULL;
    m_axVertices.clear();
//    m_iNumVertices = 0;

    m_xNetForce = TVector2D(0, 0);
    m_fNetTorque = 0.0f;
    m_xVelocity	= TVector2D(0, 0);
}

void Body::AddForce(const TVector2D& F)
{
    // if (!IsUnmovable())
    m_xNetForce += F;
}

void Body::AddForce(const TVector2D& F, const TVector2D& P)
{
    //  if (!IsUnmovable())
    {
        m_xNetForce += F;
        m_fNetTorque -= (P - m_xPosition) ^ F; // SIGN IS WRONG?!?
    }

}

void Body::Update(float dt)
{
    /*  if (IsUnmovable())
      {
          m_xVelocity = TVector2D(0, 0);
          m_fAngVelocity = 0.0f;
          return;
      }
    */
//    pointer->update();
    //-------------------------------------------------------
    // Integrate position (verlet integration)
    //-------------------------------------------------------
    m_xPosition		+= m_xVelocity * dt;
    m_fOrientation	+= m_fAngVelocity * dt;
    m_fOrientation   = wrapf(m_fOrientation, -static_cast<float>(2.0f*M_PI), static_cast<float>(2.0f*M_PI));
    m_xOrientation   = TMatrix2D(m_fOrientation);

    //-------------------------------------------------------
    // Integrate velocity (implicit linear velocity)
    //-------------------------------------------------------
    m_xVelocity		+= m_xNetForce  * (m_fInvMass    * dt);
    m_fAngVelocity	+= m_fNetTorque * (m_fInvInertia * dt);

    //-------------------------------------------------------
    // clear forces
    //-------------------------------------------------------
    m_xNetForce		 = TVector2D(0.0f, 0.0f);
    m_fNetTorque	 = 0.0f;
}


void Body::draw(void)
{
    //m_fptrHandleContact = NULL;
    unsigned int uFill = (IsUnmovable())? 0x808080FF : 0x8080FF80;
    unsigned int uLine = (IsUnmovable())? 0xFF8080FF : 0xFF00A000;

    if (m_axVertices.size() == 1)
    {
        uFill = uLine = 0xFFFF0000;

        glColor4ub(ARGB_R(uLine), ARGB_G(uLine), ARGB_B(uLine), ARGB_A(uLine));
        glEnable(GL_POINT_SIZE);
        glPointSize(3.0f);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_xPosition.x, m_xPosition.y, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(m_axVertices[0].x, m_axVertices[0].y);
        glEnd();
        glPopMatrix();
        glDisable(GL_POINT_SIZE);
        return;
    }

//std::cout << "LE " << m_iNumVertices << std::endl;
    //glColor3f(1.0f, 1.0f, 1.0f);
    glColor4ub(ARGB_R(uLine), ARGB_G(uLine), ARGB_B(uLine), ARGB_A(uLine));
    glPushMatrix();

    glTranslatef(m_xPosition.x, m_xPosition.y, 0.0f);
    glRotatef(RadiansToDegrees(m_fOrientation), 0, 0, -1);
    glBegin(GL_LINE_LOOP);
    for (unsigned int i = 0; i < m_axVertices.size(); i++)
        glVertex2f(m_axVertices[i].x, m_axVertices[i].y);

    glEnd();
    glPopMatrix();
}

bool Body::IntersectSegment(const TVector2D& xStart, const TVector2D& xEnd, float fDist, TVector2D& N, float& t) const
{
    if (m_axVertices.size() <= 2)
        return false;

    TVector2D Nnear;
    TVector2D Nfar;
    float tnear = 0.0f;
    float tfar = fDist;
    bool bIntersect = PolyColl::ClipSegment(m_axVertices,
                                            m_xPosition, m_xVelocity, m_xOrientation,
                                            xStart, xEnd, tnear, tfar, Nnear, Nfar);

    if (bIntersect)
    {
        if (tnear < fDist)
        {
            N = Nnear;
            t = tnear;
            return true;
        }
    }
    return false;
}


bool Body::Collide(Body& xBody, float dt)
{
    //if ((IsUnmovable() && xBody.IsUnmovable()) || !ShouldCollide(this, &xBody))
 //   if (!ShouldCollide(this, &xBody))
   //     return false;

    float  t = dt;
    TVector2D N;

  //  const TVector2D* A  = m_axVertices;
  //  int Anum         = m_iNumVertices;
    const TVector2D& PA = m_xPosition;
    const TVector2D& VA = m_xVelocity;
    TMatrix2D OA        = m_xOrientation;//Matrix::Identity();

   // const TVector2D* B  = xBody.m_axVertices;
   // int Bnum         = xBody.m_iNumVertices;
    const TVector2D& PB = xBody.m_xPosition;
    const TVector2D& VB = xBody.m_xVelocity;
    TMatrix2D OB        = xBody.m_xOrientation;//Matrix::Identity();


    if (PolyColl::Collide(m_axVertices, PA, VA, OA,
                          xBody.m_axVertices, PB, VB, OB,
                          N, t))
    {
        if (m_fptrHandleContact)
        {
            if (!m_fptrHandleContact(N, t, this, &xBody))
                //return true;
                return false;
        }

        if (xBody.m_fptrHandleContact)
        {
            if (!xBody.m_fptrHandleContact(-N, t, &xBody, this))
                //return true;
                return false;
        }

        TVector2D CA[4];
        TVector2D CB[4];
        int Cnum;

        PolyColl::FindContacts( m_axVertices, PA, VA, OA,
                                xBody.m_axVertices, PB, VB, OB,
                                N, t,
                                CA, CB, Cnum);

        CContact xContact(CA, CB, Cnum, N, t, this, &xBody);
        xContact.Solve();
        return true;
    }

    return false;
}

void Body::SetCollisionCallback(bool (*fptrHandleContact)(const TVector2D& N, float& t, Body* pxThisBody, Body* pxOtherBody))
{
    m_fptrHandleContact = fptrHandleContact;
}


void Body::FindAABB()
{
    //TVector2D min, max;
    // int index_min, index_max;
    // index_min = index_max = 0;
    min = max = m_axVertices[0];

    for (unsigned int i = 1; i < m_axVertices.size(); ++i)
    {
        if (m_axVertices[i].x <= min.x)
            min.x = m_axVertices[i].x;
        if (m_axVertices[i].y <= min.y)
            min.y = m_axVertices[i].y;
        if (m_axVertices[i].x >= max.x)
            max.x = m_axVertices[i].x;
        if (m_axVertices[i].y >= max.y)
            max.y = m_axVertices[i].y;

    }

    min += GetPosition()-TVector2D(20, 20);
    max += GetPosition()+TVector2D(20, 20);

  //   std::cout << "Pozycja " << GetPosition().y << ", boxik " << min.y << ", " << max.y << std::endl;

}

