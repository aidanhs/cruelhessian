/*   bot.h
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

#ifndef BOT_H
#define BOT_H


#include "moving_objects.h"

extern int FRAMES_MAX[50], BODYPARTS_MAX[50];
extern float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie

typedef enum MT
{
    BIEGA = 0,
    BIEGA_TYL,
    CELUJE,
    CIESZY,
    GORA,
    KUCA,
    RZUCA,
    SKOK,
    SPADA,
    STOI,
    ZMIEN_BRON
} MT;

typedef enum MD
{
    LEFT = 0,
    RIGHT
} MD;


class Bot : public MovingObjects
{
public:
    std::string name;
    int killer;
    bool isFlying;
    bool isReloading;
    bool isShooting;
    bool isKilled;
    int gunModel;
    float shotPointX;
    float shotPointY;
    int botNumber;
    int leftAmmos;
    float procJet;
    float procLife;
    int wayNumber;
    int numGrenades;
    Uint32 timerChangeFrame;
    Uint32 respawnTime;
    int destinationPoint;
    Uint32 timerRespawnTime;
    int currentFrame;       // aktualnie wyswietlana ramka
    Uint32 lastShotTime;
    Uint32 startReloadingTime;
    MD movementDirection;   // kierunek poruszania sie bota
    MT movementType;        // sposob poruszania sie

    void move_left();
    void move_right();
    void move_up();
    void move_down();
    void move_jet();
    int find_nearest_waypoint_nr(int spawn_nr);
    bool is_inside(int x, int y);

    Bot(int bot_nr, std::string name, float posx, float posy, int gunmodel, int spawn_nr);
};

//void read_poas();
void make_lines(MT move, int frame, int kierunek);

extern Bot **bot;

#endif

