/*   worldmap.cpp
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

#include "SDL_mixer.h"
#include "SDL_opengl.h"
#include "SDL.h"
#include <math.h>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <dirent.h>

#include "globals.h"
#include "bot.h"
#include "grenade.h"
#include "bullet.h"
#include "worldmap.h"
#include "regex.h"
#include "read_pms.h"
#include "soil/SOIL.h"

#include "fonts.h"

const float _180overpi = 57.29f;

Tex text_ammos[11];
const float multiplier_x = 3.3f;
const float multiplier_y = 3.5f;


Weapon weapon[20];



void WorldMap::moveBotRight(int bot_nr)
{
    // if (bot[bot_nr]->collision_det_with_wall(0, 1) >= 0)
    {
        bot[bot_nr]->movementType = BIEGA;
        bot[bot_nr]->velocity.x -= sWalking;
    }
}

void WorldMap::moveBotLeft(int bot_nr)
{
    //if (bot[bot_nr]->collision_det_with_wall(0, 1) >= 0)
    {
        bot[bot_nr]->movementType = BIEGA;
        bot[bot_nr]->velocity.x += sWalking;
    }
}

void WorldMap::moveBotUp(int bot_nr)
{
    // jump, if there is collision with the floor
    if (bot[bot_nr]->collision_det_with_wall(0, 1) >= 0)
    {
        bot[bot_nr]->movementType = SKOK;
        bot[bot_nr]->velocity.y = sJumping;
    }
}

void WorldMap::moveBotDown(int bot_nr)
{
    bot[bot_nr]->movementType = KUCA;
}

void WorldMap::moveBotJet(int bot_nr)
{
    bot[bot_nr]->velocity.y += sFlying;
}

void WorldMap::moveBotJumpLeft(int bot_nr)
{
    if (bot[bot_nr]->collision_det_with_wall(0, 1) >= 0)
    {
        bot[bot_nr]->movementType = SKOK_W_BOK;
        bot[bot_nr]->velocity.x += sJumping * 0.1f;
        bot[bot_nr]->velocity.y = sJumping * 0.5f;
    }
}

void WorldMap::moveBotJumpRight(int bot_nr)
{
    if (bot[bot_nr]->collision_det_with_wall(0, 1) >= 0)
    {
        bot[bot_nr]->movementType = SKOK_W_BOK;
        bot[bot_nr]->velocity.x -= sJumping * 0.1f;
        bot[bot_nr]->velocity.y = sJumping * 0.5f;
    }
    //cpBodyApplyForce(bot[bot_nr]->cBody, cpv(1000.0f, -10000.0f), cpvzero);
}

WorldMap::~WorldMap()
{

    // delete textures
    /*for (int i = 0; i < p.sceneryCount; i++)
    {
        delete text_scen[i];
    }*/
    delete text_scen;

    //cout << "Koniec" << endl;
//    int i;
    //for (i = 0; i < p.sceneryCount; i++) free(text_scen_xy[i]);
    //for (i = 0; i < p.sceneryCount; i++) delete[] text_scen_xy[i];
    //free(text_scen_xy);
    //delete[] text_scen_xy;

    ammo_list.clear();
    gren_list.clear();
    chat_list.clear();

    for (size_t i = 0; i < bot.size(); i++)
        delete bot[i];
    bot.clear();

    p.waypoint.clear();
    p.spawnpoint.clear();
    p.collider.clear();
    p.polygon.clear();
    p.sector.clear();
    p.prop.clear();
    p.scenery.clear();
    p.texture.clear();

    //Ammo *am_temp;
    //Grenade *gr_temp;


    /*for (list<Bullet>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        delete ammo_list.at(i);
    }
    for (list<Grenade>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        //gr_temp = &(*temp);
        temp = gren_list.erase(temp);
        //delete gr_temp;
    }*/

    if (SOUNDS_VOL > 0)
    {
        for (int i = 0; i < 16; ++i)
        {
            Mix_FreeChunk(weapon[i].fireSound);
            weapon[i].fireSound = NULL;
        }
        for (int i = 0; i < 11; ++i)
        {
            Mix_FreeChunk(weapon[i].reloadSound);
            weapon[i].reloadSound = NULL;
        }
        Mix_FreeChunk(grenade_explosion);
        grenade_explosion = NULL;
        Mix_FreeChunk(grenade_throw);
        grenade_throw = NULL;
        Mix_FreeChunk(sound_new_life);
        sound_new_life = NULL;
        Mix_FreeChunk(sound_heaven);
        sound_heaven = NULL;
        for (int i = 0; i < 3; ++i)
        {
            Mix_FreeChunk(sound_death[i]);
            sound_death[i] = NULL;
        }
    }
    if (MUSIC_VOL > 0)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    if (SOUNDS_VOL + MUSIC_VOL > 0)
        Mix_CloseAudio();

    font1_16.clean();
    font2_12.clean();
    font2_28.clean();

    //    glDeleteTextures( 1, &texture );
