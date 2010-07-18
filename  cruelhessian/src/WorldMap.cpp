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


#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"

#include "WorldMap.h"
#include "Game.h"
#include "soil/SOIL.h"





void WorldMap::moveBotRight(unsigned int bot_nr)
{
    //int t1, t2;
    //if (!collisionCircle2Wall(*bot[bot_nr], 1, 0, t1, t2))
    {
        bot[bot_nr]->movementType = (bot[bot_nr]->movementDirection == RIGHT) ? BIEGA : BIEGA_TYL;
        bot[bot_nr]->velocity.x += sWalking;// - bot[bot_nr]->friction;
        //bot[bot_nr]->a.x += 5.0;
//        bot[bot_nr]->a.x += 800;
    }

}

void WorldMap::moveBotLeft(unsigned int bot_nr)
{
    //int t1, t2;
    //if (!collisionCircle2Wall(*bot[bot_nr], -1, 0, t1, t2))
    {
        //      bot[bot_nr]->a.x -= 800;
        bot[bot_nr]->movementType = (bot[bot_nr]->movementDirection == LEFT) ? BIEGA : BIEGA_TYL;
        bot[bot_nr]->velocity.x -= sWalking;// + bot[bot_nr]->friction;
        //bot[bot_nr]->a.x -= sWalking;
    }

}

void WorldMap::moveBotUp(unsigned int bot_nr)
{

    if (bot[bot_nr]->isAbleToJump)
    {
        //    bot[bot_nr]->a.y -= 200;
        bot[bot_nr]->movementType = SKOK;
        bot[bot_nr]->velocity.y -= 15*sJumping;
    }

}


void WorldMap::moveBotDown(unsigned int bot_nr)
{
    bot[bot_nr]->movementType = KUCA;
}


void WorldMap::moveBotJet(unsigned int bot_nr)
{

    if (bot[bot_nr]->isAbleToFly)
    {
        bot[bot_nr]->movementType = GORA;
        bot[bot_nr]->velocity.y -= sFlying;
        //bot[bot_nr]->a.y = -15*sFlying;
    }

}

void WorldMap::moveBotJumpLeft(unsigned int bot_nr)
{

    if (bot[bot_nr]->isAbleToJump)
    {
        bot[bot_nr]->movementType = SKOK_W_BOK;
        bot[bot_nr]->velocity.x += sJumping * 0.1f;
        bot[bot_nr]->velocity.y -= sJumping * 0.5f;
    }
}

void WorldMap::moveBotJumpRight(unsigned int bot_nr)
{

    if (bot[bot_nr]->isAbleToJump)
    {
        bot[bot_nr]->movementType = SKOK_W_BOK;
        bot[bot_nr]->velocity.x -= sJumping * 0.1f;
        bot[bot_nr]->velocity.y -= sJumping * 0.5f;
    }
    //cpBodyApplyForce(bot[bot_nr]->cBody, cpv(1000.0f, -10000.0f), cpvzero);
}


WorldMap::~WorldMap()
{

    // delete textures
    /*for (int i = 0; i < map->sceneryCount; i++)
    {
        delete text_scen[i];
    }*/
//    delete text_scen;

    //cout << "Koniec" << endl;
//    int i;
    //for (i = 0; i < p.sceneryCount; i++) free(text_scen_xy[i]);
    //for (i = 0; i < p.sceneryCount; i++) delete[] text_scen_xy[i];
    //free(text_scen_xy);
    //delete[] text_scen_xy;

    bullet_list.clear();
    bonus_list.clear();
    gren_list.clear();
//    chat_list.clear();

    for (size_t i = 0; i < bot.size(); i++)
        delete bot[i];
    bot.clear();

    delete map;
    delete backg;
    delete mouse;
    delete arrow;
    delete chat;
    delete window_scores;
    //Ammo *am_temp;
    //Grenade *gr_temp;


    /*for (list<Bullet>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        delete bullet_list.at(i);
    }
    for (list<Grenade>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        //gr_temp = &(*temp);
        temp = gren_list.erase(temp);
        //delete gr_temp;
    }*/

    if (game.SOUNDS_VOL > 0)
    {
        /*  for (int i = 0; i < 16; ++i)
          {
              Mix_FreeChunk(Weapons[i].fireSound);
              Weapons[i].fireSound = NULL;
          }
          for (int i = 0; i < 11; ++i)
          {
              Mix_FreeChunk(Weapons[i].reloadSound);
              Weapons[i].reloadSound = NULL;
          }*/
        Mix_FreeChunk(grenade_explosion);
        grenade_explosion = NULL;
        Mix_FreeChunk(grenade_bounce);
        grenade_bounce = NULL;
        Mix_FreeChunk(grenade_throw);
        grenade_throw = NULL;
        Mix_FreeChunk(sound_new_life);
        sound_new_life = NULL;
        Mix_FreeChunk(sound_heaven);
        sound_heaven = NULL;
        for (int i = 0; i < 2; ++i)
        {
            Mix_FreeChunk(sound_kitfall[i]);
            sound_kitfall[i] = NULL;
        }
        for (int i = 0; i < 3; ++i)
        {
            Mix_FreeChunk(sound_death[i]);
            sound_death[i] = NULL;
        }
    }
    if (game.MUSIC_VOL > 0)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    if (game.SOUNDS_VOL + game.MUSIC_VOL > 0)
        Mix_CloseAudio();

    //Weapons.clear();
//    Bots.clear();


    for (int i = 0; i < 2; ++i)
    {
        font[i][game.FontMenuSize].clean();
        font[i][game.FontConsoleSize].clean();
        font[i][game.FontBigSize].clean();
        font[i][game.FontWeaponMenuSize].clean();
        font[i][game.FontConsoleSmallSize].clean();
    }

//    delete [] text_scen;

}


