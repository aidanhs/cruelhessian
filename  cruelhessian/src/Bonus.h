#ifndef BONUS_H
#define BONUS_H

/*   Bonus.h
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
#include "Body.h"



class Tex;
class TVector2D;


class Bonus : public Drawable, public Body
{
    const Tex& m_xTexture;
    float m_fHalfWidth, m_fHalfHeight;
	Bonus& operator=(const Bonus&) {}

public:

    Bonus(const TVector2D& pos, int _type);
    ~Bonus();

    int m_iTypeBonus;
    void Update();
    bool killMyself;
    void Draw() const;
};


#endif

