/*   Bot.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawel Konieczny <konp84 at gmail.com>
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


#include "Bot.h"
#include "WorldMap.h"


Bot::Bot(const std::string& _name, float spawn_x, float spawn_y, int gunmodel, TEAM _team, unsigned int bot_nr, int dest) : name(_name), team(_team)
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
    friction = 0;
    //r = world.SOL_HEIGHT[movementType] / 2;
    //std::cout << "WER " << r << std::endl;
    r = 4;

    velocity = TVector2D(0.0f, 0.0f);
    mass = Bots.mass;
    massInv = Bots.massInv;
    maxSpeed = Bots.maxSpeed;
    isAbleToFly = isReloading = isKilled = isAbleToJump = false;
    procJet = 1.0;
    actLife = world.fullLife;

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

    old_a = TVector2D(0, world.sGravity);

}



bool Bot::is_inside(int x, int y)
{
    return ((x > position.x - r) && (x < position.x + r) &&
            (y > position.y - r) && (y < position.y + r));
}



void Bot::draw_gostek_help(int part)
{

    BODY name = Bots.indices[part];
    int dir = movementDirection;

    glPushMatrix();

    glTranslatef(position.x + Bots.frame[movementType][part][currentFrame][dir].x + Bots.renderInfo[part].x,
                 position.y + Bots.frame[movementType][part][currentFrame][dir].y + Bots.renderInfo[part].y, 0.0f);

    glRotatef(Bots.frame[movementType][part][currentFrame][dir].r, 0.0f, 0.0f, 1.0f);

    glTranslatef(-Bots.renderInfo[part].x,
                 -Bots.renderInfo[part].y, 0.0f);

      //           glScalef(2,2,1);

    glBindTexture(GL_TEXTURE_2D, Bots.gostek[name][dir].tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(Bots.gostek[name][dir].w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(Bots.gostek[name][dir].w, Bots.gostek[name][dir].h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, Bots.gostek[name][dir].h);
    glEnd();

    glPopMatrix();

}

void Bot::draw()
{

    if (!isKilled)
    {

        // torso (biodro)
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        //draw_gostek_help(9, &Bots.gostek[BIODRO][dir]);
        draw_gostek_help(9);

        // lewa noga
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        //draw_gostek_help(5, &Bots.gostek[NOGA][dir]);
        draw_gostek_help(5);

        // prawa noga
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        //draw_gostek_help(2, &Bots.gostek[NOGA][dir]);
        draw_gostek_help(2);

        // lewe udo
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        //draw_gostek_help(4, &Bots.gostek[UDO][dir]);
        draw_gostek_help(4);

        // prawe udo
        glColor3ub(color[PANTS][0], color[PANTS][1], color[PANTS][2]);
        //draw_gostek_help(1, &Bots.gostek[UDO][dir]);
        draw_gostek_help(1);
        glColor3f(1,1,1);

        // lewa stopa
        //draw_gostek_help(3, &Bots.gostek[STOPA][dir]);
        draw_gostek_help(3);

        // prawa stopa
        //draw_gostek_help(0, &Bots.gostek[STOPA][dir]);
        draw_gostek_help(0);

        // prawe ramie
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        //draw_gostek_help(6, &Bots.gostek[RAMIE][dir]);
        draw_gostek_help(6);

        // prawa reka
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        //draw_gostek_help(7, &Bots.gostek[REKA][dir]);
        draw_gostek_help(7);

        // prawa dlon
        glColor3ub(color[SKIN][0], color[SKIN][1], color[SKIN][2]);
        //draw_gostek_help(8, &Bots.gostek[DLON][dir]);
        draw_gostek_help(8);

        // (chest) klata
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        //draw_gostek_help(10, &Bots.gostek[KLATA][dir]);
        draw_gostek_help(10);

        // (head) morda
        glColor3ub(color[SKIN][0], color[SKIN][1], color[SKIN][2]);
        //draw_gostek_help(11, &Bots.gostek[MORDA][dir]);
        draw_gostek_help(11);

        // lewe ramie
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        //draw_gostek_help(12, &Bots.gostek[RAMIE][dir]);
        draw_gostek_help(12);

        // lewa reka
        glColor3ub(color[SHIRT][0], color[SHIRT][1], color[SHIRT][2]);
        //draw_gostek_help(13, &Bots.gostek[REKA][dir]);
        draw_gostek_help(13);

        // lewa dlon
        glColor3ub(color[SKIN][0], color[SKIN][1], color[SKIN][2]);
        //draw_gostek_help(14, &Bots.gostek[DLON][dir]);
        draw_gostek_help(14);
        glColor3f(1,1,1);

    }

}


void Bot::update()
{

    if (!isKilled && (world.getCurrentTime - timerChangeFrame >= 30))
    {
        timerChangeFrame = world.getCurrentTime;
        if (currentFrame >= Bots.FRAMES_MAX[movementType]-1)
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
