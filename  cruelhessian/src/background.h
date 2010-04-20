/*   backgroung.h
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

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "tvector2d.h"
#include "map.h"


class Background
{

    TVector2D move, old_pos;
    bool flag;

public:

    Background(float startX, float startY, const TVector2D& bot_pos);
    ~Background() {};

    void setPosition(float startX, float startY);
    void draw(const Map& map);
    void update(const TVector2D& mouse_pos, const TVector2D& bot_pos);
};


#endif
