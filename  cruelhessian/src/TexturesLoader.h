#ifndef SOIL_LOADER_H
#define SOIL_LOADER_H

/*   TexturesLoader.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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

#include "SDL_opengl.h"
#include "Tex.h"



GLuint SOIL_LoadTextureBMP(const std::string& file);
GLuint SOIL_LoadTexturePNG(const std::string& file);
GLuint SOIL_LoadTexture2(const std::string& src_dir, const std::string& file);

Tex SOIL_LoadTextureExBMP(const std::string& file);
Tex SOIL_LoadTextureExPNG(const std::string& file);
Tex SOIL_LoadTextureEx2(const std::string& src_dir, const std::string& file);

#endif
