/*   gui_graphics.cpp
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

#include <iostream>
//#include <locale.h>
//#include <libintl.h>

#include "gui.h"
#include "globals.h"

//#define _(string) gettext(string)



bool GUI::onGraphicsChanged(const CEGUI::EventArgs& )
{
    if (mDeep16->isSelected())
        MAX_BPP = 16;
    else if (mDeep32->isSelected())
        MAX_BPP = 32;

    if (mResol640->isSelected())
    {
        MAX_WIDTH = 640.0f;
        MAX_HEIGHT = 480.0f;
    }
    else if (mResol800->isSelected())
    {
        MAX_WIDTH = 800.0f;
        MAX_HEIGHT = 600.0f;
    }
    else if (mResol1024->isSelected())
    {
        MAX_WIDTH = 1024.0f;
        MAX_HEIGHT = 768.0f;
    }

    glViewport(0, 0, static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, MAX_WIDTH, MAX_HEIGHT, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mIsFullscreen->isSelected())
    {
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
        FULLSCREEN = true;
    }
    else
    {
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);
        FULLSCREEN = false;
    }

    return true;
}