//    glDeleteTextures(p.sceneryCount, text_scen);
//    delete [] text_scen;

}


void WorldMap::run()
{
    init_gl();

    int countfps = 0;
    Uint32 movement_timer, timer_fps;
    SDL_WarpMouse(MAX_WIDTH/2, MAX_HEIGHT/2);


    if (SOUNDS_VOL > 0)
        Mix_PlayChannel(-1, sound_spawn, 0);

    movement_timer = timer_fps = lastOutChatEntity = SDL_GetTicks();

    while (!CHOICE_EXIT)
    {
        getCurrentTime = SDL_GetTicks();
        if (getCurrentTime - timer_fps >= 1000)
        {
            currentFPS = countfps;
            std::cout << "FPS : " << countfps << std::endl;
            countfps = 0;
            timer_fps = getCurrentTime;
        }
        if (getCurrentTime - movement_timer >= fTimeStepMS)
        {
            inputUser();
            bots_control();
            calculate_physics();
            movement_timer = getCurrentTime;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        draw_background();
        draw_gostek();
        draw_arms();
        draw_screen();
        draw_interface();
        draw_mouse();

        SDL_GL_SwapBuffers();
        countfps++;
    }
}
/*
void WorldMap::startServer()
{
    isServer = 1;
    //MY_BOT_NR = 0;
    //prepare_net_server();
    //TH_ser = SDL_CreateThread(server_thread, NULL);
}


void WorldMap::startClient()
{
    isServer = 0;

    // zapytaj serwera ile juz jest Botow
    //MY_BOT_NR = ask_server(HOW_MANY_BOTS);
   // MY_BOT_NR = 1;
    //prepare_net_client();
    //TH_cli = SDL_CreateThread(client_thread, NULL);
}
*/


WorldMap::WorldMap(const std::string& map, int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt)
{

    load_fonts();
    load_audio();
    load_map(map);
    load_spawnpoints();
    load_animations();
    load_weapons();
    load_textures();
    load_configs();
    load_bots(alpha_cnt, bravo_cnt, charlie_cnt, delta_cnt);

}

void WorldMap::addBot(const std::string& name, int spawn_nr, int gunmodel, TEAM team, unsigned int bot_nr)
{
    Bot *newbot = new Bot(bot_nr, name, spawn_nr, gunmodel, team);
    bot.push_back(newbot);
}

void WorldMap::addBot(const Bots& bots, int spawn_nr, TEAM team, unsigned int bot_nr)
{
    Bot *newbot = new Bot(bot_nr, bots.name, spawn_nr, 0, team);
    newbot->chatDead = bots.chatDead;
    newbot->chatKill = bots.chatKill;
    newbot->chatLowhealth = bots.chatLowhealth;
    newbot->chatSeeEnemy = bots.chatSeeEnemy;
    newbot->chatWinning = bots.chatWinning;
    bot.push_back(newbot);
}

void WorldMap::addGrenade(unsigned int bot_nr, float dest_x, float dest_y, Uint32 push_time)
{
    bot[bot_nr]->movementType = RZUCA;
    if (SOUNDS_VOL > 0)
        Mix_PlayChannel(-1, grenade_throw, 0);

//    float tang, sq;
    // fix it !!!!!!!
    bot[bot_nr]->shotPointX = bot[bot_nr]->position.x;
    bot[bot_nr]->shotPointY = bot[bot_nr]->position.y;

    Grenade *grenade = new Grenade(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, dest_x, dest_y, bot[bot_nr]->botNumber, push_time);
    //Grenade grenade(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, bot[bot_nr]->botNumber);
    /*   tang = (dest_y - bot[bot_nr]->shotPointY) /
              (dest_x - bot[bot_nr]->shotPointX);

       sq = 3*power / (float)sqrt(1+tang*tang);
       if (dest_x - bot[bot_nr]->shotPointX > 0)
       {
       //        grenade->velocity.x = -sq;
           //      grenade->velocity.y = -sq * tang;
       }
       else
       {
           //    grenade->velocity.x = sq;
           //  grenade->velocity.y = sq * tang;
       }*/
    gren_list.push_back(grenade);
    bot[bot_nr]->numGrenades--;

}


void WorldMap::gunReloading(int bot_nr)
{

    if (!bot[bot_nr]->isReloading)
    {
        if (SOUNDS_VOL > 0)
            Mix_PlayChannel(-1, weapon[bot[bot_nr]->gunModel].reloadSound, 0);
        bot[bot_nr]->startReloadingTime = getCurrentTime;
        bot[bot_nr]->isReloading = true;
        bot[bot_nr]->leftAmmos = 0;
    }
    if ((getCurrentTime - bot[bot_nr]->startReloadingTime) >= weapon[bot[bot_nr]->gunModel].reloadTime)
    {
        bot[bot_nr]->leftAmmos = weapon[bot[bot_nr]->gunModel].ammo;
        bot[bot_nr]->isReloading = false;
    }
    else
    {
        text_reloadbar.w = 115.0f * (getCurrentTime - bot[bot_nr]->startReloadingTime) / weapon[bot[MY_BOT_NR]->gunModel].reloadTime;
    }
}



void WorldMap::addBullet(unsigned int bot_nr, float dest_x, float dest_y)
{
    // fix it !!!!!!!
    bot[bot_nr]->shotPointX = bot[bot_nr]->position.x - bot[bot_nr]->w;
    bot[bot_nr]->shotPointY = bot[bot_nr]->position.y - bot[bot_nr]->h;

    if (getCurrentTime - bot[bot_nr]->lastShotTime >= weapon[bot[bot_nr]->gunModel].fireInterval && !bot[bot_nr]->isReloading && bot[bot_nr]->leftAmmos > 0)
    {
        if (SOUNDS_VOL > 0)
        {
            // make noise
            Sint16 angle = 0;
            int distance = 0;
            if (bot_nr != MY_BOT_NR)
            {
                angle = static_cast<Sint16>(_180overpi * atan2((bot[bot_nr]->position.y - bot[MY_BOT_NR]->position.y), (bot[bot_nr]->position.x - bot[MY_BOT_NR]->position.x))) + 90;
                distance = static_cast<int>(sqrt(pow((bot[bot_nr]->position.x - bot[MY_BOT_NR]->position.x), 2.0f) + pow((bot[bot_nr]->position.y - bot[MY_BOT_NR]->position.y), 2.0f)) / 4);
            }
            //std::cout << "KAT " << angle << std::endl;
            //std::cout << "ODL " << distance << std::endl;
            Mix_SetPosition(-1, angle, distance);
            Mix_PlayChannel(-1, weapon[bot[bot_nr]->gunModel].fireSound, 0);
        }
        Bullet *sbullet = new Bullet(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, dest_x, dest_y, bot[bot_nr]->gunModel, bot[bot_nr]->botNumber);
        bot[bot_nr]->leftAmmos--;
        ammo_list.push_back(sbullet);
        bot[bot_nr]->lastShotTime = getCurrentTime;
    }

    if (bot[bot_nr]->leftAmmos == 0) gunReloading(bot_nr);

}

std::string WorldMap::anim_type(MT name)
{
    switch (name)
    {
    case BIEGA:
        return SOL_PATH + "Anims/biega.poa";
    case BIEGA_TYL:
        return SOL_PATH + "Anims/biegatyl.poa";
    case CELUJE:
        return SOL_PATH + "Anims/celuje.poa";
    case CIESZY:
        return SOL_PATH + "Anims/cieszy.poa";
    case GORA:
        return SOL_PATH + "Anims/gora.poa";
    case KUCA:
        return SOL_PATH + "Anims/kuca.poa";
    case RZUCA:
        return SOL_PATH + "Anims/rzuca.poa";
    case SKOK:
        return SOL_PATH + "Anims/skok.poa";
    case SKOK_DOL_OBROT:
        return SOL_PATH + "Anims/skokdolobrot.poa";
    case SKOK_DOL_OBROT_TYL:
        return SOL_PATH + "Anims/skokdolobrottyl.poa";
    case SKOK_W_BOK:
        return SOL_PATH + "Anims/skokwbok.poa";
    case SPADA:
        return SOL_PATH + "Anims/spada.poa";
    case STOI:
        return SOL_PATH + "Anims/stoi.poa";
    case ZMIEN_BRON:
        return SOL_PATH + "Anims/zmienbron.poa";
        //case
    }
    //return SOL_PATH+"Anims/"+
    return NULL;
}




int WorldMap::read_poa(MT name)
{
    std::string buffer;
    int i = 0, k = 0;
    bool stop = false;
    float min_x = 100, max_x = -100, min_z = 100, max_z = -100;

    std::ifstream file(anim_type(name).c_str());

    if (!file.is_open())
    {
        std::cerr << "ERROR opening file " << anim_type(name) << std::endl;
        return -1;
    }

    while (!stop)
    {
        file >> buffer;
        if (buffer[0] != '\r' && buffer[0] != '\n' && buffer != "NEXTFRAME" && buffer != "ENDFILE")
        {
            file >> part_x[name][k][i][LEFT];
            file >> part_z[name][k][i][LEFT];
            file >> part_y[name][k][i][LEFT];

            part_x[name][k][i][LEFT] *= multiplier_x;
            part_y[name][k][i][LEFT] *= -multiplier_y;

            part_x[name][k][i][RIGHT] = -part_x[name][k][i][LEFT];
            part_y[name][k][i][RIGHT] = part_y[name][k][i][LEFT];
            part_z[name][k][i][RIGHT] = part_z[name][k][i][LEFT];

            if (part_x[name][k][i][RIGHT] < min_x) min_x = part_x[name][k][i][RIGHT];
            if (part_x[name][k][i][RIGHT] > max_x) max_x = part_x[name][k][i][RIGHT];
            if (part_z[name][k][i][RIGHT] < min_z) min_z = part_z[name][k][i][RIGHT];
            if (part_z[name][k][i][RIGHT] > max_z) max_z = part_z[name][k][i][RIGHT];

            k++;
        }
        else
        {
            if (buffer == "ENDFILE")
            {
                BODYPARTS_MAX[name] = k;
                stop = true;
            }
            else if (buffer == "NEXTFRAME")
            {
                i++;       // next frame
                k = 0;
            }
        }
    }

    SOL_WIDTH[name] = max_x - min_x;
    SOL_HEIGHT[name] = max_z - min_z;

    FRAMES_MAX[name] = i+1;

    file.close();

    return 0;

}


void WorldMap::init_gl()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, MAX_WIDTH, MAX_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glOrtho(bgX, bgX + MAX_WIDTH, bgY + MAX_HEIGHT, bgY, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



std::string WorldMap::findInterface(const char* name)
{
    char expr[128];
    int i = 0;
    std::string out;
    struct dirent *dp;
    DIR *dirp = opendir(INTERF_PATH.c_str());
    regex_t preg;

    if (dirp == NULL)
        return false;

    for (unsigned int k = 0; k < strlen(name); k++)
    {
        expr[i++] = '[';
        expr[i++] = toupper(name[k]);
        expr[i++] = tolower(name[k]);
        expr[i++] = ']';
    }
    expr[i] = '\0';

    regcomp(&preg, expr, REG_EXTENDED);
    while ((dp = readdir(dirp)) != NULL)
    {
        if (regexec(&preg, dp->d_name, 0, NULL, 0) == 0)
        {
            out.assign(INTERF_PATH + dp->d_name);
            closedir(dirp);
            regfree(&preg);
            return out;
        }
    }

    closedir(dirp);
    regfree(&preg);
    return "";
    //return "Error in findInterface function while loading " + name;
}

void WorldMap::getGLpos()
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY;
    GLdouble winZ = 0.0f;
    GLdouble posZ = 0;
    GLdouble tempx = 0.0f;
    GLdouble tempy = 0.0f;

    SDL_GetMouseState(&NMOUSE_X, &NMOUSE_Y);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = static_cast<float>(NMOUSE_X);
    winY = static_cast<float>(viewport[3] - NMOUSE_Y);
    glReadPixels(static_cast<GLint>(NMOUSE_X), static_cast<GLint>(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &tempx, &tempy, &posZ);

    MOUSE_X = static_cast<int>(tempx);
    MOUSE_Y = static_cast<int>(tempy);
}


Mix_Chunk* WorldMap::loadSoundFile(const std::string& file)
{
    Mix_Chunk *temp = NULL;
    if ((temp = Mix_LoadWAV(file.c_str())) == NULL)
    {
        std::cout << "  Cannot load sound file: " << file << ", " << Mix_GetError() << std::endl;
    }
    return temp;
}
