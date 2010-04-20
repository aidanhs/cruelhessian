/*   globals.h
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

#ifndef GLOBALS_H
#define GLOBALS_H


#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_mixer.h"
//#include "SDL_opengl.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include "fonts.h"
/*
#ifdef WIN32
#include <windows.h>
#endif
*/
#define _(string) (string)

namespace global
{


extern int CONFIG_VERSION;
extern unsigned int *COLOR_SHIRT, *COLOR_SKIN, *COLOR_HAIR, *COLOR_PANTS, *COLOR_JET;
extern unsigned int textColorGunOnTouch[4];
extern unsigned int textColorGunNormal[4];
extern unsigned int textCol[5][4];
extern unsigned int textGunColor[4];
extern unsigned int* tcolor2rgb(const std::string& col);
extern std::string rgb2tcolor(unsigned int* col);

extern     SDL_Surface *screen;


extern  int setSDL();

extern freetype::font_data font[2][50];
void printText(freetype::font_data& font, const std::string& text, unsigned int* color, float x, float y);
void printTextMiddle(freetype::font_data& font, const std::string& text, unsigned int* color, float y);


extern    int FontMenuSize;
extern   int FontConsoleSize;
extern    int FontBigSize;
extern   int FontWeaponMenuSize;
extern  int FontConsoleSmallSize;
extern   int FontHeightScale;
extern   int FontMenuBold;
extern   int FontConsoleBold;
extern   int FontBigBold;
extern   int FontWeaponMenuBold;
extern   int FontConsoleSmallBold;
extern   int KillConsoleNameSpace;

//extern float bgX, bgY;

extern  bool FULLSCREEN;
extern     int MAX_BPP;

extern   float sGravity; // Przyspieszenie ziemskie
extern   float sDrag; // Wspólczynnik oporu
extern   float sDragWalking; // Wspólczynnik oporu podczas chodzenia
extern   float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
//static float sElasticity = 0.05; // Elastycznosc odbicia
//float sFriction = 1.5; // Tarcie
//float sFriction = 1; // Tarcie

extern     float sWalking; // Szybkosc chodzenia
//float getsWalking(){return sWalking;};
extern    float sFlying; // Szybkosc latania
extern   float sJumping; // Sila skoku
extern   bool checkSoldat();
extern float fTimeStep; // Krok czasowy fizyki

extern unsigned int FRAMES_MAX[50], BODYPARTS_MAX[50];

typedef enum BONUS
{
    BONUS_BERSERKER = 0,
    BONUS_CLUSTERS,
    BONUS_GRENADES,
    BONUS_FLAMER,
    BONUS_MEDIKITS,
    BONUS_PREDATOR,
    BONUS_VESTKIT
} BONUS;


extern const float _180overpi;

extern float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie (prawo, lewo)

extern Uint32 getCurrentTime;

extern float MAX_WIDTH, MAX_HEIGHT;

extern float SOUNDS_VOL, MUSIC_VOL;
//extern std::string SONG_NAME;
extern int AUDIO_QUAL;

extern std::string PLAYER_NAME;
extern std::string CH_HOME_DIRECTORY;
extern std::string CH_DATA_DIRECTORY;
extern std::string CH_CONFIG_FILE;
extern std::string SOL_PATH;
extern std::string INTERF_PATH;
//extern std::string INTERFACES_PATH;
//extern std::string INTERFACESX_PATH;
extern std::string CH_INTERFACE;

extern unsigned int TIME_LIMIT;
extern unsigned int FIRST_LIMIT;

extern float SOL_WIDTH[50], SOL_HEIGHT[50];

//extern float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie

typedef struct Tex
{
    float w, h;
    GLuint tex;
} Tex;

typedef enum GAME_MODE
{
    DM = 0,
    PM,
    TM,
    CTF,
    RM,
    INF,
    HTF
} GAME_MODE;

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
    TEAM_GENERAL = 0,
    TEAM_ALPHA,
    TEAM_BRAVO,
    TEAM_CHARLIE,
    TEAM_DELTA
} TEAM;

typedef enum OBJECT_SHAPE
{
    XPOINT = 0,
    CIRCLE,
    SQUARE
} OBJECT_SHAPE;


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

//extern bool FRIENDLY_FIRE;

//extern float OLD_POS_X;
//extern float OLD_POS_Y;

extern float fullLife;
/*
extern ubyte textColor[];
extern ubyte textGunColor[];
extern ubyte textColorGunOnTouch[];
extern ubyte textColorGunNormal[];
//extern ubyte textYellow[];
//extern ubyte textRed[];
//extern ubyte textBlue[];

extern ubyte textCol[][4];
*/
extern std::vector<std::string> gMusicList;
//extern std::vector<std::string> mapsList;
extern int CURRENT_SONG_NUMBER;

extern int KEY_LEFT;
extern int KEY_RIGHT;
extern int KEY_UP;
extern int KEY_DOWN;
extern int KEY_RELOAD;
extern int KEY_GRENADE;
extern int KEY_CHAT;
extern int KEY_TEAMCHAT;
extern GAME_MODE CURRENT_GAME_MODE;

extern  Mix_Chunk *grenade_throw, *grenade_bounce, *grenade_explosion, *grenade_pullout, *sound_new_life, *sound_heaven, *sound_death[3], *sound_kitfall[2], *sound_spawn, *menu_click;

typedef enum BODY
{
    STOPA = 0,
    KLATA,
    RAMIE,
    MORDA,
    REKA,
    DLON,
    UDO,
    BIODRO,
    NOGA
} BODY;


typedef enum
{
    SHIRT = 0,
    HAIR,
    SKIN,
    PANTS
} BODY_COLOR;


class WeaponBase
{
public:
    std::string name;
    Mix_Chunk *fireSound;
    Mix_Chunk *reloadSound;
    float damage;
    Uint32 fireInterval;
    unsigned int ammo;
    Uint32 reloadTime;
    float speed;
    int bulletStyle;
    float startUpTime;
    float bink;
    int movementAcc;
    int recoil;
    Tex textureAmmo;
    Tex textureGun;
};

}

using namespace global;
#endif
