/*   InterfaceElement.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2011 by Paweł Konieczny <konp84 at gmail.com>
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

#include "InterfaceElement.h"



InterfaceElement::InterfaceElement() :
    x(0),
    y(0),
    width(0),
    height(0),
    type(BAR_HORIZONTAL),
    angle(0),
    visible(true)
{

}



void InterfaceElement::Draw() const
{
  //  if (visible)
    {
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();

        glLoadIdentity();
        glTranslatef(x, y, 0.0f);
        glRotatef(-angle, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, texture.tex);

        glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex2i(0, 0);
        glTexCoord2i(1, 1);
        glVertex2i(width, 0);
        glTexCoord2i(1, 0);
        glVertex2i(width, height);
        glTexCoord2i(0, 0);
        glVertex2i(0, height);
        glEnd();

        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }

}