void WorldMap::run()
{
    init_gl();

    SDL_WarpMouse(static_cast<Uint16>(game.MAX_WIDTH/2), static_cast<Uint16>(game.MAX_HEIGHT/2));

    if (game.SOUNDS_VOL > 0)
        Mix_PlayChannel(-1, sound_spawn, 0);

    Uint32 firstFrameFPS, firstFrameSYS;
    Uint32 lastTime = firstFrameFPS = getStartGameTime = firstFrameSYS = SDL_GetTicks();
    Uint32 frames = 0, dtFPS;
    //  const Uint32 step = 5;

    std::cout << " -- Starting game --" << std::endl << std::endl;

    while (!CHOICE_EXIT)
    {

        getCurrentTime = SDL_GetTicks();

        fTimeStep = static_cast<float>(getCurrentTime - lastTime) / 1000.0f;

        lastTime = getCurrentTime;

        frames++;

        dtFPS = getCurrentTime - firstFrameFPS;
        if (dtFPS >= 1000)
        {
            currentFPS = 1000 * frames / dtFPS;
            frames = 0;
            firstFrameFPS = getCurrentTime;
        }

        //if (getCurrentTime - firstFrameSYS >= step)
        {
            //  fTimeStep *= step;
            inputUser();
            bots_control();
            collisions();

            //for (std::list<Bullet *>::iterator it = bullet_list.begin(); it != bullet_list.end(); ++it) (*it)->update();
            for (std::list<Bullet *>::iterator it = bullet_list.begin(); it != bullet_list.end(); ++it) (*it)->AccumulateForces();
            for (std::list<Grenade *>::iterator it = gren_list.begin(); it != gren_list.end(); ++it) (*it)->update();
            for (std::list<Grenade *>::iterator it = gren_list.begin(); it != gren_list.end(); ++it) (*it)->AccumulateForces();

            for (std::vector<Bot *>::iterator it = bot.begin(); it != bot.end(); ++it) (*it)->update();

            for (std::vector<Bot *>::iterator it = bot.begin(); it != bot.end(); ++it) (*it)->AccumulateForces();

            //for (std::list<Bonus *>::iterator it = bonus_list.begin(); it != bonus_list.end(); ++it) (*it)->update();
            for (std::list<Bonus *>::iterator it = bonus_list.begin(); it != bonus_list.end(); ++it) (*it)->AccumulateForces();


            for (std::list<Bullet *>::iterator it = bullet_list.begin(); it != bullet_list.end(); ++it) (*it)->Verlet();
            for (std::list<Grenade *>::iterator it = gren_list.begin(); it != gren_list.end(); ++it) (*it)->Verlet();
            for (std::vector<Bot *>::iterator it = bot.begin(); it != bot.end(); ++it) (*it)->Verlet();
            for (std::list<Bonus *>::iterator it = bonus_list.begin(); it != bonus_list.end(); ++it) (*it)->Verlet();


            arrow->update(bot[MY_BOT_NR]->position);
            mouse->update();
            window_scores->update(bot);
            backg->update(mouse->getGlobalPosition(), bot[MY_BOT_NR]->position);


            game_control();   // time, flags, bonuses, kills ...
            // firstFrameSYS = getCurrentTime;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        backg->draw(*map);

        glEnable(GL_TEXTURE_2D);

        map->drawBack();
        for (std::list<Bullet *>::iterator it = bullet_list.begin(); it != bullet_list.end(); ++it) (*it)->draw();
        for (std::list<Grenade *>::iterator it = gren_list.begin(); it != gren_list.end(); ++it) (*it)->draw();
        for (std::vector<Bot *>::iterator it = bot.begin(); it != bot.end(); ++it) (*it)->draw();
        for (std::list<Bonus *>::iterator it = bonus_list.begin(); it != bonus_list.end(); ++it) (*it)->draw();
        map->drawFront();

        arrow->draw();

        glDisable(GL_TEXTURE_2D);

        draw_interface();

        mouse->draw();

        SDL_GL_SwapBuffers();

    }
}


void WorldMap::insertMe(TEAM team)
{

    BotsBase temp;

    temp.name = game.PLAYER_NAME;
    temp.color[SKIN]  = game.COLOR_SKIN;
    temp.color[HAIR]  = game.COLOR_HAIR;
    temp.color[PANTS] = game.COLOR_PANTS;

    if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == TM)
    {
        temp.color[SHIRT] = textCol[team];
    }
    else
        temp.color[SHIRT] = game.COLOR_SHIRT;


    int point = static_cast<int>(rand()%spawnpoint[team].size());
    MY_BOT_NR = addBot(temp, spawnpoint[team][point], team);

    backg = new Background(static_cast<float>(game.MAX_WIDTH/2 - map->spawnpoint[spawnpoint[team][point]].x),
                           static_cast<float>(game.MAX_HEIGHT/2 - map->spawnpoint[spawnpoint[team][point]].y), bot[MY_BOT_NR]->position);

    window_scores = new WindowScores(text_deaddot, text_smalldot, MY_BOT_NR);
}


