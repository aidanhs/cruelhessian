/*   draw.cpp
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

#define _USE_MATH_DEFINES

#include <sstream>
#include <cmath>

#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"
#include "worldmap.h"
#include "globals.h"
#include "soil/SOIL.h"
#include "window_guns.h"


/*void WorldMap::printTextMovable(freetype::font_data& font, const std::string& text, Uint8* color, float x, float y)
{

    glColor3ub(color[0], color[1], color[2]);
    freetype::print(font, x, static_cast<float>(MAX_HEIGHT)-y-12.0f, text.c_str());

}
*/

void WorldMap::draw_background()
{
    // zawsze staraj sie ustawiac ekran tak, aby bot, mysz i srodek ekranu byly w jednej linii (punkcie)

    // przesun tlo ekranu w zaleznosci od polozenia Bota
    if (SHOW_GUN_MENU && ONLY_ONCE)
    {
        // when bot was killed
        ONLY_ONCE = false;
        glTranslatef(OLD_POS.x - bot[MY_BOT_NR]->position.x, OLD_POS.y - bot[MY_BOT_NR]->position.y, 0.0f);
    }

    else if (mouse->getGlobalX() <= bot[MY_BOT_NR]->position.x + MAX_WIDTH &&
             mouse->getGlobalX() >= bot[MY_BOT_NR]->position.x - MAX_WIDTH &&
             mouse->getGlobalY() <= bot[MY_BOT_NR]->position.y + MAX_HEIGHT &&
             mouse->getGlobalY() >= bot[MY_BOT_NR]->position.y - MAX_HEIGHT)
    {
        int REL_MOUSE_X, REL_MOUSE_Y;          // roznica od ostatniego polozenia
        SDL_GetRelativeMouseState(&REL_MOUSE_X, &REL_MOUSE_Y);
        glTranslatef(-static_cast<GLfloat>(REL_MOUSE_X), -static_cast<GLfloat>(REL_MOUSE_Y), 0.0f);
    }

    glBegin(GL_POLYGON);
    glColor4ub(static_cast<GLubyte>(map->bgColorTop.red), static_cast<GLubyte>(map->bgColorTop.green), static_cast<GLubyte>(map->bgColorTop.blue), static_cast<GLubyte>(map->bgColorTop.alpha));
    glVertex2f(map->leftoffs - MAX_WIDTH, -map->topoffs);
    glVertex2f(map->rightoffs + MAX_WIDTH, -map->topoffs);
    glColor4ub(static_cast<GLubyte>(map->bgColorBottom.red), static_cast<GLubyte>(map->bgColorBottom.green), static_cast<GLubyte>(map->bgColorBottom.blue), static_cast<GLubyte>(map->bgColorBottom.alpha));
    glVertex2f(map->rightoffs + MAX_WIDTH, -map->bottomoffs);
    glVertex2f(map->leftoffs - MAX_WIDTH, -map->bottomoffs);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

}


void WorldMap::draw_int_help(const Tex& texture, float dx, float dy)
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(bgX + dx, bgY + dy, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(texture.w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, texture.h);
    glEnd();

    glPopMatrix();

}


