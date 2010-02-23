/*   grenade.cpp
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

#include "grenade.h"
#include "globals.h"
#include <cmath>



Grenade::Grenade(const TVector2D& src, const TVector2D& dest, unsigned int _owner, Uint32 push_time, Tex* tex)
{
    type = CIRCLE;
    position = src;
    owner = _owner;
    timer_change_frame = 0;
    currentFrame = 0;
    mass = 5;
    massInv = 1 / mass;
    maxSpeed = TVector2D(100, 100);

    float tang = (dest.y - src.y) / (dest.x - src.x);
    float sq = push_time / sqrt(1+tang*tang);
    velocity = (dest.x - src.x > 0) ? TVector2D(-sq, -sq * tang) : TVector2D(sq, sq * tang);

    for (int i = 0; i < 17; ++i)
    {
        texture[i] = &tex[i];
    }
    //w = texture[0]->w;
    //h = texture[0]->h;
    w = h = 0;
    r = texture[0]->w / 2;
    killMyself = false;

    // old_position = position;

    timer_throw = getCurrentTime;
}


void Grenade::draw() const
{
    glPushMatrix();
    glTranslatef(position.x - texture[currentFrame]->w/2,
                 position.y - texture[currentFrame]->h/2, 0.0);

    glBindTexture(GL_TEXTURE_2D, texture[currentFrame]->tex);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(texture[currentFrame]->w, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(texture[currentFrame]->w, texture[currentFrame]->h);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, texture[currentFrame]->h);
    glEnd();

    glPopMatrix();
}


void Grenade::update()
{

    if (getCurrentTime - timer_throw > 4000)
    {
        // explode after 4 second
        if (SOUNDS_VOL > 0)
            Mix_PlayChannel(-1, grenade_explosion, 0);

        if (getCurrentTime - timer_change_frame >= 20)
        {
            timer_change_frame = getCurrentTime;
            if (currentFrame < 16)
            {
                ++currentFrame;
            }
            // remove grenade from the list
            else
            {
                killMyself = true;
            }
        }

    }

    gravity();

}