WorldMap::WorldMap(const std::string& mapp, int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt)
{

    load_fonts();
    load_map(mapp);
    load_textures();
    load_configs();
    load_audio();
    load_spawnpoints();
    load_bonuses();

    load_bots(alpha_cnt, TEAM_ALPHA);
    load_bots(bravo_cnt, TEAM_BRAVO);
    load_bots(charlie_cnt, TEAM_CHARLIE);
    load_bots(delta_cnt, TEAM_DELTA);

    insertMe(TEAM_ALPHA);

    run();

}



WorldMap::WorldMap(const std::string& mapp, int bots_cnt)
{

    load_fonts();
    load_map(mapp);
    load_textures();
    load_configs();
    load_audio();
    load_spawnpoints();
    load_bonuses();

    load_bots(bots_cnt, TEAM_GENERAL);

    insertMe(TEAM_GENERAL);

    run();

}


unsigned int WorldMap::addBot(const BotsBase& botss, int spawn_nr, TEAM team)
{
    Bot *newbot = new Bot(botss.name, static_cast<float>(map->spawnpoint[spawn_nr].x), static_cast<float>(map->spawnpoint[spawn_nr].y), 0, team, bot.size(), nearestWaypoint(spawn_nr));
    newbot->chatDead = botss.chatDead;
    newbot->chatKill = botss.chatKill;
    newbot->chatLowhealth = botss.chatLowhealth;
    newbot->chatSeeEnemy = botss.chatSeeEnemy;
    newbot->chatWinning = botss.chatWinning;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j)
            newbot->color[i][j] = botss.color[i][j];

    if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == TM)
    {
        for (int j = 0; j < 3; ++j)
            newbot->color[SHIRT][j] = textCol[team][j];
    }

    bot.push_back(newbot);

    return newbot->number;
}


