/*   globals.cpp
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
#include <fstream>
#include <sstream>

#include "globals.h"
#include "parser/SimpleIni.h"
#include "boost/filesystem/fstream.hpp"



namespace global
{

SDL_Surface *screen;

int CONFIG_VERSION = 2;

unsigned int *COLOR_SHIRT, *COLOR_SKIN, *COLOR_HAIR, *COLOR_PANTS, *COLOR_JET;

GAME_MODE CURRENT_GAME_MODE;

unsigned int textColorGunOnTouch[4];
unsigned int textColorGunNormal[4];
unsigned int textCol[5][4];
unsigned int textGunColor[4];

float SOUNDS_VOL, MUSIC_VOL;
int AUDIO_QUAL;
bool FULLSCREEN;
int MAX_BPP;
float MAX_WIDTH, MAX_HEIGHT;

unsigned int FIRST_LIMIT;

freetype::font_data font[2][50];

int FontMenuSize;
int FontConsoleSize;
int FontBigSize;
int FontWeaponMenuSize;
int FontConsoleSmallSize;
int FontHeightScale;
int FontMenuBold;
int FontConsoleBold;
int FontBigBold;
int FontWeaponMenuBold;
int FontConsoleSmallBold;
int KillConsoleNameSpace;


int LIMIT_TEAMMATCH;
int LIMIT_DEATHMATCH;
int LIMIT_POINTMATCH;
int LIMIT_RAMBOMATCH;
int LIMIT_INFILTRATION;
int LIMIT_HOLD;
int LIMIT_CAPTURE;
int LIMIT_TIME;
bool CONSOLE_SHOW;
int RANDOM_BOTS;
int RANDOM_BOTS_1;
int RANDOM_BOTS_2;
int RANDOM_BOTS_3;
int RANDOM_BOTS_4;

bool WEAPON[14];
bool BONUSES[7];

void printTextMiddle(freetype::font_data& fontx, const std::string& text, unsigned int* color, float y)
{
    printText(fontx, text, color, MAX_WIDTH/2 - text.length() / 2 - 50, y);
}


void printText(freetype::font_data& fontx, const std::string& text, unsigned int* color, float x, float y)
{
    glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    glLoadIdentity();
    freetype::print(fontx, x, MAX_HEIGHT-y-12.0f, text.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}



std::string PLAYER_NAME;
std::string SOL_PATH;
std::string CH_HOME_DIRECTORY = "";
std::string CH_DATA_DIRECTORY = "data/";
std::string CH_CONFIG_FILE = "";
std::string INTERF_PATH = "";
//std::string INTERFACES_PATH = "";
//std::string INTERFACESX_PATH = "";
std::string CH_INTERFACE = "";

std::vector<std::string> gMusicList;
//std::vector<std::string> mapsList;
int CURRENT_SONG_NUMBER;

int KEY_LEFT;
int KEY_RIGHT;
int KEY_UP;
int KEY_DOWN;
int KEY_RELOAD;
int KEY_GRENADE;
int KEY_CHAT;
int KEY_TEAMCHAT;

Uint32 getCurrentTime;

float SOL_WIDTH[50], SOL_HEIGHT[50];
float fullLife = 495.0f;

float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie (prawo, lewo)

const float _180overpi = 57.29f;
Mix_Chunk *grenade_throw, *grenade_bounce, *grenade_explosion, *grenade_pullout, *sound_new_life, *sound_heaven, *sound_death[3], *sound_kitfall[2], *sound_spawn, *menu_click;



unsigned int FRAMES_MAX[50], BODYPARTS_MAX[50];
//Tex gost_x;
//Tex gost_stopa[2], gost_klata[2], gost_ramie[2], gost_morda[2], gost_reka[2], gost_dlon[2], gost_udo[2], gost_biodro[2], gost_noga[2];
float sGravity; // Przyspieszenie ziemskie
float sDrag; // Wspólczynnik oporu
float sDragWalking; // Wspólczynnik oporu podczas chodzenia
float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
//static float sElasticity = 0.05; // Elastycznosc odbicia
//float sFriction = 1.5; // Tarcie
//float sFriction = 1; // Tarcie

float sWalking; // Szybkosc chodzenia
//float getsWalking(){return sWalking;};
float sFlying; // Szybkosc latania
float sJumping; // Sila skoku

float fTimeStep; // Krok czasowy fizyki



// 'soldat style' interface - fix !!



bool checkSoldat()
{

    //boost::filesystem::directory_iterator end;

    if (!boost::filesystem::exists(SOL_PATH))
    {
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Anims/"))
    {
        std::cerr << "Cannot find Anims directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Bots/"))
    {
        std::cerr << "Cannot find Bots directory (not needed)" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Gostek-gfx/"))
    {
        std::cerr << "Cannot find Gostek-gfx directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Interface-gfx/"))
    {
        std::cerr << "Cannot find Interface-gfx directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Scenery-gfx/"))
    {
        std::cerr << "Cannot find Scenery-gfx directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Sfx/"))
    {
        std::cerr << "Cannot find Sfx directory (not needed)" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Sparks-gfx/"))
    {
        std::cerr << "Cannot find Sparks-gfx directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Textures/"))
    {
        std::cerr << "Cannot find Texture directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Txt/"))
    {
        std::cerr << "Cannot find Txt directory" << std::endl;
        return false;
    }
    if (!boost::filesystem::exists(SOL_PATH + "Weapons-gfx/"))
    {
        std::cerr << "Cannot find Weapons-gfx directory" << std::endl;
        return false;
    }

    return true;
}







int setSDL()
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
    {
        std::cerr << "Unable to set attributes SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (FULLSCREEN)
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);

    if (!screen)
    {
        std::cerr << "Unable to set video mode: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Cruel Hessian", "");

    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    return 0;

}

// convert TColor to RGB
// TColor - $00bbggrr
unsigned int* tcolor2rgb(const std::string& col)
{

    unsigned int *res = new unsigned int[3];

    sscanf(col.substr(3, 2).c_str(), "%x", &res[2]);
    sscanf(col.substr(5, 2).c_str(), "%x", &res[1]);
    sscanf(col.substr(7, 2).c_str(), "%x", &res[0]);

    return res;
}

// convert RGB to TColor
// TColor - $00bbggrr
std::string rgb2tcolor(unsigned int* col)
{
    std::string temp("$00"), res;
    std::ostringstream oss;

    for (int i = 2; i >= 0; --i)
    {
        oss.str("");
        oss << std::hex << col[i];
        res = oss.str();
        if (res.length() == 1)
            res.insert(0, "0");
        temp += res;
    }

    return temp;
}

}
