/*   bot.cpp
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

#include <fstream>
#include <sstream>
#include <string>
#include "worldmap.h"
#include "globals.h"
#include "bullet.h"
#include "bot.h"

#define LINE(move, start, end, i, kier) \
        glVertex2f(part_x[move][start][i][kier], part_z[move][start][i][kier]);glVertex2f(part_x[move][end][i][kier], part_z[move][end][i][kier]);


Bot **bot;
int FRAMES_MAX[50], BODYPARTS_MAX[50];
float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie




void make_lines(MT move, int frame, int kierunek)
{

    glBegin(GL_LINES);

    LINE(move, 0, 3, frame, kierunek);
    LINE(move, 0, 16, frame, kierunek);
    LINE(move, 1, 2, frame, kierunek);
    LINE(move, 1, 17, frame, kierunek);
    LINE(move, 2, 5, frame, kierunek);
    LINE(move, 3, 4, frame, kierunek);
    LINE(move, 4, 5, frame, kierunek);
    LINE(move, 4, 6, frame, kierunek);
    LINE(move, 4, 7, frame, kierunek);
    LINE(move, 5, 6, frame, kierunek);
    LINE(move, 5, 7, frame, kierunek);
    LINE(move, 6, 7, frame, kierunek);
    LINE(move, 6, 8, frame, kierunek);
    LINE(move, 6, 9, frame, kierunek);
    LINE(move, 6, 10, frame, kierunek);
    LINE(move, 6, 10, frame, kierunek);
    LINE(move, 7, 8, frame, kierunek);
    LINE(move, 7, 9, frame, kierunek);
    LINE(move, 7, 10, frame, kierunek);
    LINE(move, 8, 9, frame, kierunek);
    LINE(move, 8, 10, frame, kierunek);
    LINE(move, 8, 11, frame, kierunek);
    LINE(move, 9, 10, frame, kierunek);
    LINE(move, 9, 12, frame, kierunek);
    LINE(move, 10, 13, frame, kierunek);
    LINE(move, 12, 15, frame, kierunek);
    LINE(move, 13, 14, frame, kierunek);

    LINE(move, 14, 18, frame, kierunek);
    LINE(move, 15, 19, frame, kierunek);

    glEnd();

}


//static const char *anim_type(MT name)
std::string anim_type(MT name)
{
    switch (name)
    {
    case BIEGA:
        //return (SOL_PATH+"Anims/biega.poa").c_str();
        return SOL_PATH+"Anims/biega.poa";
    case BIEGA_TYL:
        //return (SOL_PATH+"Anims/biega_tyl.poa").c_str();
        return SOL_PATH+"Anims/biega_tyl.poa";
    case CELUJE:
        //return (SOL_PATH+"Anims/celuje.poa").c_str();
        return SOL_PATH+"Anims/celuje.poa";
    case CIESZY:
        //return (SOL_PATH+"Anims/cieszy.poa").c_str();
        return SOL_PATH+"Anims/cieszy.poa";
    case GORA:
        //return (SOL_PATH+"Anims/gora.poa").c_str();
        return SOL_PATH+"Anims/gora.poa";
    case KUCA:
        //return (SOL_PATH+"Anims/kuca.poa").c_str();
        return SOL_PATH+"Anims/kuca.poa";
    case RZUCA:
        //return (SOL_PATH+"Anims/rzuca.poa").c_str();
        return SOL_PATH+"Anims/rzuca.poa";
    case SKOK:
        //return (SOL_PATH+"Anims/skok.poa").c_str();
        return SOL_PATH+"Anims/skok.poa";
    case SPADA:
        //return (SOL_PATH+"Anims/spada.poa").c_str();
        return SOL_PATH+"Anims/spada.poa";
    case STOI:
        //return (SOL_PATH+"Anims/stoi.poa").c_str();
        return SOL_PATH+"Anims/stoi.poa";
    case ZMIEN_BRON:
        //return (SOL_PATH+"Anims/zmien_bron.poa").c_str();
        return SOL_PATH+"Anims/zmien_bron.poa";
        //case
    }
    //return SOL_PATH+"Anims/"+
    return NULL;
}



static int read_poa_temp(MT name)
{
    for (int temp = 0; temp < 2; temp++)
    {
        int i = 0, k = 0;
        FILE *file;
        int stop = 0;
        char buffer[255];
        //std::string buffer;
        float min_x = 100, max_x = -100, min_z = 100, max_z = -100;

        // reading .poa file
        if ((file = fopen(anim_type(name).c_str(), "r")) == NULL)
        /*std::ifstream file (anim_type(name).c_str());
        if (!file.is_open())*/
        {
            cout << "ERROR opening file \n\n";
            return -1;
        }
        //cout << "start";
        //istringstream in;
        //string input;
        while (stop == 0)
        {
            //getline (file,buffer);
            fgets(buffer, 255, file);
            if (buffer[0] != '\r' && buffer[0] != '\n' && buffer[0] != 'N' && buffer[0] != 'E' && buffer[0] != 'C' && stop == 0)
            {
                //sscanf(buffer, "%s", part);

                fgets(buffer, 255, file);
                sscanf(buffer, "%f\n", &part_x[name][k][i][temp]);
                //getline (file, input);
                //in.str(input);
                //in >> part_x[name][k][i][temp];

                fgets(buffer, 255, file);
                sscanf(buffer, "%f\n", &part_y[name][k][i][temp]);
                //getline (file, input);
                //in.str(input);
                //in >> part_y[name][k][i][temp];

                fgets(buffer, 255, file);
                sscanf(buffer, "%f\n", &part_z[name][k][i][temp]);
                //getline (file, input);
                //in.str(input);
                //in >> part_z[name][k][i][temp];

                if (temp == 1)
                {
                    part_x[name][k][i][temp] *= -4;
                }
                else if (temp == 0)
                {
                    part_x[name][k][i][temp] *= 4;
                }
                else
                    exit(EXIT_FAILURE);

                part_z[name][k][i][temp] *= -4;

                if (part_x[name][k][i][temp] < min_x) min_x = part_x[name][k][i][temp];
                if (part_x[name][k][i][temp] > max_x) max_x = part_x[name][k][i][temp];
                if (part_z[name][k][i][temp] < min_z) min_z = part_z[name][k][i][temp];
                if (part_z[name][k][i][temp] > max_z) max_z = part_z[name][k][i][temp];

                k++;
            }
            else
            {
                if (buffer[0] == 'E') stop = 1;
                else if (buffer[0] == 'N')
                {
                    i++;       // next frame
                    BODYPARTS_MAX[name] = k;
                    k = 0;
                }
            }
        }
        FRAMES_MAX[name] = i+1;
        fclose(file);
