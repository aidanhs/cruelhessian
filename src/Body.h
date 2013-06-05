#ifndef BODY_H
#define BODY_H


/*   Body.h
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


#include "Enums.h"
#include "TMatrix2D.h"
#include "TexturesLoader.h"
#include "TVector2D.h"
#include <set>
#include <vector>



class Body
{

    void SetMass(float mass);

public:
   // unsigned int number;
    TVector2D min, max;
    //void *pointer;
    virtual void draw(void);
    Tex m_currentTexture;
    TVector2D m_xScale;
  //  float m_fRotate;
    BODY_TYPE type;
    unsigned int owner;
  //  std::set<unsigned int> whichSectors();
    void FindAABB();
    //unsigned int whichSectors();

    Body();
    //Body(const TVector2D& xPosition, float rad_x, float rad_y, float fMass);
    void Set(const TVector2D& xPosition, const TVector2D& xVelocity, int num_vert, float rad_x, float rad_y, float fMass);
    void Set(const TVector2D& xPosition, const TVector2D& xVelocity, std::vector<TVector2D>& vec, float fMass);

    ~Body();

    void AddForce(const TVector2D& F);

    void AddForce(const TVector2D& F, const TVector2D& P);

    bool Collide(Body& Body, float dt);

    bool IntersectSegment(const TVector2D& Start, const TVector2D& End, float fDist, TVector2D& N, float& t) const;

    void Update(float dt);

    //void Render() const;

    void SetOrientation(float fAngle);


    bool IsUnmovable() const
    {
        return (m_fMass < 0.0001f);
    }
    TVector2D& GetPosition		()
    {
        return m_xPosition;
    }
    TVector2D& GetLinVelocity	()
    {
        return m_xVelocity;
    }
    float&  GetAngVelocity  ()
    {
        return m_fAngVelocity;
    }
    float&  GetAngle  ()
    {
        return m_fOrientation;
    }

        float GetAngle() const
    {
        return m_fOrientation;
    }

    float&  GetMass			()
    {
        return m_fMass;
    }
    float&  GetInvMass		()
    {
        return m_fInvMass;
    }
    float&  GetInertia		()
    {
        return m_fInertia;
    }
    float&  GetInvInertia	()
    {
        return m_fInvInertia;
    }

    const TVector2D& GetPosition	() const
    {
        return m_xPosition;
    }
    const TVector2D& GetLinVelocity() const
    {
        return m_xVelocity;
    }
    float GetAngVelocity		() const
    {
        return m_fAngVelocity;
    }
    float GetMass				() const
    {
        return m_fMass;
    }
    float GetInvMass			() const
    {
        return m_fInvMass;
    }
    float GetInvInertia			() const
    {
        return m_fInvInertia;
    }

    void SetCollisionCallback(bool (*fptrHandlePlayerContact)(const TVector2D& N, float& t, Body* pxThisBody, Body* pxOtherBody));

protected:
    void ProcessCollision(Body& xBody, const TVector2D& N, float t);
    void ProcessOverlap  (Body& xBody, const TVector2D& MTD);

    std::vector<TVector2D> m_axVertices;
    //int     m_iNumVertices;

    TVector2D	m_xVelocity;
    TVector2D	m_xPosition;

    //float	m_fDensity;
    float	m_fMass;
    float   m_fInertia;
    float	m_fInvMass;
    float   m_fInvInertia;

    float m_fOrientation;
    float m_fAngVelocity;
    TMatrix2D m_xOrientation;

    TVector2D m_xNetForce;
    float  m_fNetTorque;

    bool (*m_fptrHandleContact)(const TVector2D& N, float& t, Body* pxThisBody, Body* pxOtherBody);

};



#endif