void WorldMap::draw_interface()
{
    int x, y;
    std::ostringstream oss;
    std::string temp;


    // ----- Gun's name -----

    printText(font[1][FontConsoleSize], weapon_base[bot[MY_BOT_NR]->gunModel].name, textCol[3], 0.01f*MAX_WIDTH+170, MAX_HEIGHT-34.0f);

    // ----- Ammo count -----

    oss << bot[MY_BOT_NR]->leftAmmos;
    printText(font[1][FontConsoleSize], oss.str(), textCol[3], 0.46f*MAX_WIDTH+33, MAX_HEIGHT-32.0f);
    oss.str("");


    glEnable(GL_TEXTURE_2D);

    // ----- Health -----

    draw_int_help(text_health, 0.01f*MAX_WIDTH, MAX_HEIGHT-38.0f);

    text_healthbar.w = 115.0f*bot[MY_BOT_NR]->actLife / fullLife;
    draw_int_help(text_healthbar, 0.01f*MAX_WIDTH+40, MAX_HEIGHT-29.0f);


    // ----- Ammo -----

    draw_int_help(text_ammo, 0.46f*MAX_WIDTH, MAX_HEIGHT-38.0f);

    // grenades' count
    for (unsigned int i = 0; i < bot[MY_BOT_NR]->numGrenades; ++i)
    {
        draw_int_help(text_nade, 0.46f*MAX_WIDTH+i*10, MAX_HEIGHT-15.0f);
    }

    if (!bot[MY_BOT_NR]->isReloading)
        text_reloadbar.w = 115.0f*bot[MY_BOT_NR]->leftAmmos / weapon_base[bot[MY_BOT_NR]->gunModel].ammo;
    draw_int_help(text_reloadbar, 0.46f*MAX_WIDTH+67, MAX_HEIGHT-29.0f);


    // -----  Little gold -----

    draw_int_help(text_firebar_r, 0.46f*MAX_WIDTH + text_ammo.w - text_firebar_r.w, MAX_HEIGHT-14.0f);
    float time_gold = static_cast<float>(getCurrentTime - bot[MY_BOT_NR]->lastShotTime) / static_cast<float>(weapon_base[bot[MY_BOT_NR]->gunModel].fireInterval);
    text_firebar.w = 57.0f*(time_gold > 1.0f ? 1.0f : time_gold);
    draw_int_help(text_firebar, 0.46f*MAX_WIDTH + text_ammo.w - text_firebar_r.w, MAX_HEIGHT-14.0f);


    // ----- Flying -----

    draw_int_help(text_jet, 0.75f*MAX_WIDTH, MAX_HEIGHT-38.0f);
    text_jetbar.w = 115.0f*bot[MY_BOT_NR]->procJet;
    draw_int_help(text_jetbar, 0.75f*MAX_WIDTH+40.0f, MAX_HEIGHT-29.0f);


    // ----- Player status -----

    // my position / players number
    oss << MY_CURRENT_POS;
    temp = oss.str() + "/";
    oss.str("");
    oss << bot.size();
    temp += oss.str();
    printText(font[1][FontConsoleSize], temp, textCol[4], 0.87f*MAX_WIDTH, MAX_HEIGHT-65.0f);
    oss.str("");

    // kills / points (points to leader)
    if (CURRENT_GAME_MODE == DM)
        oss << bot[MY_BOT_NR]->killedNr;
    else
        oss << bot[MY_BOT_NR]->points;
    temp = oss.str() + " (";
    oss.str("");
    if (DISTANCE_SCORE < 0) temp += "-";
    else temp += "+";
    oss << abs(DISTANCE_SCORE);
    temp += oss.str() + ")";
    printText(font[1][FontConsoleSize], temp, textCol[1], 0.87f*MAX_WIDTH, MAX_HEIGHT-55.0f);
    oss.str("");

    // kill / point / capture limit
    oss << FIRST_LIMIT;
    printText(font[1][FontConsoleSize], oss.str(), textCol[2], 0.87f*MAX_WIDTH, MAX_HEIGHT-45.0f);
    oss.str("");

    glDisable(GL_TEXTURE_2D);


    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (bot[i]->isKilled)
        {
            Uint32 new_time = getCurrentTime - bot[i]->timerRespawnTime;
            if (new_time > bot[i]->respawnTime)
            {
                bot[i]->isKilled = false;
                if (i == MY_BOT_NR && SOUNDS_VOL > 0)
                    Mix_PlayChannel(-1, sound_new_life, 0);
            }
            else if (i == MY_BOT_NR)
            {
                oss << static_cast<float>(bot[MY_BOT_NR]->respawnTime - new_time)/1000;
                printText(font[1][FontConsoleSize], "Respawn in " + oss.str(), textCol[1], 0.5f*MAX_WIDTH-20, 50.0f);
                printText(font[1][FontConsoleSize], "Killed by " + bot[bot[MY_BOT_NR]->killer]->name, textCol[1], 0.5f*MAX_WIDTH-80, MAX_HEIGHT-94.0f);
                oss.str("");
                if (new_time >= 500 && !CHOICE_GUN)
                {
                    window_guns = new WindowGuns(weapon_base);
                    SHOW_GUN_MENU = true;
                }
            }
        }
    }

    if (!bot[MY_BOT_NR]->isKilled)
        CHOICE_GUN = false;

    if (YOU_KILLED)
    {
        if (getCurrentTime - bot[MY_BOT_NR]->youKilledTime <= 1000)
            printText(font[1][FontConsoleSize], "You killed " + bot[bot[MY_BOT_NR]->killed]->name, textCol[1], 0.5f*MAX_WIDTH-20, 50.0f);
        else
            YOU_KILLED = false;
    }


    if (SHOW_GUN_MENU && !CHOICE_GUN)
    {
        //SDL_ShowCursor(SDL_ENABLE);
        window_guns->draw(mouse->getLocalPosition());

        // if select gun with mouse button
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, menu_click, 0);

            int nr = window_guns->select();

            if (nr != -1)
            {
                bot[MY_BOT_NR]->gunModel = nr;
                bot[MY_BOT_NR]->leftAmmos = weapon_base[bot[MY_BOT_NR]->gunModel].ammo;
                CHOICE_GUN = true;
                SHOW_GUN_MENU = false;
            }
        }
        // if select gun with key
        else if (KEY_PRESSED >= SDLK_0 && KEY_PRESSED <= SDLK_9)
        {
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, menu_click, 0);

            bot[MY_BOT_NR]->gunModel = KEY_PRESSED-48;
            bot[MY_BOT_NR]->leftAmmos = weapon_base[bot[MY_BOT_NR]->gunModel].ammo;
            CHOICE_GUN = true;
            SHOW_GUN_MENU = false;
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }

    chat->update();
    chat->draw();

    if (SHOW_STATS)
    {
        oss << "FPS: ";
        oss << currentFPS;
        printText(font[1][FontConsoleSize], oss.str(), textCol[3], 0.8f*MAX_WIDTH, 15.0f);
        oss.str("");
    }

    // scores
    if (SHOW_SCORES)
    {
//        window_scores->update(bot, MY_BOT_NR);
        window_scores->draw();
    }

    if (SHOW_COMMAND_LINE)
    {
        command_line();
    }

    if (SHOW_MYCHAT_LINE)
    {
        mychat_line();
    }


    if (SHOW_ESC)
    {
        //glutSetCursor(GLUT_CURSOR_INHERIT);
        window_exit->draw(mouse->getLocalPosition());
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            CHOICE_EXIT = window_exit->select();
            // std::cout << "C " << CHOICE_EXIT << std::endl;
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }


}


