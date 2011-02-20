#ifndef CLUSTER_H
#define CLUSTER_H

/*   Cluster.h
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


#include "Drawable.h"
#include "Body.h"


class Tex;
class TVector2D;

class Cluster : public Drawable, public Body
{

    const Tex& m_xTexture;
    unsigned int currentFrame;           ///< Counter of the current frame
    float timerChangeFrame;  ///< Time when the previous frame was changed
    float timerThrow;         ///< Time when the grenade was thrown
    float m_fHalfWidth;
	float m_fHalfHeight;
	Cluster& operator=(const Cluster&) {return *this;}

public:
    unsigned int m_iOwner;
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
    Cluster(const TVector2D& src, const TVector2D& velocity, unsigned int _owner);
    ~Cluster() {}

    void Draw() const;
    void Update();
    void ThrowFive();

};


#endif
