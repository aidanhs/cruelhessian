/*   BotManager.cpp
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


#include <cmath>
#include <sstream>
#include "boost/filesystem/fstream.hpp"

#include "Bot.h"
#include "Body.h"
#include "Map.h"
#include "BotManager.h"
#include "Background.h"
#include "WeaponManager.h"
#include "FontManager.h"
#include "WorldMap.h"
#include "Enums.h"
#include "ParserManager.h"
#include "Game.h"
#include "parser/SimpleIni.h"
#include "Regex.h"
#include "TexturesLoader.h"



const TVector2D multiplier = TVector2D(2.6f, 3.2f);
const float _180overpi = 57.29f;


const std::string BotManager::anim_type(MT name) const
{
    switch (name)
    {
    case BIEGA:
        return Parser.GAME_PATH + "Anims/biega.poa";
    case BIEGA_TYL:
        return Parser.GAME_PATH + "Anims/biegatyl.poa";
    case CELUJE:
        return Parser.GAME_PATH + "Anims/celuje.poa";
    case CHANGE:
        return Parser.GAME_PATH + "Anims/change.poa";
    case CIESZY:
        return Parser.GAME_PATH + "Anims/cieszy.poa";
    case GORA:
        return Parser.GAME_PATH + "Anims/gora.poa";
    case KUCA:
        return Parser.GAME_PATH + "Anims/kuca.poa";
    case RZUCA:
        return Parser.GAME_PATH + "Anims/rzuca.poa";
    case SKOK:
        return Parser.GAME_PATH + "Anims/skok.poa";
    case SKOK_DOL_OBROT:
        return Parser.GAME_PATH + "Anims/skokdolobrot.poa";
    case SKOK_DOL_OBROT_TYL:
        return Parser.GAME_PATH + "Anims/skokdolobrottyl.poa";
    case SKOK_W_BOK:
        return Parser.GAME_PATH + "Anims/skokwbok.poa";
    case SPADA:
        return Parser.GAME_PATH + "Anims/spada.poa";
    case STOI:
        return Parser.GAME_PATH + "Anims/stoi.poa";
    case WYRZUCA:
        return Parser.GAME_PATH + "Anims/wyrzuca.poa";
    case ZMIEN_BRON:
        return Parser.GAME_PATH + "Anims/zmienbron.poa";
        //case
    default:
        break;
    }

    return NULL;
}



int BotManager::read_poa(const MT name)
{
    std::string buffer;
    int i = 0, k = 0;
    bool stop = false;

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

            part_x[name][k][i][LEFT] *= multiplier.x;
            part_y[name][k][i][LEFT] *= -multiplier.y;

            part_x[name][k][i][RIGHT] = -part_x[name][k][i][LEFT];
            part_y[name][k][i][RIGHT] = part_y[name][k][i][LEFT];
            part_z[name][k][i][RIGHT] = part_z[name][k][i][LEFT];

            /*  if (part_x[name][k][i][RIGHT] < min_x) min_x = part_x[name][k][i][RIGHT];
              if (part_x[name][k][i][RIGHT] > max_x) max_x = part_x[name][k][i][RIGHT];
              if (part_z[name][k][i][RIGHT] < min_z) min_z = part_z[name][k][i][RIGHT];
              if (part_z[name][k][i][RIGHT] > max_z) max_z = part_z[name][k][i][RIGHT];
            */
            ++k;
        }
        else
        {
            if (buffer == "ENDFILE")
            {
                //world.BODYPARTS_MAX[name] = k;
                stop = true;
            }
            else if (buffer == "NEXTFRAME")
            {
                ++i;
                k = 0;
            }
        }
    }

    //world.SOL_WIDTH[name] = max_x - min_x;
    //world.SOL_HEIGHT[name] = max_z - min_z;

    FRAMES_MAX[name] = i+1;
    //std::cout << "FRAMES : " << FRAMES_MAX[name] << std::endl;

    file.close();

    return 0;

}

int BotManager::getWeaponNumber(const std::string& gun) const
{
    for (unsigned int i = 0; i < WeaponManager::WEAPONS_NUMBER; ++i)
    {
        if (Weapons[i].name == gun)
            return i;
    }
    std::cerr << "Unknown gun : " << gun << std::endl;
    return -1;
}


