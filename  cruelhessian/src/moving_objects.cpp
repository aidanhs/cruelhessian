/*   moving_objects.cpp
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


#include "bot.h"
#include "worldmap.h"
#include "moving_objects.h"



// funkcja oblicza, czy rzuty przesunietego soldiera o [dx, dy] i trojkata [ii] na os o wspolcz. [aa] sie pokrywaja
bool MovingObjects::rzuty_bots(int tr_number, int vec_nr, float dx, float dy)
{
    //float con1 = 1/(aa[tr_number][vec_nr]*aa[tr_number][vec_nr]+1);
    float x[4];
    float min_s, max_s, min_t, max_t;

    // --------------   rzutowanie soldiera

    x[0] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (this->position.y + dy) + this->position.x + dx);
    x[1] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (this->position.y + dy) + this->position.x + this->w + dx);
    x[2] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (this->position.y + this->h + dy) + this->position.x + this->w + dx);
    x[3] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (this->position.y + this->h + dy) + this->position.x + dx);

    // wyznaczanie min i max z x0 .. x3
    min_s = max_s = x[0];

    if (x[1] > max_s) max_s = x[1];
    if (x[1] < min_s) min_s = x[1];
    if (x[2] > max_s) max_s = x[2];
    if (x[2] < min_s) min_s = x[2];
    if (x[3] > max_s) max_s = x[3];
    if (x[3] < min_s) min_s = x[3];

    // --------------   rzutowanie trojkata

    // rzutowanie wierzcholkow
    x[0] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (p.polygon[tr_number].vertex[0].y) + p.polygon[tr_number].vertex[0].x);
    x[1] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (p.polygon[tr_number].vertex[1].y) + p.polygon[tr_number].vertex[1].x);
    x[2] = world->con[tr_number][vec_nr] * (world->aa[tr_number][vec_nr] * (p.polygon[tr_number].vertex[2].y) + p.polygon[tr_number].vertex[2].x);

    // wyznaczanie min i max z x0 .. x2
    min_t = max_t = x[0];
    if (x[1] > max_t) max_t = x[1];
    if (x[1] < min_t) min_t = x[1];
    if (x[2] > max_t) max_t = x[2];
    if (x[2] < min_t) min_t = x[2];

    if ((max_s <= min_t) || (max_t <= min_s)) return false;
    else return true;
}

// zwraca
// 0 - brak kolizji
// 1 - jest kolizja
/*
bool MovingObjects::collision_det_with_wall(float dx, float dy)
{
    for (int i = 0; i < p.polygonCount; i++) // sprawdz kazdy trojkat, czy koliduje z obiektem
    {
        if (p.polygon[i].polyType != ptONLY_BULLETS_COLLIDE && p.polygon[i].polyType != ptNO_COLLIDE)
        {

            // (-1,0)  -  wektor poziomy soldiera jako os
            // (badany trojkat jest za lub przed soldierem po osi X)
            if ((tr_minx[i] > this->position.x + this->w + dx) || (tr_maxx[i] < this->position.x + dx))
                continue; // nie ma kolizji z tym trojkatem

            // (0,-1)  -  wektor  pionowy soldiera jako os
            // (badany trojkat jest pod lub nad soldierem po osi Y)
            if ((tr_miny[i] > this->position.y + this->h + dy) || (tr_maxy[i] < this->position.y + dy))
                continue;

            // normalna wierzcholka 0 trojkata jako os  (wierzch 0 1)
            if //(p.polygon[i].perpendicular[0].x != 0 &&
                  //  p.polygon[i].perpendicular[0].y != 0 &&
                    (!rzuty_Bots(i, 0, dx, dy, this))
                continue;

            // normalna wierzcholka 1 trojkata jako os (wierzch 1 2)
            if //(p.polygon[i].perpendicular[1].x != 0 &&
                  //  p.polygon[i].perpendicular[1].y != 0 &&
                    (!rzuty_Bots(i, 1, dx, dy, this))
                continue;

            // normalna wierzcholka 2 trojkata jako os  (wierzch 2 0)
            if //(p.polygon[i].perpendicular[2].x != 0.0 &&
                 //   p.polygon[i].perpendicular[2].y != 0.0 &&
                    (!rzuty_Bots(i, 2, dx, dy, this))
                continue;

            return true;       // jest kolizja
        }
    }
    return false;
}
*/

// zwraca
// -1 - brak kolizji
// 0:p.polygonCount - jest kolizja z tym trojkatem
int MovingObjects::collision_det_with_wall(float dx, float dy)
{
    for (int i = 0; i < p.polygonCount; i++) // sprawdz kazdy trojkat, czy koliduje z obiektem
    {
        if (p.polygon[i].polyType != ptONLY_BULLETS_COLLIDE && p.polygon[i].polyType != ptNO_COLLIDE)
        {

            // (-1,0)  -  wektor poziomy soldiera jako os
            // (badany trojkat jest za lub przed soldierem po osi X)
            if ((world->tr_minx[i] > this->position.x + this->w + dx) || (world->tr_maxx[i] < this->position.x + dx))
                continue; // nie ma kolizji z tym trojkatem

            // (0,-1)  -  wektor  pionowy soldiera jako os
            // (badany trojkat jest pod lub nad soldierem po osi Y)
            if ((world->tr_miny[i] > this->position.y + this->h + dy) || (world->tr_maxy[i] < this->position.y + dy))
                continue;

            // normalna wierzcholka 0 trojkata jako os  (wierzch 0 1)
            if (//p.polygon[i].perpendicular[0].x != 0 &&
                    //p.polygon[i].perpendicular[0].y != 0 &&
                    !rzuty_bots(i, 0, dx, dy))
                continue;

            // normalna wierzcholka 1 trojkata jako os (wierzch 1 2)
            if (//p.polygon[i].perpendicular[1].x != 0 &&
                   // p.polygon[i].perpendicular[1].y != 0 &&
                    !rzuty_bots(i, 1, dx, dy))
                continue;

            // normalna wierzcholka 2 trojkata jako os  (wierzch 2 0)
            if (//p.polygon[i].perpendicular[2].x != 0.0 &&
                   // p.polygon[i].perpendicular[2].y != 0.0 &&
                    !rzuty_bots(i, 2, dx, dy))
                continue;

            return i;       // jest kolizja
        }
    }
    return -1;
}
