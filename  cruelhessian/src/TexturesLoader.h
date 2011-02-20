#ifndef TEXTURES_LOADER_H
#define TEXTURES_LOADER_H

/*   TexturesLoader.h
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



#include <iostream>
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif



class Tex
{
public:
    Tex() : w(0), h(0), tex(0) {};
    Tex(int _w, int _h, GLuint _tex) : w(_w), h(_h), tex(_tex) {};
    int w, h;
    GLuint tex;
};


class Texture
{
private:
    static Tex SOIL_LoadTextureExBMP(const std::string& file);
    static Tex SOIL_LoadTextureExPNG(const std::string& file);
    static GLuint SOIL_LoadTextureBMP(const std::string& file);
    static GLuint SOIL_LoadTexturePNG(const std::string& file);

public:

    static GLuint Load(const std::string& src_dir, const std::string& file);
    static Tex LoadExt(const std::string& src_dir, const std::string& file);
};



#endif
