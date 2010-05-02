/*   bot.cpp
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


#include <cmath>
#include "bot.h"
#include "globals.h"


Bot::Bot(const std::string& _name, float spawn_x, float spawn_y, int gunmodel, TEAM _team, unsigned int bot_nr, int dest, Tex text[9][2]) : name(_name), team(_team)
{

    type = CIRCLE;
    number = bot_nr;
    movementDirection = LEFT;
    movementType = STOI;
    position = TVector2D(spawn_x, spawn_y);

    // treat as a circle (ellipse - that will be the best)
    //w = SOL_WIDTH[STOI];
    //h = SOL_HEIGHT[STOI];
    w = h = 0;
    r = SOL_HEIGHT[movementType] / 2;
    //std::cout << "WER " << r << std::endl;
    //r = 10;

    velocity = TVector2D(0.0f, 0.0f);
    mass = 100;
    massInv = 1 / mass;
    maxSpeed = TVector2D(140, 150);
    isAbleToFly = isReloading = isKilled = isAbleToJump = false;
    procJet = 1.0;
    actLife = fullLife;

    gunModel = gunmodel;
    respawnTime = 0;
    lastShotTime = 0;
    currentFrame = 0;
    timerChangeFrame = 0;
    timerRespawnTime = 0;
    youKilledTime = 0;
    //if (bot_nr != MY_BOT_NR)
    destinationPoint = dest;
    //else destinationPoint = 0;
    leftAmmos = 1;
    numGrenades = 5;
    points = 0;
    killedNr = 0;
    deaths = 0;
    ping = 0;

    for (int j = 0; j < 2; ++j)
        for (int i = 0; i < 9; ++i)
        {
            texture[i][j] = &text[i][j];
        }

    old_a = TVector2D(0, sGravity);
}



bool Bot::is_inside(int x, int y)
{
    return ((x > position.x - r) && (x < position.x + r) &&
            (y > position.y - r) && (y < position.y + r));
}



void Bot::draw_gostek_help(int partx, int party, float angle, const Tex* xtex)
{

    if (movementDirection == LEFT && (partx == 7 && party == 9))
    {
        partx = 9;
        party = 6;
    }
    if (movementDirection == RIGHT && (partx == 7 && party == 4)) // biodro
    {
        partx = 7;
        party = 6;
    }/*
    if (movementDirection == LEFT && (partx == 8 && party == 8)) // morda
    {
        partx = 10;
        party = 8;
    }*/
    //Mix xtex;

    float sx  = part_x[movementType][partx][currentFrame][movementDirection];
    float sy  = part_y[movementType][party][currentFrame][movementDirection];
    if (movementDirection == RIGHT && ((partx == 0 && party == 0) || (partx == 1 && party == 1)))
    {
        //sy -= 5.2*xtex.h;
        //sy -= xtex.h;
    }

    // xtex = which_part(parts, movementDirection);

    glPushMatrix();

    glTranslatef(position.x - r + sx, position.y - r + sy, 0.0f);

    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, xtex->tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(xtex->w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(xtex->w, xtex->h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, xtex->h);
    glEnd();

    glPopMatrix();

}

float Bot::getAngle(int x, int y)
{
    return _180overpi*atan2(part_y[movementType][y][currentFrame][movementDirection]-
                            part_y[movementType][x][currentFrame][movementDirection],
                            part_x[movementType][y][currentFrame][movementDirection]-
                            part_x[movementType][x][currentFrame][movementDirection]);


    /* return _180overpi*GetAngle(part_x[bot[bot_nr]->movementType][y][bot[bot_nr]->currentFrame][bot[bot_nr]->movementDirection]-
                     part_x[bot[bot_nr]->movementType][x][bot[bot_nr]->currentFrame][bot[bot_nr]->movementDirection],
                     part_y[bot[bot_nr]->movementType][y][bot[bot_nr]->currentFrame][bot[bot_nr]->movementDirection]-
                     part_y[bot[bot_nr]->movementType][x][bot[bot_nr]->currentFrame][bot[bot_nr]->movementDirection])-90;*/
}


void Bot::draw()
{

    int dir = 1 - movementDirection;
    if (!isKilled)
    {

        // biodro
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        draw_gostek_help(7, 4, getAngle(7,6), texture[BIODRO][dir]);

        // lewa noga
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        draw_gostek_help(3, 3, getAngle(3,0), texture[NOGA][dir]);

        // prawa noga
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        draw_gostek_help(2, 2, getAngle(2,1), texture[NOGA][dir]);

        // lewe udo
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        draw_gostek_help(5, 5, getAngle(5,2), texture[UDO][dir]);

        // prawe udo
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        draw_gostek_help(4, 4, getAngle(4,3), texture[UDO][dir]);
        glColor3f(1,1,1);

        // lewa stopa
        draw_gostek_help(0, 0, getAngle(0,16), texture[STOPA][dir]);

        // prawa stopa
        draw_gostek_help(1, 1, getAngle(1,17), texture[STOPA][dir]);

        // prawe ramie
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        draw_gostek_help(10, 10, getAngle(13,10) - 270, texture[RAMIE][dir]);

        // prawa reka
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        draw_gostek_help(13, 13, getAngle(13,14), texture[REKA][dir]);

        // prawa dlon
        glColor3ub(color[SKIN][0], color[SKIN][1], color[SKIN][2]);
        draw_gostek_help(14, 14, getAngle(14,18), texture[DLON][dir]);

        // klata
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        draw_gostek_help(7, 9, getAngle(9,10), texture[KLATA][dir]);

        // morda
        glColor3ub(color[SKIN][0], color[SKIN][1], color[SKIN][2]);
        draw_gostek_help(8, 8, getAngle(8,11), texture[MORDA][dir]);

        // lewe ramie
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        draw_gostek_help(9, 9, getAngle(12,9) - 270, texture[RAMIE][dir]);

        // lewa reka
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        draw_gostek_help(12, 12, getAngle(12,15), texture[REKA][dir]);

        // lewa dlon
        glColor3ub(color[SKIN][0], color[SKIN][1], color[SKIN][2]);
        draw_gostek_help(15, 15, getAngle(15,19), texture[DLON][dir]);
        glColor3f(1,1,1);

    }

}


void Bot::update()
{

    if (!isKilled && (getCurrentTime - timerChangeFrame >= 30))
    {
        timerChangeFrame = getCurrentTime;
        if (currentFrame >= FRAMES_MAX[movementType]-1)
        {
            if (movementType != KUCA)
                currentFrame = 0;
        }
        else
            currentFrame++;
    }

    // speed limit
    if (velocity.x > maxSpeed.x) velocity.x = maxSpeed.x;
    else if (velocity.x < -maxSpeed.x) velocity.x = -maxSpeed.x;
    if (velocity.y > maxSpeed.y) velocity.y = maxSpeed.y;
    else if (velocity.y < -maxSpeed.y) velocity.y = -maxSpeed.y;

}
