/*   grenade.h
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

#ifndef GRENADE_H
#define GRENADE_H

#include "globals.h"
#include "moving_object.h"


class Grenade : public MovingObject
{

    const Tex* texture[17];

public:
    int currentFrame;           ///< Counter of the current frame
    Uint32 timer_change_frame;  ///< Time when the previous frame was changed
    Uint32 timer_throw;         ///< Time when the grenade was thrown
    //float r;
    bool killMyself;

    /**
     *
     * Constructor
     *
     * @param  src start point
     * @param  dest destination point
     * @param  owner owner of the grenade
     * @param  push_time time when grenade was pushed
     */
    Grenade(const TVector2D& src, const TVector2D& dest, unsigned int _owner, Uint32 push_time, const Tex* text);

    void draw() const;
    void update();

};


#endif
