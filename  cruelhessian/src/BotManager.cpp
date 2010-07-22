/*   BotManager.cpp
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


#include <cmath>
#include "boost/filesystem/fstream.hpp"

#include "BotManager.h"
#include "Enums.h"
#include "ParserManager.h"
#include "Game.h"
#include "parser/SimpleIni.h"
#include "regexp.h"
#include "TexturesLoader.h"



const TVector2D multiplier = TVector2D(2.6f, 3.2f);
const float _180overpi = 57.29f;


std::string BotManager::anim_type(MT name)
{
    switch (name)
    {
    case BIEGA:
        return Parser.SOL_PATH + "Anims/biega.poa";
    case BIEGA_TYL:
        return Parser.SOL_PATH + "Anims/biegatyl.poa";
    case CELUJE:
        return Parser.SOL_PATH + "Anims/celuje.poa";
    case CIESZY:
        return Parser.SOL_PATH + "Anims/cieszy.poa";
    case GORA:
        return Parser.SOL_PATH + "Anims/gora.poa";
    case KUCA:
        return Parser.SOL_PATH + "Anims/kuca.poa";
    case RZUCA:
        return Parser.SOL_PATH + "Anims/rzuca.poa";
    case SKOK:
        return Parser.SOL_PATH + "Anims/skok.poa";
    case SKOK_DOL_OBROT:
        return Parser.SOL_PATH + "Anims/skokdolobrot.poa";
    case SKOK_DOL_OBROT_TYL:
        return Parser.SOL_PATH + "Anims/skokdolobrottyl.poa";
    case SKOK_W_BOK:
        return Parser.SOL_PATH + "Anims/skokwbok.poa";
    case SPADA:
        return Parser.SOL_PATH + "Anims/spada.poa";
    case STOI:
        return Parser.SOL_PATH + "Anims/stoi.poa";
    case ZMIEN_BRON:
        return Parser.SOL_PATH + "Anims/zmienbron.poa";
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

    file.close();

    return 0;

}

int BotManager::getWeaponNumber(const std::string& gun)
{
    for (unsigned int i = 0; i < WEAPONS_NUMBER; ++i)
    {
        if (Weapons[i].name == gun)
            return i;
    }
    std::cerr << "Unknown gun : " << gun << std::endl;
    return -1;
}


float BotManager::getAngle(float x, float y)
{

    return _180overpi*atan2(x, y);

}

float BotManager::GetAngle(int a, int b, int c, int x, int y)
{

    return getAngle(part_x[a][x][b][c]-
                    part_x[a][y][b][c],
                    part_y[a][x][b][c]-
                    part_y[a][y][b][c]);

}

BotManager::BotManager()
{

    std::cout << "Starting BotManager ... " << std::endl;

    std::cout << "   loading bots' information ..." << std::endl;


    std::string fold = Parser.SOL_PATH + "Bots/";
    CSimpleIni ini(false, false, false);

    srand(static_cast<unsigned int>(time(0)));

    //boost::regex re(fold + ".+.bot");
    //boost::regex re(".+.bot");
    std::string re = ".+.bot";
    boost::filesystem::directory_iterator end;

    // reading Bots directory
    if (!boost::filesystem::exists(fold))
    {
        std::cerr << "'Bots' directory doesn't exist !" << std::endl;
//        return 1;
    }

    //int nr = 0;
    BotsBase temp;

    for (boost::filesystem::directory_iterator iter(fold); iter != end; ++iter)
    {
        //if (boost::regex_match(iter->leaf(), re))
        if (regexec(regcomp((char *)re.c_str()), (char *)iter->leaf().c_str()))
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

    gostek[STOPA][0]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "stopa");
    gostek[STOPA][1]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "stopa2");
    gostek[KLATA][0]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "klata");
    gostek[KLATA][1]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "klata2");
    gostek[RAMIE][0]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "ramie");
    gostek[RAMIE][1]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "ramie2");
    gostek[MORDA][0]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "morda");
    gostek[MORDA][1]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "morda2");
    gostek[REKA][0]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "reka");
    gostek[REKA][1]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "reka2");
    gostek[DLON][0]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "dlon");
    gostek[DLON][1]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "dlon2");
    gostek[BIODRO][0] = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "biodro");
    gostek[BIODRO][1] = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "biodro2");
    gostek[UDO][0]    = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "udo");
    gostek[UDO][1]    = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "udo2");
    gostek[NOGA][0]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "noga");
    gostek[NOGA][1]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Gostek-gfx/", "noga2");



    indices[0] = indices[3] = STOPA;
    indices[1] = indices[4] = UDO;
    indices[2] = indices[5] = NOGA;
    indices[6] = indices[12] = RAMIE;
    indices[7] = indices[13] = REKA;
    indices[8] = indices[14] = DLON;
    indices[9] = BIODRO;
    indices[10] = KLATA;
    indices[11] = MORDA;


    renderInfo[9].x = 1; //biodro
    renderInfo[9].y = 2.5f;
    renderInfo[10].x = 1.5f; //klata
    renderInfo[10].y = 4;
    renderInfo[11].x = 1.5f; //morda
    renderInfo[11].y = 4;

    renderInfo[0].x = 2; //prawa stopa
    renderInfo[0].y = 2.5f;
    renderInfo[3].x = 2; //lewa stopa
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

    renderInfo[8].x = 2; //prawa dlon
    renderInfo[8].y = 1.5f;
    renderInfo[14].x = 2; // lewa dlon
    renderInfo[14].y = 1.5f;

    renderInfo[12].x = 2; //lewe ramie
    renderInfo[12].y = 2;
    renderInfo[6].x = 2; // prawe ramie
    renderInfo[6].y = 2;


    mass = 100;
    massInv = 1 / mass;
    maxSpeed = TVector2D(140, 150);
}


BotManager::~BotManager()
{
    std::cout << "Removing BotManager ..." << std::endl;
    element.clear();
}
