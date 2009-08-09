/*   bullet.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawe³ Konieczny <konp84@gmail.com>
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

#include "moving_object.h"
#include "worldmap.h"
//#include "bot.h"


class Bullet : public MovingObject
{
    //friend class Bot;
public:
//list<Bullet *> ammo_list;
    int gunModel;
    unsigned int owner;

    Bullet(float src_x, float src_y, float dest_x, float dest_y, int gunmodel, int owner);
    int collision_det_with_bots();

//private:
  //  float mMass();
    //float mStartSpeed();
};

extern std::list<Bullet *> ammo_list;

#endif
