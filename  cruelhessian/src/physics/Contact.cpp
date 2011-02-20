/*   Contact.cpp
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


//------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File          : Contact.cpp
//
// Created by    : OR - 07/02/2004 12:42:27
//
// Copyright (C) : 2004 Rebellion, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description   :
// ---------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------


#include "Contact.h"
#include "Material.h"
#include "../Body.h"
#include <iostream>


static inline float clampf(float x, float min, float max)
{
	return (x < min)? min : (x > max)? max : x;
}




////////////////////////////////////////////////////////////////
// ResolveCollision
////////////////////////////////////////////////////////////////
// calculates the change in angular and linear velocity of two
// colliding objects
////////////////////////////////////////////////////////////////
// parameters :
// ------------
// Ncoll : normal of collision
// t : time of collision, (if negative, it's a penetration depth)
// fCoF : coefficient of friction
// fCoR : coefficient of restitution
// C0 : point of contact on object 0
// P0 : centre of gravity (position) of object 0
// V0 : linear Velocity of object 0
// w0 : angular velocity of object 0
// m0 : inverse mass of object 0
// i0 : inverse inertia of object 0
// C1 : point of contact on object 1
// P1 : centre of gravity (position) of object 1
// V1 : linear Velocity of object 1
// w1 : angular velocity of object 1
// m1 : inverse mass of object 1
// i1 : inverse inertia of object 1
//
// return values : V0, w0, V1, w1 will change upon a collision
// -------------
///////////////////////////////////////////////////////////////
void CContact::ResolveCollision(const TVector2D& Ncoll, float t, float fCoF, float fCoR,
                                const TVector2D& C0, const TVector2D& P0, TVector2D& V0, float& w0, float m0, float i0,
                                const TVector2D& C1, const TVector2D& P1, TVector2D& V1, float& w1, float m1, float i1)
{
    //------------------------------------------------------------------------------------------------------
    // pre-computations
    //------------------------------------------------------------------------------------------------------
    float tcoll = (t > 0.0f)? t : 0.0f;

    TVector2D Q0    = P0 + V0 * tcoll;
    TVector2D Q1    = P1 + V1 * tcoll;
    TVector2D R0    = C0 - Q0;
    TVector2D R1    = C1 - Q1;
    TVector2D T0    = TVector2D(-R0.y, R0.x);
    TVector2D T1    = TVector2D(-R1.y, R1.x);
    TVector2D VP0   = V0 - T0 * w0; // point velocity (SIGN IS WRONG)
    TVector2D VP1   = V1 - T1 * w1; // point velocity (SIGN IS WRONG)

    //------------------------------------------------------------------------------------------------------
    // impact velocity
    //------------------------------------------------------------------------------------------------------
    TVector2D Vcoll = VP0 - VP1;
    float  vn	 = Vcoll * Ncoll;
    TVector2D Vn	 = vn    * Ncoll;
    TVector2D Vt	 = Vcoll - Vn;

    // separation
    if (vn > 0.0f)
        return;

    float  vt = Vt.normalize();


    //------------------------------------------------------------------------------------------------------
    // compute impulse (frction and restitution).
    // ------------------------------------------
    //
    //									-(1+Cor)(Vel.norm)
    //			j =  ------------------------------------------------------------
    //			     [1/Ma + 1/Mb] + [Ia' * (ra x norm)²] + [Ib' * (rb x norm)²]
    //------------------------------------------------------------------------------------------------------
    TVector2D J;
    TVector2D Jt(0.0f, 0.0f);
    TVector2D Jn(0.0f, 0.0f);

    float t0 = (R0 ^ Ncoll) * (R0 ^ Ncoll) * i0;
    float t1 = (R1 ^ Ncoll) * (R1 ^ Ncoll) * i1;
    float m  = m0 + m1;

    float denom = m + t0 + t1;

    float jn = vn / denom;

    Jn = (-(1.0f + fCoR) * jn) * Ncoll;

//	if (dbg_UseFriction)
    {
        Jt = (fCoF * jn) * Vt.Direction();
    }

    J = Jn + Jt;

    //------------------------------------------------------------------------------------------------------
    // changes in momentum
    //------------------------------------------------------------------------------------------------------
    TVector2D dV0 = J * m0;
    TVector2D dV1 =-J * m1;

    float dw0 =-(R0 ^ J) * i0; // (SIGN IS WRONG)
    float dw1 = (R1 ^ J) * i1; // (SIGN IS WRONG)

    //------------------------------------------------------------------------------------------------------
    // apply changes in momentum
    //------------------------------------------------------------------------------------------------------
    if (m0 > 0.0f) V0 += dV0;
    if (m1 > 0.0f) V1 += dV1;
    if (m0 > 0.0f) w0 += dw0;
    if (m1 > 0.0f) w1 += dw1;

    //------------------------------------------------------------------------------------------------------
    // Check for static frcition
    //------------------------------------------------------------------------------------------------------
    if (vn < 0.0f && fCoF > 0.0f)
    {
        float cone = -vt / vn;

        if (cone < fCoF)
        {
            // treat static friciton as a collision at the contact point
            TVector2D Nfriction = -Vt.Direction();
            float fCoS = s_xContactMaterial.GetStaticFriction();

            ResolveCollision(Nfriction, 0.0f, 0.0f, fCoS,
                             C0, P0, V0, w0, m0, i0,
                             C1, P1, V1, w1, m1, i1);
        }
    }
}

CContact::CContact()
{

    m_pxBodies[0] = m_pxBodies[1] = NULL;
    m_iNumContacts = 0;
}


CContact::CContact(const TVector2D* CA, const TVector2D* CB, int Cnum,
                   const TVector2D& N, float t,
                   Body* pxBodyA, Body* pxBodyB)
{
    m_iNumContacts = 0;
    m_pxBodies[0]  = pxBodyA;
    m_pxBodies[1]  = pxBodyB;
    m_xNormal	   = N;
    m_t			   = t;

    for(int i = 0; i < Cnum; i ++)
    {
        AddContactPair(CA[i], CB[i]);
    }
}


void CContact::AddContactPair(const TVector2D& CA, const TVector2D& CB)
{
    if (m_iNumContacts >= eMaxContacts)
        return;

    m_xContacts[m_iNumContacts][0] = CA;
    m_xContacts[m_iNumContacts][1] = CB;
    m_iNumContacts++;
}

void CContact::Solve()
{
    if (m_t < 0.0f)
        ResolveOverlap();

    ResolveCollision();
}

void CContact::ResolveOverlap()
{
    if (!m_pxBodies[0] || !m_pxBodies[1])
        return;

    for(int i = 0; i < m_iNumContacts; i ++)
    {
        ResolveOverlap(m_xContacts[i][0], m_xContacts[i][1]);
    }
}

void CContact::ResolveOverlap(const TVector2D& C0, const TVector2D& C1)
{

    float m0 = m_pxBodies[0]->GetInvMass();
    float m1 = m_pxBodies[1]->GetInvMass();
    float m  = m0 + m1;

    TVector2D D = C1 - C0;
    float fRelaxation = s_xContactMaterial.GetSeparation();

    D *= fRelaxation;
    TVector2D D0 = TVector2D(0.0f, 0.0f);
    TVector2D D1 = TVector2D(0.0f, 0.0f);

    if (m0 > 0.0f)
    {
        D0 = D * (m0 / m);
        m_pxBodies[0]->GetPosition() += D0;
    }
    if (m1 > 0.0f)
    {
        D1 = D * -(m1 / m);
        m_pxBodies[1]->GetPosition() += D1;
    }
}

void CContact::ResolveCollision()
{
    if (!m_pxBodies[0] || !m_pxBodies[1])
        return;

    for (int i = 0; i < m_iNumContacts; ++i)
    {
        ResolveCollision(m_xContacts[i][0], m_xContacts[i][1]);
    }
}




void CContact::ResolveCollision(const TVector2D& C0, const TVector2D& C1)
{
    float m0 = m_pxBodies[0]->GetInvMass();
    float m1 = m_pxBodies[1]->GetInvMass();
    float i0 = m_pxBodies[0]->GetInvInertia();
    float i1 = m_pxBodies[1]->GetInvInertia();

    const TVector2D& P0	= m_pxBodies[0]->GetPosition();
    const TVector2D& P1	= m_pxBodies[1]->GetPosition();
    TVector2D& V0		= m_pxBodies[0]->GetLinVelocity();
    TVector2D& V1		= m_pxBodies[1]->GetLinVelocity();
    float&  w0			= m_pxBodies[0]->GetAngVelocity();
    float&  w1			= m_pxBodies[1]->GetAngVelocity();

//	float fCoR  = s_xContactMaterial.GetRestitution();
    //float fCoF  = s_xContactMaterial.GetFriction();

    ResolveCollision(-m_xNormal, m_t, s_xContactMaterial.GetFriction(), s_xContactMaterial.GetRestitution(),
                     C1, P1, V1, w1, m1, i1,
                     C0, P0, V0, w0, m0, i0);

//	Render();
}
