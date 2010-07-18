/*   bullet.cpp
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


#include "Bullet.h"

#include <cmath>


const float _180overpi = 57.29f;


Bullet::Bullet(const TVector2D& src, const TVector2D& dest, unsigned int gunmodel, unsigned int _owner, float speed, const Tex& tex) : texture(tex)
{
    type = XPOINT;
    position = src;
    w = texture.w;
    h = texture.h;
    gunModel = gunmodel;
    owner = _owner;
    mass = 100;
    massInv = 1 / mass;
    maxSpeed = TVector2D(10000, 10000);

    float tang = (dest.y - src.y) / (dest.x - src.x);
    float sq = 8 * speed / sqrt(1+tang*tang);

    velocity = (dest.x - src.x > 0) ? TVector2D(sq, sq * tang) : TVector2D(-sq, -sq * tang);
    //  velocity = TVector2D(0.0f, 0.0f);
//old_position = position;
    old_a = TVector2D(0,0);
}


void Bullet::draw() const
{

    glPushMatrix();

    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(_180overpi * atan(velocity.y / velocity.x), 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(texture.w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, texture.h);
    glEnd();
    glPopMatrix();

}