//        file.close();

        SOL_WIDTH[name] = max_x - min_x;
        SOL_HEIGHT[name] = max_z - min_z;

        // normalization

        for (i = 0; i < FRAMES_MAX[name]; i++)
        {
            for (k = 0; k < BODYPARTS_MAX[name]; k++)
            {
                part_x[name][k][i][temp] -= min_x;
                part_z[name][k][i][temp] -= min_z;
            }
        }
    }
    return 0;
}

void WorldMap::read_animations()
{
    read_poa_temp(STOI);
    read_poa_temp(RZUCA);
    read_poa_temp(BIEGA);
    read_poa_temp(CIESZY);
    read_poa_temp(KUCA);
    read_poa_temp(SKOK);
    read_poa_temp(GORA);
//    read_poa_temp(ZMIEN_BRON);
}


Bot::Bot(int bot_nr, string name, float posx, float posy, int gunmodel, int spawn_nr)
{
    this->botNumber = bot_nr;
    this->name = name;
    this->position.x = posx;
    this->position.y = posy;
    this->w = SOL_WIDTH[STOI];
    this->h = SOL_HEIGHT[STOI];
    this->shotPointX = this->position.x+this->w/2;
    this->shotPointY = this->position.y+this->h/2;
    this->velocity.x = 0.0;
    this->velocity.y = 0.0;
    this->mass = 100;
    this->massInv = 1 / this->mass;
    this->maxV = 40;
    this->isFlying = false;
    this->isReloading = false;
    this->isShooting = false;
    this->isKilled = false;
    this->procJet = 1.0;
    this->procLife = 1.0;
    this->respawnTime = 0;
    this->movementDirection = LEFT;
    this->movementType = STOI;
    this->gunModel = gunmodel;
    this->lastShotTime = 0;
    this->currentFrame = 0;
    this->timerChangeFrame = 0;
    this->timerRespawnTime = 0;
    this->destinationPoint = find_nearest_waypoint_nr(spawn_nr);
    this->leftAmmos = gun_ammos(this->gunModel);
    this->numGrenades = 5;
}

int Bot::find_nearest_waypoint_nr(int spawn_nr)
{
    double min_val = 0.0;
    int min_nr = 0;
    int i, j;

    for (i = 1; i < p.waypointCount; i++)
    {
        if (p.waypoint[i].path == 1 && (p.waypoint[i].right || p.waypoint[i].left || p.waypoint[i].up || p.waypoint[i].down || p.waypoint[i].jet))
        {
            min_val = pow(p.spawnpoint[spawn_nr].x-p.waypoint[i].x, 2) + pow(p.spawnpoint[spawn_nr].y-p.waypoint[i].y, 2);
            min_nr = i;
            break;
        }
    }

    for (j = i+1; j < p.waypointCount; j++)
    {
        if ((pow(p.spawnpoint[spawn_nr].x-p.waypoint[j].x, 2) + pow(p.spawnpoint[spawn_nr].y-p.waypoint[j].y, 2) < min_val) &&
                (p.waypoint[i].path == 1 && (p.waypoint[j].right || p.waypoint[j].left || p.waypoint[j].up || p.waypoint[j].down || p.waypoint[j].jet)))
        {
            min_val = pow(p.spawnpoint[spawn_nr].x-p.waypoint[j].x, 2) + pow(p.spawnpoint[spawn_nr].y-p.waypoint[j].y, 2);
            min_nr = j;
        }
    }
    return min_nr;
}

bool Bot::is_inside(int x, int y)
{
    if ((x > this->position.x) &&
        (x < this->position.x + this->w) &&
        (y > this->position.y) &&
        (y < this->position.y + this->h))
        return true;
    else return false;
}


void Bot::move_right()
{
    //cout << "Prawo\n";
    this->movementType = BIEGA;
    this->velocity.x -= world->sWalking;
}

void Bot::move_left()
{
    //cout << "Lewo\n";
    this->movementType = BIEGA;
    this->velocity.x += world->sWalking;
}

void Bot::move_up()
{
    //cout << "Gora\n";
    this->velocity.y += 0.2*world->sWalking;
}

void Bot::move_down()
{
    //cout << "Dol\n";
    this->velocity.y -= world->sWalking;
}

void Bot::move_jet()
{
    //cout << "Jet\n";
    this->velocity.y += 3*world->sWalking;
}