Tex WorldMap::SOIL_LoadTextureExBMP(const std::string& file)
{
    Tex res_tex;
    int width = 0, height = 0;

    int channels;
    unsigned char *imgdata = SOIL_load_image
                             (
                                 file.c_str(),
                                 &width, &height, &channels,
                                 SOIL_LOAD_RGBA
                             );

    if (imgdata == NULL)
    {
        std::cout << "Image was not loaded : " << file << std::endl;
        res_tex.w = res_tex.h = 0;
        return res_tex;
    }

    for (int i = 0; i < width*height*channels; i += 4)
    {
        if (imgdata[i] == 0 && imgdata[i+1] == 255 && imgdata[i+2] == 0)
        {
            imgdata[i+1] = imgdata[i+3] = 0;
        }
    }
    res_tex.tex = SOIL_create_OGL_texture
                  (
                      imgdata, width, height, channels, SOIL_CREATE_NEW_ID,
                      SOIL_LOAD_RGBA | SOIL_FLAG_INVERT_Y
                  );

    SOIL_free_image_data(imgdata);

    res_tex.w = static_cast<float>(width);
    res_tex.h = static_cast<float>(height);
    return res_tex;
}



Tex WorldMap::SOIL_LoadTextureExPNG(const std::string& file)
{
    Tex res_tex;
    int width = 0, height = 0;
    int channels;

    if (SOIL_load_image(file.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA) == NULL)
    {
        std::cout << "Image was not loaded : " << file << std::endl;
        res_tex.w = res_tex.h = 0;
        return res_tex;
    }

    res_tex.tex = SOIL_load_OGL_texture
                  (
                      file.c_str(),
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_LOAD_RGBA | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
                      //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                  );

    res_tex.w = static_cast<float>(width);
    res_tex.h = static_cast<float>(height);
    return res_tex;
}


GLuint WorldMap::SOIL_LoadTextureBMP(const std::string& file)
{

    GLuint texID = 0;

    int width, height, channels;
    unsigned char *imgdata = SOIL_load_image
                             (
                                 file.c_str(),
                                 &width, &height, &channels,
                                 SOIL_LOAD_RGBA
                             );
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


GLuint WorldMap::SOIL_LoadTexturePNG(const std::string& file)
{

    return SOIL_load_OGL_texture
           (
               file.c_str(),
               SOIL_LOAD_AUTO,
               SOIL_CREATE_NEW_ID,
               SOIL_LOAD_RGBA | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
               //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
           );

}



// urwana nazwa pliku, bez rozszerzenia (najpierw png, potem bmp), odporna na wielkosc znakow
Tex WorldMap::SOIL_LoadTextureEx2(const std::string& src_dir, const std::string& file)
{

    if (!boost::filesystem::exists(src_dir))
    {
        Tex res_tex;
        res_tex.w = res_tex.h = 0.0f;
		res_tex.tex = 0;
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



    Tex res_tex;
    res_tex.w = res_tex.h = 0.0f;
	res_tex.tex = 0;
    std::cerr << "Cannot find : " << file << std::endl;

    return res_tex;

}



// urwana nazwa pliku, bez rozszerzenia (najpierw png, potem bmp), odporna na wielkosc znakow
GLuint WorldMap::SOIL_LoadTexture2(const std::string& src_dir, const std::string& file)
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
