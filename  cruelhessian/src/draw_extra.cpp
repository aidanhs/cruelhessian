/*   draw_extra.cpp
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

//#include <math.h>
#include <sstream>
#include "draw_extra.h"
#include "worldmap.h"
#include "gun.h"
#include "bot.h"
#include "SDL.h"
//#include "worldmap.h"


//SDL_Surface *Load_Image(const char *file)
SDL_Surface *Load_Image(std::string file)
{
    SDL_Surface *out = NULL, *temps;
    SDL_RWops *rwop;
    temps = IMG_Load(file.c_str());
    if (temps == NULL )
    {
        cout << "Error while loading the file: " << file;
        SDL_FreeSurface(temps);
        exit(EXIT_FAILURE);
    }
    rwop = SDL_RWFromFile(file.c_str(), "rb");
    if (IMG_isBMP(rwop) == 1)
    {
        out = SDL_DisplayFormat(temps);
        SDL_SetColorKey(out, SDL_SRCCOLORKEY, SDL_MapRGB(out->format, 0, 255, 0));
    }
    else if (IMG_isPNG(rwop) == 1)
    {
        out = SDL_DisplayFormatAlpha(temps);
    }
    //else exit(1);

    SDL_FreeSurface(temps);
    SDL_FreeRW(rwop);
    return out;
}




GLDPOINT GetOGLPos()
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY;
    GLdouble winZ = 0.0f;
    //GLdouble posX, posY, posZ;
    GLdouble posZ = 0;
    GLDPOINT ret;
    int x, y;

    ret.x = 0.0f;
    ret.y = 0.0f;

    SDL_GetMouseState(&x, &y);

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( (GLint)x, (GLint)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    //gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &ret.x, &ret.y, &posZ);

    return ret;
}

GLDPOINT CURRENT_MOUSE_POS;

GLuint which_part(int nr)
{
    switch (nr)
    {
        //case 0 : return gost_morda;
        //case 1 : return gost_morda;
    case 2 :
        return world->gost_noga;
    case 3 :
        return world->gost_noga;
    case 4 :
        return world->gost_udo;
    case 5 :
        return world->gost_udo;
        //case 6 : return gost_morda;
    case 7 :
        return world->gost_biodro;
        //case 8 : return gost_morda;
    case 9 :
        return world->gost_klata;
    case 10 :
        return world->gost_ramie;
    case 11 :
        return world->gost_morda;
    case 12 :
        return world->gost_reka;
    case 13 :
        return world->gost_reka;
    case 14 :
        return world->gost_dlon;
    case 15 :
        return world->gost_dlon;
    case 16 :
        //return gost_x;
        //case 17 : return gost_stopa;
        //case 18 : return gost_dlon;
        //case 19 : return gost_dlon;
    default :
        return world->gost_dlon;
    }
    return 0;

}

GLfloat* which_part_xy(int nr)
{
    switch (nr)
    {
        //case 0 : return gost_morda_xy;
        //case 1 : return gost_morda_xy;
    case 2 :
        return world->gost_noga_xy;
    case 3 :
        return world->gost_noga_xy;
    case 4 :
        return world->gost_udo_xy;
    case 5 :
        return world->gost_udo_xy;
        //case 6 : return gost_morda_xy;
    case 7 :
        return world->gost_biodro_xy;
        //case 8 : return gost_morda_xy;
    case 9 :
        return world->gost_klata_xy;
    case 10 :
        return world->gost_ramie_xy;
    case 11 :
        return world->gost_morda_xy;
    case 12 :
        return world->gost_reka_xy;
    case 13 :
        return world->gost_reka_xy;
    case 14 :
        return world->gost_dlon_xy;
    case 15 :
        return world->gost_dlon_xy;
    case 16 :
        //return gost_x_xy;
        //case 17 : return gost_stopa_xy;
        //case 18 : return gost_dlon_xy;
        //case 19 : return gost_dlon_xy;
    default :
        return world->gost_dlon_xy;
    }
    return 0;

}


/*
int Draw_Gostek_Th(void *data)
{

    int k, i;

    //glColor3ub(0,0,0);

    while (1)
    {
        glPushMatrix();
        glTranslatef(Bot[0].position.x, Bot[0].position.y, 0.0);
        for (k = 0; k < FRAMES_MAX; k++)
        {
            printf("dss\n");
            //glClear(GL_COLOR_BUFFER_BIT);
            //glEnable(GL_TEXTURE_2D);

            for (i = 0; i < BODYPARTS_MAX; i++)
            {

                //if (i == 9)
                //glPushMatrix();

                glRectf(part_x[i][k]-0.1, part_z[i][k]-0.1, part_x[i][k]+0.1, part_z[i][k]+0.1);
                //
                //draw_int_help(gost_klata, gost_klata_xy, part_x[i][k], part_z[i][k]);
                //if (i == 11)
                //draw_int_help(gost_morda, gost_morda_xy, part_x[i][k], part_z[i][k]);
            }
            //glDisable(GL_TEXTURE_2D);

            make_lines(k);
            SDL_GL_SwapBuffers();
            SDL_Delay(50);
        }
        glPopMatrix();
    }
    return 0;
}
*/
//void Draw_Gostek_help(float dx, float dy, GLuint tex)
// (dx, dy) - przesuniecie
// part - czesc ciala (1 .. 21)

