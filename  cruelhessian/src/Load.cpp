/*   Load.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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

#include <sstream>
#include <ctime>
#include "boost/filesystem/fstream.hpp"
#include "regexp.h"
#include "WorldMap.h"
#include "BonusManager.h"
#include "ParserManager.h"
#include "TexturesLoader.h"
#include "WeaponManager.h"
#include "InterfaceBaseManager.h"
#include "Mouse.h"
#include "parser/SimpleIni.h"



void WorldMap::load_configs()
{
    std::cout << "Loading configs ..." << std::endl;

    /* aGravity = TVector2D(0.0f, 9.81f);
     aWalkingLeft = TVector2D(8.0f, 0.0f);
     aWalkingRight = TVector2D(-8.0f, 0.0f);
     aJumpUp = TVector2D(0.0f, -17.0f);
    */

    MAX_RESPAWN_TIME = 5;
    CHOICE_GUN = false;
    SHOW_GUN_MENU = true;
    SHOW_SCORES = false;
    SHOW_COMMAND_LINE = false;
    SHOW_MYCHAT_LINE = false;
    SHOW_ESC = false;
    SHOW_STATS = true;
    CHOICE_EXIT = false;
    FRIENDLY_FIRE = false;
    YOU_KILLED = false;
    prev_time = 0;

    JET_CHANGE = 0.0005f;
    sGravity = 9.81f;
    sDrag = 0.3f;
    sDragWalking = 300.0f;
    sWalking = 10.0f;
    sFlying = 1000.0f;
    sJumping = 20.0f;
    fullLife = 495.0f;

    DISTANCE_SCORE = MY_CURRENT_POS = 0;
    KEY_PRESSED = SDLK_UNKNOWN;
    currentFPS = 0;
    getCurrentTime = 0;

    // --------------------------------------   loading interface  ------------------------------------------
    interfaceBaseMgr = new InterfaceBaseManager();

    if (Parser.INTERFACE_NAME != "--None--")
        interfaceMgr = new InterfaceManager();
    else
        std::cout << "Load limited interface" << std::endl;
// ------------------------------------------------------------------------------------------------------

    mouse = new Mouse();
    chat = new Chat();
    window_guns = new WindowGuns();
    arrow = new Arrow();

}


