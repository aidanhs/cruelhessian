/*   bonus.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84 at mail.com>
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

#ifndef BONUS_H
#define BONUS_H

#include "globals.h"
#include "moving_object.h"


class Bonus : public MovingObject
{
    const Tex& texture;
    float scaleX, scaleY;

public:

    int typeBonus;

    Bonus(const TVector2D& pos, const Tex& tex, int _type);
    ~Bonus() {};

    void draw() const;
    //void AccumulateForces();
};


#endif
