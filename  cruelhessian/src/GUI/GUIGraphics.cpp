/*   GUIGraphics.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawel Konieczny <konp84 at gmail.com>
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

#include "../GUI.h"
#include "../Game.h"



bool GUI::onGraphicsChanged(const CEGUI::EventArgs& )
{
    if (mDeep16->isSelected())
        Parser.MAX_BPP = 16;
    else if (mDeep32->isSelected())
        Parser.MAX_BPP = 32;

    if (mResol640->isSelected())
    {
        Parser.MAX_WIDTH = 640.0f;
        Parser.MAX_HEIGHT = 480.0f;
    }
    else if (mResol800->isSelected())
    {
        Parser.MAX_WIDTH = 800.0f;
        Parser.MAX_HEIGHT = 600.0f;
    }
    else if (mResol1024->isSelected())
    {
        Parser.MAX_WIDTH = 1024.0f;
        Parser.MAX_HEIGHT = 768.0f;
    }

    glViewport(0, 0, static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Parser.MAX_WIDTH, Parser.MAX_HEIGHT, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mIsFullscreen->isSelected())
    {
        game.screen = SDL_SetVideoMode(static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT), Parser.MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
        Parser.FULLSCREEN = true;
    }
    else
    {
        game.screen = SDL_SetVideoMode(static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT), Parser.MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);
        Parser.FULLSCREEN = false;
    }

    return true;
}