void WorldMap::addGrenade(unsigned int bot_nr, const TVector2D& dest, Uint32 push_time)
{
    bot[bot_nr]->movementType = RZUCA;
    if (game.SOUNDS_VOL > 0)
        Mix_PlayChannel(-1, grenade_pullout, 0);

    // fix it !!!!!!!!!!!!!!!!!!
    if (game.SOUNDS_VOL > 0)
        Mix_PlayChannel(-1, grenade_throw, 0);

//    float tang, sq;
    // fix it !!!!!!!
    bot[bot_nr]->shotPoint = bot[bot_nr]->position;

    Grenade *grenade = new Grenade(bot[bot_nr]->shotPoint, dest, bot_nr, push_time, text_grenade);


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


void WorldMap::gunReloading(unsigned int bot_nr)
{

    if (!bot[bot_nr]->isReloading)
    {
        if (game.SOUNDS_VOL > 0)
            Mix_PlayChannel(-1, Weapons[bot[bot_nr]->gunModel].reloadSound, 0);
        bot[bot_nr]->startReloadingTime = getCurrentTime;
        bot[bot_nr]->isReloading = true;
        bot[bot_nr]->leftAmmos = 0;
    }
    if ((getCurrentTime - bot[bot_nr]->startReloadingTime) >= Weapons[bot[bot_nr]->gunModel].reloadTime)
    {
        bot[bot_nr]->leftAmmos = Weapons[bot[bot_nr]->gunModel].ammo;
        bot[bot_nr]->isReloading = false;
    }
    else
    {
        text_reloadbar.w = 115.0f * (getCurrentTime - bot[bot_nr]->startReloadingTime) / Weapons[bot[MY_BOT_NR]->gunModel].reloadTime;
    }

}



void WorldMap::addBullet(unsigned int bot_nr, const TVector2D& dest)
{
    // fix it !!!!!!!
    bot[bot_nr]->shotPoint = TVector2D(bot[bot_nr]->position.x - bot[bot_nr]->w, bot[bot_nr]->position.y - bot[bot_nr]->h);

    if (getCurrentTime - bot[bot_nr]->lastShotTime >= Weapons[bot[bot_nr]->gunModel].fireInterval && !bot[bot_nr]->isReloading && bot[bot_nr]->leftAmmos > 0)
    {
        if (game.SOUNDS_VOL > 0)
        {
            // make noise
            Sint16 angle = 0;
            Uint8 dist = 0;
            if (bot_nr != MY_BOT_NR)
            {
                angle = static_cast<Sint16>(_180overpi * atan2((bot[bot_nr]->position.y - bot[MY_BOT_NR]->position.y), (bot[bot_nr]->position.x - bot[MY_BOT_NR]->position.x))) + 90;
                dist = static_cast<Uint8>(sqrt(pow((bot[bot_nr]->position.x - bot[MY_BOT_NR]->position.x), 2.0f) + pow((bot[bot_nr]->position.y - bot[MY_BOT_NR]->position.y), 2.0f)) / 4);
            }
            //std::cout << "KAT " << angle << std::endl;
            //std::cout << "ODL " << distance << std::endl;
            Mix_SetPosition(-1, angle, dist);
            Mix_PlayChannel(-1, Weapons[bot[bot_nr]->gunModel].fireSound, 0);
        }
        //std::cout << "KAT " << std::endl;
        Bullet *sbullet = new Bullet(bot[bot_nr]->shotPoint, dest, bot[bot_nr]->gunModel, bot_nr, Weapons[bot[bot_nr]->gunModel].speed, Weapons[bot[bot_nr]->gunModel].textureAmmo);
        bot[bot_nr]->leftAmmos--;
        bullet_list.push_back(sbullet);
        // m_objects.push_back(sbullet);
        bot[bot_nr]->lastShotTime = getCurrentTime;
    }

    if (bot[bot_nr]->leftAmmos == 0) gunReloading(bot_nr);

}




void WorldMap::init_gl()
{
    std::cout << "Loading GL context" << std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glViewport(0, 0, static_cast<int>(game.MAX_WIDTH), static_cast<int>(game.MAX_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glOrtho(0, game.MAX_WIDTH, game.MAX_HEIGHT, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}





int WorldMap::nearestWaypoint(int spawn_nr)
{

    double min_val = 0.0;
    int min_nr = 0;
    int i, j;

    for (i = 1; i < map->waypointCount; i++)
    {
        if (map->waypoint[i].path == 1 && (map->waypoint[i].right || map->waypoint[i].left || map->waypoint[i].up || map->waypoint[i].down || map->waypoint[i].jet))
        {
            min_val = pow(map->spawnpoint[spawn_nr].x-map->waypoint[i].x, 2.0) + pow(map->spawnpoint[spawn_nr].y-map->waypoint[i].y, 2.0);
            min_nr = i;
            break;
        }
    }

    for (j = i+1; j < map->waypointCount; j++)
    {
        if ((pow(map->spawnpoint[spawn_nr].x-map->waypoint[j].x, 2.0) + pow(map->spawnpoint[spawn_nr].y-map->waypoint[j].y, 2.0) < min_val) &&
                (map->waypoint[i].path == 1 && (map->waypoint[j].right || map->waypoint[j].left || map->waypoint[j].up || map->waypoint[j].down || map->waypoint[j].jet)))
        {
            min_val = pow(map->spawnpoint[spawn_nr].x-map->waypoint[j].x, 2.0) + pow(map->spawnpoint[spawn_nr].y-map->waypoint[j].y, 2.0);
            min_nr = j;
        }
    }
    return min_nr;

}



void WorldMap::game_control()
{

    switch (game.CURRENT_GAME_MODE)
    {
    case DM :
    {
        for (unsigned int i = 0; i < bot.size(); ++i)
        {
            // jesli jakis zespol osiagnal limit zabic, pokaz staty i zakoncz
            if (bot[i]->killedNr >= game.FIRST_LIMIT)
            {
                SHOW_SCORES = true;
                CHOICE_EXIT = true;
                break;
            }
        }
        break;
    }
    case PM :
    {
        for (unsigned int i = 0; i < bot.size(); ++i)
        {
            // jesli jakis zespol osiagnal punktow, pokaz staty i zakoncz
            if (bot[i]->points >= game.FIRST_LIMIT)
            {
                SHOW_SCORES = true;
                CHOICE_EXIT = true;
                break;
            }
        }
        break;
    }
    case TM :
    {
        for (unsigned int i = 0; i < bot.size(); ++i)
        {
            // jesli jakis zespol osiagnal punktow, pokaz staty i zakoncz
            if (bot[i]->points >= game.FIRST_LIMIT)
            {
                SHOW_SCORES = true;
                CHOICE_EXIT = true;
                break;
            }
        }
        break;
    }
    case RM :
    {
        break;
    }
    case CTF :
    {
        break;
    }
    case HTF :
    {
        break;
    }
    case INF :
    {
        break;
    }
    default :
    {
        break;
    }
    }

    // time limit
    Uint32 time_to_end = game.LIMIT_TIME - (getCurrentTime - getStartGameTime) / 1000;    // liczone w sekundach
    if (time_to_end > 0)
    {
        if (time_to_end != prev_time)
        {
            // czy pozostaly czas jest wielokrotnoscia 60 sekund ( >= 2 )
            if (time_to_end % 60 == 0 && time_to_end != 60)
            {
                std::ostringstream oss;
                oss << time_to_end / 60;
                chat->addMessage("Time to end : " + oss.str() + " minutes");
                prev_time = time_to_end;
            }
            else if (time_to_end == 60 || time_to_end == 50 || time_to_end == 40 || time_to_end == 30 || time_to_end == 20 || time_to_end <= 10)
            {
                std::ostringstream oss;
                oss << time_to_end;
                chat->addMessage("Time to end : " + oss.str() + " seconds");
                prev_time = time_to_end;
            }
        }
    }
    else
    {
        SHOW_SCORES = true;
        CHOICE_EXIT = true;
    }

    unsigned int LEADING_PLAYER = 0, SECOND_PLAYER = 0;

    if (!window_scores->scores.empty())
    {
        for (unsigned int j = 0; j < window_scores->scores.size(); ++j)
        {
            if (window_scores->scores[j]->name == bot[MY_BOT_NR]->name)
                MY_CURRENT_POS = j+1;
        }

        for (unsigned int j = 0; j < bot.size(); ++j)
        {
            if (bot[j]->name == window_scores->scores[0]->name)
                LEADING_PLAYER = j;
            else if (bot[j]->name == window_scores->scores[1]->name)
                SECOND_PLAYER = j;
        }
    }


    if (game.CURRENT_GAME_MODE == DM)
    {
        if (MY_CURRENT_POS != 1)
            DISTANCE_SCORE = bot[MY_BOT_NR]->killedNr - bot[LEADING_PLAYER]->killedNr;
        else
            DISTANCE_SCORE = bot[MY_BOT_NR]->killedNr - bot[SECOND_PLAYER]->killedNr;
    }
    else
    {
        if (MY_CURRENT_POS != 1)
            DISTANCE_SCORE = bot[MY_BOT_NR]->points - bot[LEADING_PLAYER]->points;
        else
            DISTANCE_SCORE = bot[MY_BOT_NR]->points - bot[SECOND_PLAYER]->points;
    }

    // aktualizacja listy bonusow

}


void WorldMap::printTextMiddle(freetype::font_data& fontx, const std::string& text, unsigned int* color, float y)
{
    printText(fontx, text, color, game.MAX_WIDTH/2 - text.length() / 2 - 50, y);
}


void WorldMap::printText(freetype::font_data& fontx, const std::string& text, unsigned int* color, float x, float y)
{
    glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    glLoadIdentity();
    freetype::print(fontx, x, game.MAX_HEIGHT-y-12.0f, text.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}