float BotManager::GetAngle(int a, int b, int c, int x, int y) const
{

    return _180overpi*atan2(part_y[a][x][b][c] - part_y[a][y][b][c],
                            part_x[a][x][b][c] - part_x[a][y][b][c]);

}

BotManager::BotManager(void)
{

    std::cout << "Starting BotManager ... " << std::endl;

    std::cout << "   loading bots' information ..." << std::endl;


    sGravity = 9.81f;
    sDrag = 0.3f;
    sDragWalking = 300.0f;
    sWalking = 10.0f;
    sFlying = 1000.0f;
    sJumping = 20.0f;
    fullLife = 495.0f;


    std::string fold = Parser.GAME_PATH + "Bots/";
    CSimpleIni ini(false, false, false);

    srand(static_cast<unsigned int>(time(0)));


    // reading Bots directory
    if (!boost::filesystem::exists(fold))
    {
        std::cerr << "'Bots' directory doesn't exist !" << std::endl;
//        return 1;
    }

    BotsBase temp;
    boost::filesystem::directory_iterator end;

    for (boost::filesystem::directory_iterator iter(fold); iter != end; ++iter)
    {
        if (regex_match(iter->leaf(), ".+.bot"))
        {

            if (ini.LoadFile((iter->path().string()).c_str()) < 0)
            {
                std::cerr << "Error in parsing " << iter->path().string() << std::endl;
//                return 1;
            }

            temp.name = ini.GetValue("BOT", "Name");

            std::cout << "   loading bot : " << temp.name << std::endl;

            temp.favouriteWeapon = getWeaponNumber(ini.GetValue("BOT", "Favourite_Weapon"));
            temp.color[SHIRT] = game.tcolor2rgb(ini.GetValue("BOT", "Color1"));
            temp.color[PANTS] = game.tcolor2rgb(ini.GetValue("BOT", "Color2"));
            temp.color[SKIN] = game.tcolor2rgb(ini.GetValue("BOT", "Skin_Color"));
            temp.color[HAIR] = game.tcolor2rgb(ini.GetValue("BOT", "Hair_Color"));
            temp.chatKill = ini.GetValue("BOT", "Chat_Kill");
            temp.chatDead = ini.GetValue("BOT", "Chat_Dead");
            temp.chatLowhealth = ini.GetValue("BOT", "Chat_Lowhealth");
            temp.chatSeeEnemy = ini.GetValue("BOT", "Chat_SeeEnemy");
            temp.chatWinning = ini.GetValue("BOT", "Chat_Winning");

            element.push_back(temp);
        }
    }
    if (element.empty())
    {
        std::cerr << "'Bots' directory hasn't got *.bot files !" << std::endl;
        //   return 1;
    }

    std::cout << "   loading bots' animations  ... " << std::endl;

    read_poa(BIEGA);
    read_poa(BIEGA_TYL);
    read_poa(CELUJE);
    read_poa(CHANGE);
    read_poa(CIESZY);
    read_poa(GORA);
    read_poa(KUCA);
    read_poa(RZUCA);
    read_poa(SKOK);
    read_poa(SKOK_DOL_OBROT);
    read_poa(SKOK_DOL_OBROT_TYL);
    read_poa(SKOK_W_BOK);
    read_poa(SPADA);
    read_poa(STOI);
    read_poa(ZMIEN_BRON);

    for (unsigned int i = 0; i < 14; i++)
        for (unsigned int j = 0; j < FRAMES_MAX[i]; j++)
            for (unsigned int k = 0; k < 2; k++)
            {
                //right foot (stopa)
                frame[i][0][j][k].x = part_x[i][1][j][k];
                frame[i][0][j][k].y = part_y[i][1][j][k];
                frame[i][0][j][k].r = GetAngle(i, j, k, 17, 1);

                //upper right leg (udo)
                frame[i][1][j][k].x = part_x[i][5][j][k];
                frame[i][1][j][k].y = part_y[i][5][j][k];
                frame[i][1][j][k].r = GetAngle(i, j, k, 2, 5);

                //lower right leg (noga)
                frame[i][2][j][k].x = part_x[i][2][j][k];
                frame[i][2][j][k].y = part_y[i][2][j][k];
                frame[i][2][j][k].r = GetAngle(i, j, k, 1, 2);

                //left foot (stopa)
                frame[i][3][j][k].x = part_x[i][0][j][k];
                frame[i][3][j][k].y = part_y[i][0][j][k];
                frame[i][3][j][k].r = GetAngle(i, j, k, 16, 0);

                //upper left leg (udo)
                frame[i][4][j][k].x = part_x[i][4][j][k];
                frame[i][4][j][k].y = part_y[i][4][j][k];
                frame[i][4][j][k].r = GetAngle(i, j, k, 3, 4);

                //lower left leg (noga)
                frame[i][5][j][k].x = part_x[i][3][j][k];
                frame[i][5][j][k].y = part_y[i][3][j][k];
                frame[i][5][j][k].r = GetAngle(i, j, k, 0, 3);

                //upper right arm (ramie)
                frame[i][6][j][k].x = part_x[i][10][j][k];
                frame[i][6][j][k].y = part_y[i][10][j][k];
                frame[i][6][j][k].r = GetAngle(i, j, k, 10, 13);

                //lower right arm (reka)
                frame[i][7][j][k].x = part_x[i][13][j][k];
                frame[i][7][j][k].y = part_y[i][13][j][k];
                frame[i][7][j][k].r = GetAngle(i, j, k, 14, 13);

                //right hand (dlon)
                frame[i][8][j][k].x = part_x[i][14][j][k];
                frame[i][8][j][k].y = part_y[i][14][j][k];
                frame[i][8][j][k].r = GetAngle(i, j, k, 18, 14);

                //torso (biodro)
                frame[i][9][j][k].x = part_x[i][7][j][k];
                frame[i][9][j][k].y = part_y[i][4][j][k];
                frame[i][9][j][k].r = GetAngle(i, j, k, 6, 7);

                //chest (klata)
                frame[i][10][j][k].x = part_x[i][7][j][k];
                frame[i][10][j][k].y = part_y[i][9][j][k];
                frame[i][10][j][k].r = GetAngle(i, j, k, 10, 9);

                //head (morda)
                frame[i][11][j][k].x = part_x[i][11][j][k];
                frame[i][11][j][k].y = part_y[i][11][j][k];
                frame[i][11][j][k].r = GetAngle(i, j, k, 11, 8);

                //upper left arm (ramie)
                frame[i][12][j][k].x = part_x[i][9][j][k];
                frame[i][12][j][k].y = part_y[i][9][j][k];
                frame[i][12][j][k].r = GetAngle(i, j, k, 9, 12);

                //lower left arm (reka)
                frame[i][13][j][k].x = part_x[i][12][j][k];
                frame[i][13][j][k].y = part_y[i][12][j][k];
                frame[i][13][j][k].r = GetAngle(i, j, k, 15, 12);

                //left hand (dlon)
                frame[i][14][j][k].x = part_x[i][15][j][k];
                frame[i][14][j][k].y = part_y[i][15][j][k];
                frame[i][14][j][k].r = GetAngle(i, j, k, 19, 15);

            }


    std::cout << "   loading bots' textures ... " << std::endl;

    gostek[STOPA][0]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "stopa");
    gostek[STOPA][1]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "stopa2");
    gostek[KLATA][0]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "klata");
    gostek[KLATA][1]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "klata2");
    gostek[RAMIE][0]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "ramie");
    gostek[RAMIE][1]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "ramie2");
    gostek[MORDA][0]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "morda");
    gostek[MORDA][1]  = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "morda2");
    gostek[REKA][0]   = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "reka");
    gostek[REKA][1]   = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "reka2");
    gostek[DLON][0]   = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "dlon");
    gostek[DLON][1]   = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "dlon2");
    gostek[BIODRO][0] = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "biodro");
    gostek[BIODRO][1] = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "biodro2");
    gostek[UDO][0]    = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "udo");
    gostek[UDO][1]    = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "udo2");
    gostek[NOGA][0]   = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "noga");
    gostek[NOGA][1]   = Texture::LoadExt(Parser.GAME_PATH + "Gostek-gfx/", "noga2");



    indices[0] = indices[3] = STOPA;
    indices[1] = indices[4] = UDO;
    indices[2] = indices[5] = NOGA;
    indices[6] = indices[12] = RAMIE;
    indices[7] = indices[13] = REKA;
    indices[8] = indices[14] = DLON;
    indices[9] = BIODRO;
    indices[10] = KLATA;
    indices[11] = MORDA;


    renderInfo[9].x = 1.0f; //biodro
    renderInfo[9].y = 2.5f;
    renderInfo[10].x = 1.5f; //klata
    renderInfo[10].y = 4.0f;
    renderInfo[11].x = 1.5f; //morda
    renderInfo[11].y = 4.0f;

    renderInfo[0].x = 2.0f; //prawa stopa
    renderInfo[0].y = 2.5f;
    renderInfo[3].x = 2.0f; //lewa stopa
    renderInfo[3].y = 2.5f;

    renderInfo[1].x = 1.5f; //prawe udo
    renderInfo[1].y = 3.2f;
    renderInfo[4].x = 1.5f; // lewe udo
    renderInfo[4].y = 3.2f;

    renderInfo[2].x = 1.2f; //prawa noga
    renderInfo[2].y = 3.2f;
    renderInfo[5].x = 1.2f; //lewa noga
    renderInfo[5].y = 3.2f;

    renderInfo[7].x = 1.5f; //prawa reka
    renderInfo[7].y = 1.5f;
    renderInfo[13].x = 1.5f; //lewa reka
    renderInfo[13].y = 1.5f;

    renderInfo[8].x = 2.0f; //prawa dlon
    renderInfo[8].y = 1.5f;
    renderInfo[14].x = 2.0f; // lewa dlon
    renderInfo[14].y = 1.5f;

    renderInfo[12].x = 2.0f; //lewe ramie
    renderInfo[12].y = 2.0f;
    renderInfo[6].x = 2.0f; // prawe ramie
    renderInfo[6].y = 2.0f;


