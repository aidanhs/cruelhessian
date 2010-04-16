/*   bullet.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84 at gmail.com>
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

#ifndef BULLET_H
#define BULLET_H

#include "globals.h"
#include "moving_object.h"


class Bullet : public MovingObject
{
    const Tex& texture;
public:
    unsigned int gunModel;   ///< Model of gun


    /**
     *
     * Constructor
     *
     * @param  src Start point
     * @param  dest Destination point
     * @param  gunmodel Gun model
     * @param  _owner Snipper number
     */
    Bullet(const TVector2D& src, const TVector2D& dest, unsigned int gunmodel, unsigned int _owner, const Tex& tex);

    void draw() const;
    void update();
};


#endif
