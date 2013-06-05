/*   TexturesLoader.cpp
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


#include "TexturesLoader.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"
#include "soil/SOIL.h"


/*void WorldMap::printTextMovable(freetype::font_data& font, const std::string& text, Uint8* color, float x, float y)
{

    glColor3ub(color[0], color[1], color[2]);
    freetype::print(font, x, static_cast<float>(MAX_HEIGHT)-y-12.0f, text.c_str());

}
*/


Tex Texture::SOIL_LoadTextureExBMP(const std::string& file)
{
    Tex res_tex(0, 0, 0);
    //Tex& res_tex = res_texx;
    int channels;
    unsigned char *imgdata = SOIL_load_image(file.c_str(), &res_tex.w, &res_tex.h, &channels, SOIL_LOAD_RGBA);

    if (imgdata == NULL)
    {
        std::cout << "Image was not loaded : " << file << std::endl;
        return res_tex;
    }

    for (int i = 0; i < res_tex.w*res_tex.h*channels; i += 4)
    {
        if (imgdata[i] == 0 && imgdata[i+1] == 255 && imgdata[i+2] == 0)
        {
            imgdata[i+1] = imgdata[i+3] = 0;
        }
    }

    res_tex.tex = SOIL_create_OGL_texture
                  (
                      imgdata, res_tex.w, res_tex.h, channels, SOIL_CREATE_NEW_ID,
                      SOIL_LOAD_RGBA | SOIL_FLAG_INVERT_Y
                  );

    SOIL_free_image_data(imgdata);

    return res_tex;
}



Tex Texture::SOIL_LoadTextureExPNG(const std::string& file)
{
    Tex res_tex(0, 0, 0);
    //Tex& res_tex = res_texx;
    int channels;
    unsigned char* imgdata = SOIL_load_image(file.c_str(), &res_tex.w, &res_tex.h, &channels, SOIL_LOAD_RGBA);

    if (imgdata == NULL)
    {
        std::cout << "Image was not loaded : " << file << std::endl;
        return res_tex;
    }

    res_tex.tex = SOIL_load_OGL_texture
                  (
                      file.c_str(),
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_LOAD_RGBA | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
                  );

    SOIL_free_image_data(imgdata);

    return res_tex;
}


GLuint Texture::SOIL_LoadTextureBMP(const std::string& file)
{

    GLuint texID = 0;

    int width, height, channels;
    unsigned char *imgdata = SOIL_load_image(file.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

    if (imgdata == NULL)
    {
        std::cout << "   Image was not loaded : " << file << std::endl;
        return texID;
    }

    for (int i = 0; i < width*height*channels; i += 4)
    {
        if (imgdata[i] == 0 && imgdata[i+1] == 255 && imgdata[i+2] == 0)
        {
            imgdata[i+1] = imgdata[i+3] = 0;
        }
    }

    texID = SOIL_create_OGL_texture
            (
                imgdata, width, height, channels, SOIL_CREATE_NEW_ID,
                SOIL_LOAD_RGBA | SOIL_FLAG_INVERT_Y
            );

    SOIL_free_image_data(imgdata);

    return texID;
}


GLuint Texture::SOIL_LoadTexturePNG(const std::string& file)
{

    return SOIL_load_OGL_texture
           (
               file.c_str(),
               SOIL_LOAD_AUTO,
               SOIL_CREATE_NEW_ID,
               SOIL_LOAD_RGBA | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
           );

}



// urwana nazwa pliku, bez rozszerzenia (najpierw png, potem bmp), odporna na wielkosc znakow
Tex Texture::LoadExt(const std::string& src_dir, const std::string& file)
{

    if (!boost::filesystem::exists(src_dir))
    {
        Tex res_tex(0, 0, 0);
        //const Tex& res_tex = res_texx;
        std::cerr << "Cannot find source directory : " << src_dir << std::endl;
        return res_tex;
    }

    std::string name_temp;
    boost::filesystem::directory_iterator end;


    name_temp = file + ".png";
    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {

        if (boost::iequals(iter->leaf(), name_temp))
        {
            return SOIL_LoadTextureExPNG(iter->path().string());
        }
    }

    name_temp = file + ".bmp";
    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {
        if (boost::iequals(iter->leaf(), name_temp))
        {
            return SOIL_LoadTextureExBMP(iter->path().string());
        }
    }

    Tex res_texx(0, 0, 0);
    const Tex& res_tex = res_texx;
    std::cerr << "Cannot find : " << file << std::endl;

    return res_tex;

}



// urwana nazwa pliku, bez rozszerzenia (najpierw png, potem bmp), odporna na wielkosc znakow
GLuint Texture::Load(const std::string& src_dir, const std::string& file)
{

    if (!boost::filesystem::exists(src_dir))
    {
        std::cerr << "Cannot find source directory : " << src_dir << std::endl;
        return 0;
    }

    std::string name_temp;
    boost::filesystem::directory_iterator end;


    name_temp = file.substr(0, file.size()-3) + "png";
    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {
        if (boost::iequals(iter->leaf(), name_temp))
        {
            return SOIL_LoadTexturePNG(iter->path().string());
        }
    }

    name_temp = file.substr(0, file.size()-3) + "bmp";
    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {
        if (boost::iequals(iter->leaf(), name_temp))
        {
            return SOIL_LoadTextureBMP(iter->path().string());
        }
    }

    std::cerr << "Cannot find : " << file << std::endl;

    return 0;

}
