/*   main.cpp
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

#include "SDL_mixer.h"
#include "globals.h"
//#include "net.h"
#include "worldmap.h"
#include "draw_extra.h"
#include "read_pms.h"
#include "bot.h"


static Mix_Music *music = NULL;

TTF_Font *font1_16 = NULL;
TTF_Font *font2_12 = NULL;
TTF_Font *font2_28 = NULL;
TTF_Font *FontWeaponMenu = NULL;
SDL_Color textColor = { 255, 255, 255, 0 };
SDL_Color textGunColor = { 247, 247, 247, 250 };
SDL_Color textYellow = { 247, 247, 120, 0 };
SDL_Color textRed = { 255, 0, 0, 0 };


int init_all()
{

    SDL_Surface *screen;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
    {
        cout << "Unable to set attributes SDL: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    if (FULLSCREEN)
        screen = SDL_SetVideoMode(MAX_WIDTH, MAX_HEIGHT, MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(MAX_WIDTH, MAX_HEIGHT, MAX_BPP, SDL_OPENGL);
    if (!screen)
    {
        cout << "Unable to set video mode: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}



int init_all2()
{
    if (TTF_Init() < 0)
    {
        cout << "Unable to initialize SDL_ttf: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }
//cout << "FFF";
    read_fonts();

    if (!(font1_16 = TTF_OpenFont((SOL_PATH+Font1).c_str(), 16)))
    {
        cout << "Unable to load font f1: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    if (!(font2_12 = TTF_OpenFont((SOL_PATH+Font2).c_str(), 12)))
    {
        cout << "Unable to load font f2: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    //if (!(font_red_big = TTF_OpenFont("bgothl.ttf", 28)))
    if (!(font2_28 = TTF_OpenFont((SOL_PATH+Font2).c_str(), 28)))
    {
        cout << "Unable to load font f3: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

/*    if (SDLNet_Init() < 0)
    {
        cout << "Cannot initialize net\n";
        exit(EXIT_FAILURE);
    }*/

    if (SOUNDS_VOL + MUSIC_VOL > 0)
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            cout << "Unable to init audio: " << SDL_GetError() << endl;
            //SDL_Quit();
            exit(EXIT_FAILURE);
        }

        if (Mix_OpenAudio(AUDIO_QUAL, AUDIO_S16SYS, 1, 2048) != 0)
        {
            cout << "Unable to initialize audio: " << Mix_GetError() << endl;
            exit(EXIT_FAILURE);
        }

        if (MUSIC_VOL > 0)
        {
            if ((music = Mix_LoadMUS((SOL_PATH + "Mp3/" + SONG_NAME).c_str())) == NULL)
            {
                cout << "Mixer: " << Mix_GetError() << endl;
                exit(EXIT_FAILURE);
            }
            if (Mix_PlayMusic(music, 0) == -1)
            {
                cout << "Unable to play file: " << Mix_GetError() << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    cout << "QQQQ";

    world = new WorldMap();

    bot = new Bot*[world->BOTS_NUM];

    bot[0] = new Bot(0, "Bot 1", p.spawnpoint[0].x, p.spawnpoint[0].y-8, 1, 0);
    bot[1] = new Bot(1, "Bot 2", p.spawnpoint[1].x, p.spawnpoint[1].y-8, 1, 0);
    //bot[2] = new Bot(2, "Bot 3", p.spawnpoint[2].x, p.spawnpoint[2].y-8, 1, 0);
    //bot[3] = new Bot(3, "Bot 4", p.spawnpoint[3].x, p.spawnpoint[3].y-8, 1, 0);
    //bot[4] = new Bot(4, "Bot 5", p.spawnpoint[4].x, p.spawnpoint[4].y-8, 1, 0);

    SDL_ShowCursor(0);
    srand(time(0));

    timer_fps = SDL_GetTicks();

    return 0;
}
/*
SDL_Thread *TH_cli;
SDL_Thread *TH_ser;
*/
void start_server()
{
    world->isServer = 1;
    world->MY_BOT_NR = 0;
    //prepare_net_server();
    //TH_ser = SDL_CreateThread(server_thread, NULL);
}


void start_client()
{
    world->isServer = 0;

    // zapytaj serwera ile juz jest Botow
    //MY_BOT_NR = ask_server(HOW_MANY_BOTS);
    world->MY_BOT_NR = 1;
    //prepare_net_client();
    //TH_cli = SDL_CreateThread(client_thread, NULL);
}

int free_init_all2()
{
    if (MUSIC_VOL > 0)
    {
        Mix_HaltMusic();
        Mix_CloseAudio();
        Mix_FreeMusic(music);
    }

    //if (SOUNDS_VOL + MUSIC_VOL > 0)
    world->WorldMap::~WorldMap();
    return 0;
}

void WorldMap::game_loop()
{
//cout << "start";
    if ((SDL_GetTicks()-timer_fps) >= 1000)
    {
        cout << "FPS : " << countfps << endl;
        countfps = 0;
        timer_fps = SDL_GetTicks();
    }

    world->keys_control();
    //world->bots_control();

    world->draw_background();

    world->draw_gostek();
    world->draw_screen();
    world->draw_interface();

    world->draw_arms();

    world->draw_mouse();

    //Draw_Blur_Background();
    world->draw_infos();

    SDL_GL_SwapBuffers();
    countfps++;

}
