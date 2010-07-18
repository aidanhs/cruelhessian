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

#include "../GUI.h"
//#include "Globals.h"
#include "../Game.h"

//#define _(string) gettext(string)



bool GUI::onGraphicsChanged(const CEGUI::EventArgs& )
{
    if (mDeep16->isSelected())
        game.MAX_BPP = 16;
    else if (mDeep32->isSelected())
        game.MAX_BPP = 32;

    if (mResol640->isSelected())
    {
        game.MAX_WIDTH = 640.0f;
        game.MAX_HEIGHT = 480.0f;
    }
    else if (mResol800->isSelected())
    {
        game.MAX_WIDTH = 800.0f;
        game.MAX_HEIGHT = 600.0f;
    }
    else if (mResol1024->isSelected())
    {
        game.MAX_WIDTH = 1024.0f;
        game.MAX_HEIGHT = 768.0f;
    }

    glViewport(0, 0, static_cast<int>(game.MAX_WIDTH), static_cast<int>(game.MAX_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, game.MAX_WIDTH, game.MAX_HEIGHT, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mIsFullscreen->isSelected())
    {
        game.screen = SDL_SetVideoMode(static_cast<int>(game.MAX_WIDTH), static_cast<int>(game.MAX_HEIGHT), game.MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
        game.FULLSCREEN = true;
    }
    else
    {
        game.screen = SDL_SetVideoMode(static_cast<int>(game.MAX_WIDTH), static_cast<int>(game.MAX_HEIGHT), game.MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);
        game.FULLSCREEN = false;
    }

    return true;
}