void WorldMap::draw_gostek_help(float dx, float dy, int part)
{

    GLfloat *xtex = (GLfloat *)malloc(4*sizeof(GLfloat));
    /*GLfloat xtex[4];
    xtex[0] = *which_part_xy(part);
    xtex[1] = *which_part_xy(part)+1;
    xtex[2] = *which_part_xy(part)+2;
    xtex[3] = *which_part_xy(part)+3;
    */
    xtex = which_part_xy(part);

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    glTranslatef(bot[0]->position.x+dx, bot[0]->position.y+dy, 0.0);

    glBindTexture(GL_TEXTURE_2D, which_part(part));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f((float)xtex[0], 0.0f);
    glVertex2f((float)xtex[2], 0.0f);
    glTexCoord2f((float)xtex[0], (float)xtex[1]);
    glVertex2f((float)xtex[2], (float)xtex[3]);
    glTexCoord2f(0.0f, (float)xtex[1]);
    glVertex2f(0.0f, (float)xtex[3]);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //free(xtex);
}



void draw_int_help(GLuint tex, GLfloat *xtex, float dx, float dy)
{
    glPushMatrix();

    glLoadIdentity();
    glTranslatef(world->bgX + dx, world->bgY + dy, 0.0f);

    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f((float)xtex[0], 0.0f);
    glVertex2f((float)xtex[2], 0.0f);
    glTexCoord2f((float)xtex[0], (float)xtex[1]);
    glVertex2f((float)xtex[2], (float)xtex[3]);
    glTexCoord2f(0.0f, (float)xtex[1]);
    glVertex2f(0.0f, (float)xtex[3]);
    glEnd();

    glPopMatrix();

}


//GLuint SDL_GL_LoadTexture(const char *file, GLfloat *tex)
GLuint SDL_GL_LoadTexture(std::string file, GLfloat *tex)
{
    GLuint texture;
    int w = 1, h = 1;
    SDL_Surface *image, *surface;
    SDL_Rect area;
    Uint32 saved_flags;
    Uint8 saved_alpha;
    surface = Load_Image(file);
    //surface = IMG_Load(file);

    while (w < surface->w) w <<= 1;
    while (h < surface->h) h <<= 1;

    tex[0] = (GLfloat)surface->w / w;
    tex[1] = (GLfloat)surface->h / h;
    tex[2] = (GLfloat)surface->w;
    tex[3] = (GLfloat)surface->h;

    image = SDL_CreateRGBSurface(
                SDL_SWSURFACE,
                //SDL_HWSURFACE,
                w, h,
                32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
#else
                0xFF000000,
                0x00FF0000,
                0x0000FF00,
                0x000000FF
#endif
            );
    if ( image == NULL )
    {
        return 0;
    }

    /* Save the alpha blending attributes */
    saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
    saved_alpha = surface->format->alpha;
    if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
    {
        SDL_SetAlpha(surface, 0, 0);
    }

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = (Uint16)surface->w;
    area.h = (Uint16)surface->h;
    //cout << area.w << " DSDSD " << area.h << endl;;
    SDL_BlitSurface(surface, &area, image, &area);

    /* Restore the alpha blending attributes */
    if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
    {
        SDL_SetAlpha(surface, saved_flags, saved_alpha);
    }

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 w, h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);

    return texture;
}


//GLuint SDL_GL_LoadFont(TTF_Font *font, const char *text, SDL_Color color, GLfloat *tex)
GLuint SDL_GL_LoadFont(TTF_Font *font, std::string text, SDL_Color color, GLfloat *tex)
{
    //GLuint texture;
    int w = 1, h = 1;
    SDL_Surface *image, *surface;
    SDL_Rect area;
    Uint32 saved_flags;
    Uint8 saved_alpha;
    //SDL_Rect *tex;
    surface = TTF_RenderText_Blended(font, text.c_str(), color);
    /* Use the surface width and height expanded to powers of 2 */

    while (w < surface->w) w <<= 1;
    while (h < surface->h) h <<= 1;

    tex[0] = (GLfloat)surface->w / w;
    tex[1] = (GLfloat)surface->h / h;
    tex[2] = (GLfloat)surface->w;
    tex[3] = (GLfloat)surface->h;

    image = SDL_CreateRGBSurface(
                SDL_SWSURFACE,
                //SDL_HWSURFACE,
                w, h,
                32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
#else
                0xFF000000,
                0x00FF0000,
                0x0000FF00,
                0x000000FF
#endif
            );
    if ( image == NULL )
    {
        return 0;
    }

    /* Save the alpha blending attributes */
    saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
    saved_alpha = surface->format->alpha;
    if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
    {
        SDL_SetAlpha(surface, 0, 0);
    }

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = (Uint16)surface->w;
    area.h = (Uint16)surface->h;
    SDL_BlitSurface(surface, &area, image, &area);

    /* Restore the alpha blending attributes */
    if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA )
    {
        SDL_SetAlpha(surface, saved_flags, saved_alpha);
    }

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &world->texture_temp);
    //glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, world->texture_temp);
    //glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 w, h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);

    return world->texture_temp;
    //return texture;
}
