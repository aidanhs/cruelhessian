/*   Mouse.cpp
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


#include "Mouse.h"
#include "ParserManager.h"
#include "Game.h"
#include "InterfaceBaseManager.h"
#include <SFML/Graphics.hpp>
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif
#include <GL/glu.h>


Mouse::Mouse() :
    texture(InterfaceBase.text_mouse),
    mGlobalPos(TVector2D(Parser.MAX_WIDTH/2, Parser.MAX_HEIGHT/2)),
    mLocalPos(TVector2D(Parser.MAX_WIDTH/2, Parser.MAX_HEIGHT/2))
{

}


void Mouse::Draw() const
{

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(getLocalX(), getLocalY(), 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2i(0, 0);
    glTexCoord2i(1, 1);
    glVertex2i(texture.w, 0);
    glTexCoord2i(1, 0);
    glVertex2i(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2i(0, texture.h);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}


TVector2D Mouse::getGlobalPosition(void) const
{
    return mGlobalPos;
}

TVector2D Mouse::getLocalPosition(void) const
{
    return mLocalPos;
}


float Mouse::getGlobalX(void) const
{
    return mGlobalPos.x;
}
/*
float Mouse::getGlobalY() const
{
    return mGlobalPos.y;
}
*/

float Mouse::getLocalX(void) const
{
    return mLocalPos.x;
}

float Mouse::getLocalY(void) const
{
    return mLocalPos.y;
}

void Mouse::Update()
{

    const sf::Input& Input = game.App.GetInput();

    mRelative.x = mLocalPos.x - Input.GetMouseX();
    mRelative.y = mLocalPos.y - Input.GetMouseY();

    mLocalPos.x = static_cast<float>(Input.GetMouseX());
    mLocalPos.y = static_cast<float>(Input.GetMouseY());


    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY;
    GLdouble winZ = 0.0f;
    GLdouble posZ = 0;
    GLdouble tempx = 0.0f;
    GLdouble tempy = 0.0f;
//    int tx, ty;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = mLocalPos.x;
    winY = static_cast<float>(viewport[3]) - mLocalPos.y;
    glReadPixels(static_cast<GLint>(mLocalPos.x), static_cast<GLint>(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &tempx, &tempy, &posZ);

    mGlobalPos.x = static_cast<float>(tempx);
    mGlobalPos.y = static_cast<float>(tempy);

  //  std::cout << "GLOBAL " << mGlobalPos.x << ", " << mGlobalPos.y << std::endl;

}
