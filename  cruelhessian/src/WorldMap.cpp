/*   WorldMap.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
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
#include <cstdarg>
#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"

#include "WorldMap.h"
#include "WindowGuns.h"
#include "WindowScores.h"
#include "Background.h"
#include "Bonus.h"
#include "Bot.h"
#include "Body.h"
#include "Bullet.h"
#include "textinout/TextOutput.h"
#include "Weapon.h"
#include "Grenade.h"
#include "Cluster.h"
#include "AudioManager.h"
#include "BonusManager.h"
#include "WeaponManager.h"
#include "WindowExit.h"
#include "BotManager.h"
#include "Arrow.h"
#include "Mouse.h"
#include "Map.h"
#include "ParserManager.h"
#include "FontManager.h"
#include "InterfaceBaseManager.h"
#include "InterfaceManager.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "SpatialManager.h"
#include "RenderManager.h"
#include "soil/SOIL.h"


const float _180overpi = 57.29f;





WorldMap::~WorldMap()
{

    for (unsigned int i = 0; i < bot.size(); ++i)
        delete bot[i];
    bot.clear();

    if (InterfaceManager::GetSingletonPtr() != NULL)
        delete interfaceMgr;

    delete interfaceBaseMgr;

    delete renderMgr;

    delete spatialMgr;
    delete mapBack;
    delete mapFront;
    delete map;
    delete physicsMgr;
    delete backg;
    delete mouse;
    delete arrow;

    delete window_scores;
    if (WindowExit::GetSingletonPtr() != NULL)
        delete window_exit;

    delete text_output;

}



void WorldMap::run()
{

    setGL();

    game.App.SetCursorPosition(static_cast<int>(Parser.MAX_WIDTH/2.0f), static_cast<int>(Parser.MAX_HEIGHT/2.0f));
    game.App.ShowMouseCursor(false);

// --------------------------------------   start playing audio  --------------------------------------

    if (Parser.SOUNDS_VOL > 0)
    {
        Audio.Play(Audio.sound_spawn);
        if (Parser.MUSIC_VOL > 0)
        {
            Audio.playMusic(0);
            Audio.music->SetVolume(Parser.MUSIC_VOL);
        }
    }


// ------------------------------------------------------------------------------------------------------

    sf::Clock Clock;

    float firstFrameFPS, firstFrameSYS;
    float lastTime = firstFrameFPS = getStartGameTime = firstFrameSYS = 1000.f*Clock.GetElapsedTime();
    int frames = 0;
    float dtFPS;

    std::cout << " -- Starting game --" << std::endl << std::endl;

    while (!CHOICE_EXIT)
    {

        getCurrentTime = 1000*Clock.GetElapsedTime();

        lastTime = getCurrentTime;

        frames++;

        dtFPS = getCurrentTime - firstFrameFPS;
        if (dtFPS >= 1000)
        {
            currentFPS = static_cast<int>(1000 * frames / dtFPS);
            frames = 0;
            firstFrameFPS = getCurrentTime;
        }

        if (getCurrentTime - firstFrameSYS >= 17)          // ca. 1000/60
        {

            inputUser();

            bots_control();

            Physics.GameUpdate(0.01667f);   // ca. 1/60

            arrow->Update(bot[MY_BOT_NR]->GetPosition());
            mouse->Update();
            window_scores->Update();

            backg->Update(mouse->getGlobalPosition(), bot[MY_BOT_NR]->GetPosition());

            game_control();   // time, flags, bonuses, kills ...

            firstFrameSYS = getCurrentTime;
        }
        //Renderer.AddToDraw(backg);

        RendererCH.Draw();

    }



// --------------------------------------   stop playing audio  --------------------------------------
    if (Parser.MUSIC_VOL > 0)
    {
        //Mix_HaltMusic();
        Audio.music->Stop();
        //Mix_FreeMusic(Audio.music);
    }

// ------------------------------------------------------------------------------------------------------

}



WorldMap::WorldMap(const std::string& mapp) :
    REST_TIME(""),
    CHOICE_GUN(false),
    SHOW_STATS(true),
    OLD_POS(TVector2D(0, 0)),
    MY_CURRENT_POS(0),
    DISTANCE_SCORE(0),
    currentFPS(0),
    SHOW_SCORES(false),
    MY_BOT_NR(0),
    YOU_KILLED(false),
    getCurrentTime(0),
    CHOICE_EXIT(false),
    FRIENDLY_FIRE(false),
    getStartGameTime(0),
    prev_time(0),
    MAX_RESPAWN_TIME(5),
    JET_CHANGE(0.005f)
{

    physicsMgr = new PhysicsManager();
    map = new Map(mapp);
    mapFront = new MapFront(*map);
    mapBack = new MapBack(*map);
    spatialMgr = new SpatialManager();
    Spatial.AddStatic();
    renderMgr = new RenderManager();
}


void WorldMap::SetPlayers(int amount, ...)
{
    va_list vl;
    va_start(vl, amount);


    // --------------------------------------   loading interface  ------------------------------------------
    interfaceBaseMgr = new InterfaceBaseManager();

    if (Parser.INTERFACE_NAME != "--None--")
        interfaceMgr = new InterfaceManager();
    else
        std::cout << "Load limited interface" << std::endl;
// ------------------------------------------------------------------------------------------------------

    mouse = new Mouse();
    text_output = new TextOutput();
    window_guns = new WindowGuns();
    arrow = new Arrow();

    load_spawnpoints();
    Bonuses.LoadBonuses();

    if (amount == 1)
    {
        Bots.LoadBots(va_arg(vl, int), TEAM_GENERAL);
        Bots.LoadBots(TEAM_GENERAL);
    }
    else if (amount == 4)
    {
        Bots.LoadBots(va_arg(vl, int), TEAM_ALPHA);
        Bots.LoadBots(va_arg(vl, int), TEAM_BRAVO);
        Bots.LoadBots(va_arg(vl, int), TEAM_CHARLIE);
        Bots.LoadBots(va_arg(vl, int), TEAM_DELTA);

        Bots.LoadBots(TEAM_ALPHA);
    }
    va_end(vl);

    window_scores = new WindowScores();

    run();

}


unsigned int WorldMap::addBot(const BotsBase& botss, int spawn_nr, TEAM team)
{
    TVector2D spawn = TVector2D(static_cast<float>(map->spawnpoint[spawn_nr].x), static_cast<float>(map->spawnpoint[spawn_nr].y));
    Bot *newbot = new Bot(botss.name, spawn, 0, team, bot.size(), nearestWaypoint(spawn_nr));
    newbot->chatDead = botss.chatDead;
    newbot->chatKill = botss.chatKill;
    newbot->chatLowhealth = botss.chatLowhealth;
    newbot->chatSeeEnemy = botss.chatSeeEnemy;
    newbot->chatWinning = botss.chatWinning;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j)
        {
            newbot->color[i][j] = botss.color[i][j];
        }


    if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == TM)
    {
        for (int j = 0; j < 3; ++j)
        {
            newbot->color[SHIRT][j] = Fonts.textCol[team][j];
        }
    }

    bot.resize(bot.size()+1);
    bot[bot.size()-1] = newbot;

    Physics.m_movingObj.push_back(newbot);

    //std::cout << "DF " << bot[bot.size()] <<

    //bot.push_back(newbot);

    return newbot->number;
}







void WorldMap::gunReloading(unsigned int bot_nr)
{

    if (!bot[bot_nr]->isReloading)
    {
        if (Parser.SOUNDS_VOL > 0)
            Audio.Play(Audio.reloadSound[bot[bot_nr]->gunModel]);
        bot[bot_nr]->startReloadingTime = getCurrentTime;
        bot[bot_nr]->isReloading = true;
        bot[bot_nr]->leftAmmos = 0;
    }
    if ((getCurrentTime - bot[bot_nr]->startReloadingTime) >= Weapons[bot[bot_nr]->gunModel].reloadTime)
    {
        bot[bot_nr]->leftAmmos = Weapons[bot[bot_nr]->gunModel].ammo;
        bot[bot_nr]->isReloading = false;
    }
    else if (bot_nr == MY_BOT_NR)
    {
        if (InterfaceManager::GetSingletonPtr() != NULL)
            Interface.ReloadBar();
    }

}



void WorldMap::setGL()
{
    std::cout << "Loading GL context ... " << std::flush;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //  glViewport(0, 0, static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Parser.MAX_WIDTH, Parser.MAX_HEIGHT, 0, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    std::cout << "DONE" << std::endl;
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
            if (bot[i]->killedNr >= Parser.FIRST_LIMIT)
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
            // jesli jakis zespol osiagnal limit punktow, pokaz staty i zakoncz
            if (bot[i]->points >= Parser.FIRST_LIMIT)
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
            if (bot[i]->points >= Parser.FIRST_LIMIT)
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
    int time_to_end = Parser.LIMIT_TIME - static_cast<int>((getCurrentTime - getStartGameTime) / 1000.f);    // liczone w sekundach
    if (time_to_end > 0)
    {
        if (time_to_end != prev_time)
        {
            // czy pozostaly czas jest wielokrotnoscia 60 sekund ( >= 2 )
            if (time_to_end % 60 == 0 && time_to_end != 60)
            {
                std::ostringstream oss;
                oss << time_to_end / 60;
                text_output->AddMessage("Time to end : " + oss.str() + " minutes");
                prev_time = time_to_end;
            }
            else if (time_to_end == 60 || time_to_end == 50 || time_to_end == 40 || time_to_end == 30 || time_to_end == 20 || time_to_end <= 10)
            {
                std::ostringstream oss;
                oss << time_to_end;
                text_output->AddMessage("Time to end : " + oss.str() + " seconds");
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


int WorldMap::takeScreenshot()
{
    std::string screens_dir = game.CH_HOME_DIRECTORY + "Screens/";
    std::ostringstream oss;
    boost::filesystem::create_directory(screens_dir);
    std::string filename = screens_dir + "screen", temp;

    oss << rand()%RAND_MAX;
    filename += oss.str();
    filename += ".bmp";

    if (SOIL_save_screenshot(filename.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, static_cast<int>(Parser.MAX_WIDTH), static_cast<int>(Parser.MAX_HEIGHT)) == 0)
    {
        text_output->AddMessage("Cannot save screenshot");
        return -1;
    }
    else
    {
        text_output->AddMessage("Screenshot saved as 'screen" + oss.str() + ".bmp'");
        return 0;
    }

}




void WorldMap::load_spawnpoints()
{
    std::cout << "Loading spawnpoints ..." << std::endl;

    spawnpoint.resize(5);

    for (int i = 0; i < map->spawnpointCount; ++i)
    {
        if (map->spawnpoint[i].active)
        {
            if (map->spawnpoint[i].team == map->stGENERAL) spawnpoint[TEAM_GENERAL].push_back(i);
            else if (map->spawnpoint[i].team == map->stALPHA) spawnpoint[TEAM_ALPHA].push_back(i);
            else if (map->spawnpoint[i].team == map->stBRAVO) spawnpoint[TEAM_BRAVO].push_back(i);
            else if (map->spawnpoint[i].team == map->stCHARLIE) spawnpoint[TEAM_CHARLIE].push_back(i);
            else if (map->spawnpoint[i].team == map->stDELTA) spawnpoint[TEAM_DELTA].push_back(i);
        }
    }

}
