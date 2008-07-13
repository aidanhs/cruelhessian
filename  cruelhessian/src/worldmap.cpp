/*   worldmap.cpp
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
#include "SDL_opengl.h"
#include <math.h>
#include <list>
#include <sstream>
#include "draw_extra.h"
#include "globals.h"
#include "gun.h"
#include "physics.h"
#include "controls.h"
#include "bot.h"
#include "grenade.h"
#include "bullet.h"
#include "worldmap.h"


WorldMap *world;



float SOL_WIDTH[50], SOL_HEIGHT[50];
float AMMO_WIDTH[20], AMMO_HEIGHT[20];

WorldMap::WorldMap()
{
    this->read_map(MAP);
    this->TimeStepSize = 0.008;
    this->sGravity = 9.80665;
    this->sDrag = 5;
    this->sDragWalking = 100;
    this->sWalking = 0.75;
    this->sFlying = 50.0;
    this->sJumping = 4.0;
    this->MAX_RESPAWN_TIME = 5;
    this->REL_TIME = 8.0;
    this->BOTS_NUM = 2;
    this->GUN_MENU_START_X = (float)MAX_WIDTH/4;
    this->GUN_MENU_START_Y = (float)MAX_HEIGHT/7;
    this->isServer = 1;
    this->MY_BOT_NR = 0;
    this->CHOICE_GUN = false;
    this->CHOICE_GUN_MENU = true;
    this->CHOICE_EXIT = false;
    this->CHOICE_ESC = false;
   // this->GAME_LOOP = false;
    this->bgX = p.spawnpoint[MY_BOT_NR].x - MAX_WIDTH/2;
    this->bgY = p.spawnpoint[MY_BOT_NR].y - MAX_HEIGHT/2;
    this->oblicz_min_max();
    this->read_animations();
    this->load_textures();
    this->init_gl();
}

WorldMap::~WorldMap()
{
    int i;
    //for (i = 0; i < p.sceneryCount; i++) free(text_scen_xy[i]);
    for (i = 0; i < p.sceneryCount; i++) delete[] text_scen_xy[i];
    //free(text_scen_xy);
    delete[] text_scen_xy;

    ammo_list.clear();
    gren_list.clear();
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
    for (i = 0; i < world->BOTS_NUM; i++) delete bot[i];
    //delete [] bot;
/*
    for (list<Ammo>::iterator temp = ammo_list.begin(); temp != ammo_list.end(); temp++)
    {
        am_temp = &(*temp);
        temp = ammo_list.erase(temp);
        delete am_temp;
    }
    for (list<Grenade>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        //gr_temp = &(*temp);
        temp = gren_list.erase(temp);
        //delete gr_temp;
    }*/

    //TTF_CloseFont(font1_16);
    //TTF_CloseFont(font2_12);
    //TTF_CloseFont(font2_28);
    //TTF_Quit();
    //    glDeleteTextures( 1, &texture );
    glDeleteTextures(p.sceneryCount, text_scen);
    delete [] text_scen;
}




void WorldMap::add_grenade(int bot_nr, float dest_x, float dest_y, float power)
{
    float tang, sq;
    // fix it !!!!!!!
    bot[bot_nr]->shotPointX = bot[bot_nr]->position.x+bot[bot_nr]->w/2;
    bot[bot_nr]->shotPointY = bot[bot_nr]->position.y+bot[bot_nr]->h/2;

    Grenade *grenade = new Grenade(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, bot[bot_nr]->botNumber);
    //Grenade grenade(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, bot[bot_nr]->botNumber);
    tang = (dest_y - bot[bot_nr]->shotPointY) /
           (dest_x - bot[bot_nr]->shotPointX);

    sq = 3*power / (float)sqrt(1+tang*tang);
    if (dest_x - bot[bot_nr]->shotPointX > 0)
    {
        grenade->velocity.x = -sq;
        grenade->velocity.y = -sq * tang;
    }
    else
    {
        grenade->velocity.x = sq;
        grenade->velocity.y = sq * tang;
    }
    gren_list.push_back(grenade);
    bot[bot_nr]->numGrenades--;
}


// dest_x, dest_y - destination point
void WorldMap::add_bullet(int bot_nr, float dest_x, float dest_y)
{

    // fix it !!!!!!!
    bot[bot_nr]->shotPointX = bot[bot_nr]->position.x+bot[bot_nr]->w/2;
    bot[bot_nr]->shotPointY = bot[bot_nr]->position.y+bot[bot_nr]->h/2;

    if (bot[bot_nr]->leftAmmos == 0)
    {
        if (!bot[bot_nr]->isReloading)
        {
            bot[bot_nr]->startReloadingTime = SDL_GetTicks();
            bot[bot_nr]->isReloading = true;
        }
        if ((SDL_GetTicks() - bot[bot_nr]->startReloadingTime) >= gun_reload_time(bot[bot_nr]->gunModel))
        {
            bot[bot_nr]->leftAmmos = gun_ammos(bot[bot_nr]->gunModel);
            bot[bot_nr]->isReloading = false;
        }
    }

    else if (SDL_GetTicks() - bot[bot_nr]->lastShotTime >= gun_interval_time(bot[bot_nr]->gunModel))
    {
        float tang, sq;

        //bot[bot_nr]->isShooting = true;
        bot[bot_nr]->lastShotTime = SDL_GetTicks();
        Bullet *ammo = new Bullet(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, bot[bot_nr]->gunModel, bot[bot_nr]->botNumber);
        //Ammo ammo(bot[bot_nr]->shotPointX, bot[bot_nr]->shotPointY, bot[bot_nr]->gunModel, bot[bot_nr]->botNumber);

        tang = (dest_y - bot[bot_nr]->shotPointY) /
               (dest_x - bot[bot_nr]->shotPointX);

        sq = gun_speed(bot[bot_nr]->gunModel) / (float)sqrt(1+tang*tang);
        if (dest_x - bot[bot_nr]->shotPointX > 0)
        {
            ammo->velocity.x = -sq;
            ammo->velocity.y = -sq * tang;
        }
        else
        {
            ammo->velocity.x = sq;
            ammo->velocity.y = sq * tang;
        }

        bot[bot_nr]->leftAmmos--;
        ammo_list.push_back(ammo);
        //bot[bot_nr]->isShooting = false;
    }
}
