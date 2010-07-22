#ifndef BULLET_H
#define BULLET_H

/*   Bullet.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at mail.com>
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


#include "Tex.h"
#include "TVector2D.h"
#include "MovingObject.h"


class Bullet : public MovingObject
{
    const Tex& texture;

public:

    unsigned int gunModel;

    Bullet(const TVector2D& src, const TVector2D& dest, unsigned int gunmodel, unsigned int _owner, float speed, const Tex& tex);

    void draw() const;

};


#endif