void WorldMap::load_bonuses()
{

    std::cout << "   loading bonuses information ..." << std::endl;

    bonus.resize(7);

    for (int i = 0; i < map->spawnpointCount; ++i)
    {
        if (map->spawnpoint[i].active)
        {
            if (map->spawnpoint[i].team == map->stBERSERKER && Parser.BONUSES[BONUS_BERSERKER])
                bonus[BONUS_BERSERKER].push_back(i);
            else if (map->spawnpoint[i].team == map->stCLUSTERS && Parser.BONUSES[BONUS_CLUSTERS])
                bonus[BONUS_CLUSTERS].push_back(i);
            else if (map->spawnpoint[i].team == map->stFLAMER && Parser.BONUSES[BONUS_FLAMER])
                bonus[BONUS_FLAMER].push_back(i);
            else if (map->spawnpoint[i].team == map->stGRENADES)
                bonus[BONUS_GRENADES].push_back(i);
            else if (map->spawnpoint[i].team == map->stMEDKITS)
                bonus[BONUS_MEDIKITS].push_back(i);
            else if (map->spawnpoint[i].team == map->stPREDATOR && Parser.BONUSES[BONUS_PREDATOR])
                bonus[BONUS_PREDATOR].push_back(i);
            else if (map->spawnpoint[i].team == map->stVEST && Parser.BONUSES[BONUS_VESTKIT])
                bonus[BONUS_VESTKIT].push_back(i);
        }
    }

    // laduj wszystko co jest
    // fix it !!!!!!!!!!!!
    TVector2D p;
    int num;
    for (unsigned int i = 0; i < 7; ++i)
    {
        if (Parser.BONUSES[i])
            for (unsigned int j = 0; j < bonus[i].size(); ++j)
            {
                num = bonus[i][j];
                p.x = static_cast<float>(map->spawnpoint[num].x);
                p.y = static_cast<float>(map->spawnpoint[num].y);
                Bonus *newbon = new Bonus(p, i);
                //Bonus newbon(p, text_bonus[i], i);
                bonus_list.push_back(newbon);
//            m_objects.push_back(newbon);
            }
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



int WorldMap::load_bots(unsigned int bot_count, TEAM team)
{

    if (bot_count == 0)
    {
        std::cerr << "   no bots assigned for " << team << " team" << std::endl;
        return 0;
    }

    /* losujemy boty, zeby byly niepowtarzalne w teamie (CTF, ...) lub w calej rozgrywce (DM, ...)
       (numery umieszczamy w unikalnej liscie - u nas vector).
       jesli liczba botow, ktore maja zostac dodane przekracza
       liczbe zaladowanych botow z dysku,
       to dodawaj nazwom botow liczbe od 10 do 99

       flist zawiera tyle elementow, ile wynosi liczba botow z kalatogu "Bots" */

    if (!spawnpoint[team].empty())
    {
        unsigned int b_cnt = 0, sol_nr, bot_nr;
        std::vector<unsigned int>::iterator it;
        std::vector<unsigned int> flist;
        std::ostringstream oss;

        while (b_cnt < bot_count)
        {

            for (unsigned int k = 0; k < Bots.element.size(); ++k)
            {
                flist.push_back(k);
            }

            while (b_cnt < bot_count && !flist.empty())
            {
                sol_nr = rand() % flist.size();
                bot_nr = addBot(Bots.element[flist[sol_nr]], spawnpoint[team][rand()%spawnpoint[team].size()], team);

                // check, if there is already bot with the same name
                for (unsigned int k = 0; k < bot.size(); ++k)
                {
                    if (bot[k]->name == bot[bot_nr]->name && bot[k]->team == bot[bot_nr]->team && k != bot_nr)
                    {
                        oss << rand() % 90 + 10;
                        bot[bot_nr]->name += " " + oss.str();
                        oss.str("");
                        break;
                    }
                }

                std::cout << "   assigning " << bot[bot_nr]->name << " to " << bot[bot_nr]->team << " team" << std::endl;

                it = flist.begin();
                it += sol_nr;
                flist.erase(it);

                b_cnt++;
            }
        }
        flist.clear();
    }
    else
    {
        std::cerr << "Cannot find spawnpoints for " << team << " team !" << std::endl;
        return -1;
    }

    return 0;
}


void WorldMap::load_textures()
{
    std::cout << "Loading textures ... " << std::endl;

    std::cout << "   loading scenery textures ... " << std::endl;
    text_scen = new GLuint[map->sceneryCount];
    //GLuint text_scen[map->sceneryCount];
    std::vector<std::string> names(map->getTextScen());

    for (unsigned int i = 0; i < names.size(); ++i)
    {
        text_scen[i] = SOIL_LoadTexture2(Parser.SOL_PATH + "Scenery-gfx/", names[i]);
    }
    map->setScen(text_scen);

    std::cout << "   loading triangles texture ... " << std::endl;
    GLuint text_poly = SOIL_LoadTexture2(Parser.SOL_PATH + "Textures/", map->getTextPoly());
    map->setPoly(text_poly);


    std::cout << "   loading grenade's textures ... " << std::endl;
    text_grenade[0] = SOIL_LoadTextureEx2(Parser.INTERFACE_PATH, "nade");
    std::ostringstream oss;
    std::string path;
    for (int i = 1; i < 16; ++i)
    {
        oss << i+1;
        text_grenade[i] = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Sparks-gfx/explosion/", "explode" + oss.str());
        oss.str("");
    }

    std::cout << "   loading weather textures ... " << std::endl;
    if (map->weather != map->wtNONE)
    {
        if (map->weather == map->wtRAIN)
            text_weath = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Sparks-gfx/", "rain");
        else if (map->weather == map->wtSANDSTORM)
            text_weath = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Sparks-gfx/", "sand");
        else if (map->weather == map->wtSNOW)
            text_weath = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Sparks-gfx/", "snow");
    }



}



int WorldMap::load_map(const std::string& name)
{
    std::cout << "Loading map file ... " << std::endl;

    if (boost::filesystem::exists(name))
    {
        map = new Map(name);
        return 0;
    }
    else
    {
        std::cerr << "Cannot find map file : " << name << std::endl;
        exit(1);
    }
}

