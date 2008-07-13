/*   globals.h
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

#ifndef GLOBALS_H
#define GLOBALS_H


//#include <SDL/SDL_net.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include <iostream>
#include <list>
#include "gun.h"
#include "read_pms.h"
#include <math.h>



class TVector2D
{
public:
    float x;
    float y;
};


typedef enum SQ
{
    HOW_MANY_BOTS = 0
} SQ;


extern const char *host;
extern int port;

extern std::string MAP;
extern bool ONLY_ONCE;
extern Uint32 timer_fps;
extern int countfps;

extern int MAX_WIDTH, MAX_HEIGHT;

extern int quitt;

extern int MAX_BPP;

extern float SOUNDS_VOL, MUSIC_VOL;
extern std::string SONG_NAME;
extern int AUDIO_QUAL;
extern bool GAME_LOOP;

extern int FULLSCREEN;

extern Uint32 lastt;

extern Uint32 ElapsedTime;

extern std::string SOL_PATH;

#endif
