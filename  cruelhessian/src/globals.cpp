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


#include "globals.h"


//TVector2D TVector2D::normalize()
void TVector2D::normalize()
{
    float len = sqrt(this->x*this->x + this->y*this->y);

    this->x = (len == 0) ? 0 : this->x / len;
    this->y = (len == 0) ? 0 : this->y / len;
    //return (len == 0) ? TVector2D(0, 0) : TVector2D(this->x / len, this->y / len);
}

TVector2D::TVector2D()
{
    x = 0;
    y = 0;
}

TVector2D::TVector2D(float a, float b)
{
    x = a;
    y = b;
}

TVector2D& TVector2D::operator=(float b)
{
    x = b;
    y = b;
    return *this;
}


TVector2D& TVector2D::operator=(const TVector2D& b)
{
    x = b.x;
    y = b.y;
    return *this;
}

bool TVector2D::operator==(const TVector2D& b) const
{
    return ((x == b.x) && (y == b.y));
}

bool TVector2D::operator!=(const TVector2D& b) const
{
    return ((x != b.x) || (y != b.y));

}

TVector2D& TVector2D::operator+=(const TVector2D &b)
{
    x += b.x;
    y += b.y;
    return *this;
}

TVector2D& TVector2D::operator-=(const TVector2D &b)
{
    x -= b.x;
    y -= b.y;
    return *this;
}

TVector2D& TVector2D::operator*=(float b)
{
    x *= b;
    y *= b;
    return *this;
}

TVector2D& TVector2D::operator/=(float b)
{
    x /= b;
    y /= b;
    return *this;
}


TVector2D TVector2D::operator+(const TVector2D& b) const
{
    TVector2D tmp;

    tmp.x = x + b.x;
    tmp.y = y + b.y;

    return tmp;
}

TVector2D TVector2D::operator-(const TVector2D& b) const
{
    TVector2D tmp;

    tmp.x = x - b.x;
    tmp.y = y - b.y;

    return tmp;
}

float TVector2D::operator*(const TVector2D& b) const
{
    return x * b.x + y * b.y;
}



TVector2D TVector2D::operator*(float a) const
{
    TVector2D tmp;

    tmp.x = x * a;
    tmp.y = y * a;

    return tmp;
}

TVector2D TVector2D::operator/(float a) const
{
    TVector2D tmp;

    tmp.x = x / a;
    tmp.y = y / a;

    return tmp;
}

TVector2D TVector2D::operator-(float a) const
{
    TVector2D tmp;

    tmp.x = x - a;
    tmp.y = y - a;

    return tmp;
}



TVector2D operator*(float a, const TVector2D& b)
{
    TVector2D tmp;

    tmp.x = b.x * a;
    tmp.y = b.y * a;

    return tmp;
}

TVector2D operator/(float a, const TVector2D& b)
{
    TVector2D tmp;

    tmp.x = b.x / a;
    tmp.y = b.y / a;

    return tmp;
}

bool FRIENDLY_FIRE;
float SOUNDS_VOL, MUSIC_VOL;
//std::string SONG_NAME;
int AUDIO_QUAL;

int MAX_WIDTH;
int MAX_HEIGHT;


std::string SOL_PATH = "";
std::string CH_HOME_DIRECTORY = "";
std::string CH_DATA_DIRECTORY = "data/";
std::string CH_CONFIG_FILE = "";
std::string INTERF_PATH = "";
//std::string INTERFACES_PATH = "";
//std::string INTERFACESX_PATH = "";
std::string CH_INTERFACE = "";

float tr_maxx[10000];
float tr_maxy[10000];
float tr_minx[10000];
float tr_miny[10000];
float aa[10000][3];
float con[10000][3];

std::vector<std::string> gMusicList;
int CURRENT_SONG_NUMBER;

int KEY_LEFT;
int KEY_RIGHT;
int KEY_UP;
int KEY_DOWN;
int KEY_RELOAD;
int KEY_GRENADE;

float SOL_WIDTH[50], SOL_HEIGHT[50];

float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie (prawo, lewo)

float OLD_POS_X;
float OLD_POS_Y;

float fullLife = 495.0f;

ubyte textColor[] = { 255, 255, 255, 0 };
ubyte textColorGunOnTouch[] = { 255, 255, 255, 255 };
ubyte textColorGunNormal[] = { 200, 200, 200, 250 };
ubyte textGunColor[] = { 247, 247, 247, 250 };
//ubyte textYellow[] = { 247, 247, 120, 0 };
//ubyte textRed[] = { 255, 0, 0, 0 };
//ubyte textBlue[] = { 0, 0, 255, 0 };

// red, blue, yellow, green
ubyte textCol[][4] = {{255, 0, 0, 0},   {0, 0, 255, 0},  {247, 247, 120, 0},  {0, 255, 0, 0}};


//SDL_Surface* screen;