//    mass = 100;
//    massInv = 1 / mass;
    maxSpeed = TVector2D(140, 150);
}


BotManager::~BotManager(void)
{
    std::cout << "Removing BotManager ... DONE" << std::endl;
    element.clear();
}


int BotManager::LoadBots(unsigned int bot_count, TEAM team)
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

    if (!world.spawnpoint[team].empty())
    {
        unsigned int b_cnt = 0, sol_nr, bot_nr;
        std::vector<unsigned int>::iterator it;
        std::vector<unsigned int> flist;
        std::ostringstream oss;

        while (b_cnt < bot_count)
        {

            for (unsigned int k = 0; k < element.size(); ++k)
            {
                flist.push_back(k);
            }

            while (b_cnt < bot_count && !flist.empty())
            {
                sol_nr = rand() % flist.size();
                bot_nr = world.addBot(element[flist[sol_nr]], world.spawnpoint[team][rand() % world.spawnpoint[team].size()], team);

                // check, if there is already bot with the same name
                for (unsigned int k = 0; k < world.bot.size(); ++k)
                {
                    if (world.bot[k]->name == world.bot[bot_nr]->name && world.bot[k]->team == world.bot[bot_nr]->team && k != bot_nr)
                    {
                        oss << rand() % 90 + 10;
                        world.bot[bot_nr]->name += " " + oss.str();
                        oss.str("");
                        break;
                    }
                }

                std::cout << "   assigning " << world.bot[bot_nr]->name << " to " << world.bot[bot_nr]->team << " team" << std::endl;

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


int BotManager::LoadBots(TEAM team)
{

    BotsBase temp;

    temp.name = Parser.PLAYER_NAME;
    temp.color[SKIN]  = Parser.COLOR_SKIN;
    temp.color[HAIR]  = Parser.COLOR_HAIR;
    temp.color[PANTS] = Parser.COLOR_PANTS;

    if (game.CURRENT_GAME_MODE == CTF || game.CURRENT_GAME_MODE == TM)
    {
        temp.color[SHIRT] = Fonts.textCol[team];
    }
    else
        temp.color[SHIRT] = Parser.COLOR_SHIRT;


    //szukaj druzyny, ktora ma spawnpointy
    int team_nr = team;
    while (world.spawnpoint[team_nr].empty() && team_nr < 5) team_nr++;

    int point = static_cast<int>(rand() % world.spawnpoint[team_nr].size());
    world.MY_BOT_NR = world.addBot(temp, world.spawnpoint[team_nr][point], (TEAM)team_nr);

    TVector2D wsk = TVector2D(static_cast<float>(Parser.MAX_WIDTH/2 - world.map->spawnpoint[world.spawnpoint[team_nr][point]].x),
                                 static_cast<float>(Parser.MAX_HEIGHT/2 - world.map->spawnpoint[world.spawnpoint[team_nr][point]].y));

    world.backg = new Background(wsk, world.bot[world.MY_BOT_NR]->GetPosition());

    return 0;
}
