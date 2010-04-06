/*   bonus.cpp
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


#include "bonus.h"
#include "globals.h"
#include "SDL_opengl.h"


Bonus::Bonus(const TVector2D& dest, int type, const Tex& tex, BONUS bontype) : texture(tex), typeBonus(bontype)
{

    type = SQUARE;
    scaleX = scaleY = 0.7f;
   // typeBonus = _type;
    position = dest;
    w = scaleX*texture.w;
    h = scaleY*texture.h;
    r = w/2;
    mass = 10;
    massInv = 1 / mass;
    maxSpeed = TVector2D(10000, 10000);
    velocity = TVector2D(0.0f, 0.0f);
   // texture = text;

}


void Bonus::draw() const
{

    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glScalef(scaleX, scaleY, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(texture.w, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(texture.w, texture.h);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, texture.h);
    glEnd();

    glPopMatrix();

}


void Bonus::update()
{
    gravity();
}

