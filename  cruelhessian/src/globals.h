/*   globals.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawe³ Konieczny <konp84@gmail.com>
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

#ifndef GLOBALS_H
#define GLOBALS_H


#include <iostream>
#include <list>
#include <math.h>

#include "SDL.h"
#include <GL/gl.h>
#include "read_pms.h"


//extern SDL_Surface* screen;


typedef struct Tex
{
    float w, h;
    GLuint tex;
} Tex;

class TVector2D
{
public:
    float x;
    float y;

    //TVector2D(const TVector2D &);
//    TVector2D& operator+=(TVector2D &a, const TVector2D &b);
    TVector2D();
    TVector2D(float, float);
    TVector2D& operator=(const TVector2D&);
    TVector2D& operator=(float);

    bool operator==(const TVector2D&) const;
    bool operator!=(const TVector2D&) const;

    TVector2D operator+(const TVector2D&) const;
    TVector2D operator-(const TVector2D&) const;
    TVector2D operator-(float) const;
    float operator*(const TVector2D&) const;
    TVector2D operator*(float) const;
    TVector2D operator/(float) const;
    friend TVector2D operator*(float, const TVector2D&);
    friend TVector2D operator/(float, const TVector2D&);
    TVector2D& operator+=(const TVector2D&);
    TVector2D& operator-=(const TVector2D&);
    TVector2D& operator*=(float b);
    TVector2D& operator/=(float b);
    void normalize();
    //TVector2D& operator*=(const TVector2D&);
    //TVector2D& operator/=(const TVector2D&);

};


extern float tr_maxx[10000];
extern float tr_maxy[10000];
extern float tr_minx[10000];
extern float tr_miny[10000];
extern float aa[10000][3];
extern float con[10000][3];

extern int MAX_WIDTH, MAX_HEIGHT;

extern float SOUNDS_VOL, MUSIC_VOL;
//extern std::string SONG_NAME;
extern int AUDIO_QUAL;

extern std::string CH_HOME_DIRECTORY;
extern std::string CH_DATA_DIRECTORY;
extern std::string CH_CONFIG_FILE;
extern std::string SOL_PATH;
extern std::string INTERF_PATH;
//extern std::string INTERFACES_PATH;
//extern std::string INTERFACESX_PATH;
extern std::string CH_INTERFACE;



extern float SOL_WIDTH[50], SOL_HEIGHT[50];

extern float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie

typedef enum OBJECT_TYPE
{
    GOSTEK = 0,
    BULLET,
    GRENADE,
    PARA,
    FLAG,
    KARABIN,
    KIT,
    STAT
} OBJECT_TYPE;


typedef enum TEAM
{
    TEAM_ALPHA = 0,
    TEAM_BRAVO,
    TEAM_CHARLIE,
    TEAM_DELTA
} TEAM;

typedef enum MT
{
    BIEGA = 0,
    BIEGA_TYL,
    CELUJE,
    CIESZY,
    GORA,
    KUCA,
    RZUCA,
    SKOK,
    SKOK_DOL_OBROT,
    SKOK_DOL_OBROT_TYL,
    SKOK_W_BOK,
    SPADA,
    STOI,
    ZMIEN_BRON
} MT;

typedef enum MD
{
    LEFT = 0,
    RIGHT
} MD;

extern bool FRIENDLY_FIRE;

extern float OLD_POS_X;
extern float OLD_POS_Y;

extern float fullLife;

extern ubyte textColor[];
extern ubyte textGunColor[];
extern ubyte textColorGunOnTouch[];
extern ubyte textColorGunNormal[];
//extern ubyte textYellow[];
//extern ubyte textRed[];
//extern ubyte textBlue[];

extern ubyte textCol[][4];

extern std::vector<std::string> gMusicList;
extern int CURRENT_SONG_NUMBER;

extern int KEY_LEFT;
extern int KEY_RIGHT;
extern int KEY_UP;
extern int KEY_DOWN;
extern int KEY_RELOAD;
extern int KEY_GRENADE;

#endif
