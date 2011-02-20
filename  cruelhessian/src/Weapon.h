#ifndef WEAPON_H
#define WEAPON_H

/*   Weapon.h
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


#include "Drawable.h"
#include "TVector2D.h"
//#include "Body.h"


class Tex;


class Weapon : public Drawable//, public Body
{
    const Tex& m_xTexture;
    float m_fHalfWidth, m_fHalfHeight;
    int m_iOwner;
    TVector2D m_xPosition;
    float m_fAngle;
	Weapon& operator=(const Weapon&) {return *this;}

public:
    int m_iModel;
    void SetOwner(int own) { m_iOwner = own;}
    void Shot(const TVector2D& aim);

    Weapon(const TVector2D& pos, unsigned int model, unsigned int _owner);
    ~Weapon();

    void Draw() const;
    void Update(const TVector2D& pos, const TVector2D& aim);
    //bool killMyself;
};


#endif
