/*   bot.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawe³ Konieczny <konp84@gmail.com>
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


#include "moving_object.h"
#include "bullet.h"
//#include "worldmap.h"

//class WorldMap;

class Bot : public MovingObject//, public Gun
{
    //friend class Bullet;
public:
int collision_det_with_wall(float dx, float dy);
    std::string name;
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;

    float r;
    int points;
    int deaths;
    int wayNumber;
    int botNumber;
    int killer;
    int killed;
    int gunModel;
    int numGrenades;
    int leftAmmos;
    int currentFrame;       // aktualnie wyswietlana ramka
    int destinationPoint;
    Uint32 lastShotTime;
    Uint32 startReloadingTime;
    Uint32 timerRespawnTime;
    Uint32 respawnTime;
    Uint32 timerChangeFrame;
    Uint32 youKilledTime;

    bool isShooting;
    bool isKilled;
    bool isFlying;
    bool isReloading;

    float shotPointX;
    float shotPointY;
    float procJet;
    float actLife;

    TEAM team;

    MT movementType;        // sposob poruszania sie
    MD movementDirection;   // kierunek poruszania sie bota

/*    void move_left(WorldMap& wor);
    void move_right(WorldMap& wor);
    void move_up(WorldMap& wor);
    void move_down(WorldMap& wor);
    void move_jet(WorldMap& wor);
*/
    //Bot() {};
    Bot(unsigned int bot_nr, const std::string& name, int spawn_nr, int gunmodel, TEAM team);
    //Bot(int bot_nr, std::string name, float posx, float posy, int gunmodel, int spawn_nr);

    int find_nearest_waypoint_nr(int spawn_nr);
    //static void make_lines(MT move, int frame, int kierunek);
//private:
    bool is_inside(int x, int y);
};

//void read_poas();
void make_lines(MT move, int frame, int kierunek, TEAM team);
extern std::vector<Bot *> bot;


#endif
