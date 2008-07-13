/*   draw_extra.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84@gmail.com>
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

#ifndef DRAW_EXTRA_H
#define DRAW_EXTRA_H

#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include <iostream>
//void Load_Textures();

void draw_int_help(GLuint tex, GLfloat *xtex, float dx, float dy);
//int reload_gun(void *data);
typedef struct
{
    GLdouble x, y;
} GLDPOINT;

//GLuint SDL_GL_LoadTexture(const char *file, GLfloat *tex);
GLuint SDL_GL_LoadTexture(std::string file, GLfloat *tex);
//GLuint SDL_GL_LoadFont(TTF_Font *font, const char *text, SDL_Color color, GLfloat *tex);
GLuint SDL_GL_LoadFont(TTF_Font *font, std::string text, SDL_Color color, GLfloat *tex);
GLDPOINT GetOGLPos();
SDL_Surface *Load_Image(std::string file);

extern GLDPOINT CURRENT_MOUSE_POS;

//int Draw_Gostek_Th(void *data);

#endif
