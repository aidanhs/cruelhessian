/*   Contact.h
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
// File          : Contact.h
//
// Created by    : OR - 07/02/2004 12:42:04
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




#ifndef CONTACT_H
#define CONTACT_H

#include "../TVector2D.h"
#include "Material.h"

class CContact
{
    CMaterial s_xContactMaterial;
public:
    enum { eMaxContacts = 2 };

    class Body* m_pxBodies [2];
    TVector2D       m_xContacts[eMaxContacts][2];
    TVector2D		 m_xNormal;
    float		 m_t;
    int			 m_iNumContacts;

    CContact();

    CContact(const TVector2D* CA, const TVector2D* CB, int iCnum,
             const TVector2D& N, float t,
             Body* pxBodyA, Body* pxBodyB);

    class Body* GetBody(int i)
    {
        return m_pxBodies[i];
    }

    void Solve();

private:
    void ResolveCollision();
    void ResolveCollision(const TVector2D& CA, const TVector2D& CB);
    void ResolveCollision(const TVector2D& Ncoll, float t, float fCoF, float fCoR,
                          const TVector2D& C0, const TVector2D& P0, TVector2D& V0, float& w0, float m0, float i0,
                          const TVector2D& C1, const TVector2D& P1, TVector2D& V1, float& w1, float m1, float i1);

    void ResolveOverlap();
    void ResolveOverlap(const TVector2D& CA, const TVector2D& CB);
    void AddContactPair(const TVector2D& CA, const TVector2D& CB);
};


#endif
