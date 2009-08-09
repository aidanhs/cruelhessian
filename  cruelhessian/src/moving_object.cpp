/*   moving_objects.cpp
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


#include "bot.h"
#include "moving_object.h"
#include <set>



// funkcja oblicza, czy rzuty przesunietego soldiera o [dx, dy] i trojkata [tr_number] na os o wspolcz. [aa] sie pokrywaja
bool MovingObject::rzuty_bots(int tr_number, int vec_nr, float dx, float dy)
{
    float x[4];
    float min_s, max_s, min_t, max_t;

    // --------------   rzutowanie soldiera

    x[0] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * (this->position.y + dy) + this->position.x + dx);
    x[1] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * (this->position.y + dy) + this->position.x + this->w + dx);
    x[2] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * (this->position.y + this->h + dy) + this->position.x + this->w + dx);
    x[3] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * (this->position.y + this->h + dy) + this->position.x + dx);


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
    x[0] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * p.polygon[tr_number].vertex[0].y + p.polygon[tr_number].vertex[0].x);
    x[1] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * p.polygon[tr_number].vertex[1].y + p.polygon[tr_number].vertex[1].x);
    x[2] = con[tr_number][vec_nr] * (aa[tr_number][vec_nr] * p.polygon[tr_number].vertex[2].y + p.polygon[tr_number].vertex[2].x);

    // wyznaczanie min i max z x0 .. x2
    min_t = max_t = x[0];
    if (x[1] > max_t) max_t = x[1];
    if (x[1] < min_t) min_t = x[1];
    if (x[2] > max_t) max_t = x[2];
    if (x[2] < min_t) min_t = x[2];

    return !((max_s <= min_t) || (max_t <= min_s));
}


// improved version
// dx, dy - przesuniecie obiektu
// zwraca:
// -1 - brak kolizji
// i - jest kolizja z tym trojkatem

static inline int minn(int a, int b)
{
    return (a < b) ? a : b;
}

static inline int maxx(int a, int b)
{
    return (a > b) ? a : b;
}

int MovingObject::collision_det_with_wall(float dx, float dy)
{
    std::set<unsigned int> trian_num;
    std::vector<unsigned int> sect_num;
    int a[4];

    a[0] = static_cast<int>((position.x + dx) / p.sectorDivisions) + p.numSectors;
    a[1] = static_cast<int>((position.y + dy) / p.sectorDivisions) + p.numSectors;
    a[2] = static_cast<int>((position.x + dx + w) / p.sectorDivisions) + p.numSectors;
    a[3] = static_cast<int>((position.y + dy + h) / p.sectorDivisions) + p.numSectors;

    // fix iterations
    for (int j = minn(a[0], a[2])-1; j <= maxx(a[0], a[2])+1; ++j)
        //for (int j = min(a[0], a[2]); j <= max(a[0], a[2]); ++j)
        for (int k = minn(a[1], a[3])-1; k <= maxx(a[1], a[3])+1; ++k)
            //for (int k = min(a[1], a[3]); k <= max(a[1], a[3]); ++k)
        {
            sect_num.push_back(51*j + k);
        }


// wyznacz numery trojkatow w tych sektorach do zbadania

    for (std::vector<unsigned int>::iterator iv = sect_num.begin(); iv != sect_num.end(); ++iv)
        for (int j = 0; j < p.sector[*iv].polyCount; ++j)
            trian_num.insert(p.sector[*iv].polys[j]-1);


    for (std::set<unsigned int>::iterator i = trian_num.begin(); i != trian_num.end(); ++i)
    {
        //cout << *i << " ";
        if (p.polygon[*i].polyType != p.ptONLY_BULLETS_COLLIDE && p.polygon[*i].polyType != p.ptNO_COLLIDE)
        {
            // (-1,0)  -  wektor poziomy soldiera jako os
            // (badany trojkat jest za lub przed soldierem po osi X)
            if ((tr_minx[*i] > this->position.x + this->w + dx) || (tr_maxx[*i] < this->position.x + dx))
                continue; // nie ma kolizji z tym trojkatem

            // (0,-1)  -  wektor pionowy soldiera jako os
            // (badany trojkat jest pod lub nad soldierem po osi Y)
            if ((tr_miny[*i] > this->position.y + this->h + dy) || (tr_maxy[*i] < this->position.y + dy))
                continue;

            // normalna wierzcholka 0 trojkata jako os  (wierzch 0 1)
            if (//p.polygon[i].perpendicular[0].x != 0 &&
                //p.polygon[i].perpendicular[0].y != 0 &&
                !rzuty_bots(*i, 0, dx, dy))
                continue;

            // normalna wierzcholka 1 trojkata jako os (wierzch 1 2)
            if (//p.polygon[i].perpendicular[1].x != 0 &&
                // p.polygon[i].perpendicular[1].y != 0 &&
                !rzuty_bots(*i, 1, dx, dy))
                continue;

            // normalna wierzcholka 2 trojkata jako os  (wierzch 2 0)
            if (//p.polygon[i].perpendicular[2].x != 0.0 &&
                // p.polygon[i].perpendicular[2].y != 0.0 &&
                !rzuty_bots(*i, 2, dx, dy))
                continue;

            return *i;       // jest kolizja
        }
    }

    return -1;
}


// use 'flounder' code from gamedev.net
int Bot::collision_det_with_wall(float dx, float dy)
{
    std::set<unsigned int> trian_num;
    std::vector<unsigned int> sect_num;
    TVector2D norm, pos, ta, tb, tc;
    float d0, d1, col_dist, min, max;
    int a[4];

    pos.x = position.x + dx;
    pos.y = position.y + dy;

// punkt ciezkosci obiektu znajduje sie w sektorze:

    a[0] = static_cast<int>((position.x - r + dx) / p.sectorDivisions) + p.numSectors;
    a[1] = static_cast<int>((position.y - r + dy) / p.sectorDivisions) + p.numSectors;
    a[2] = static_cast<int>((position.x + r + dx) / p.sectorDivisions) + p.numSectors;
    a[3] = static_cast<int>((position.y + r + dy) / p.sectorDivisions) + p.numSectors;

    // fix iterations
    for (int j = minn(a[0], a[2])-1; j <= maxx(a[0], a[2])+1; ++j)
    //for (int j = min(a[0], a[2]); j <= max(a[0], a[2]); ++j)
        for (int k = minn(a[1], a[3])-1; k <= maxx(a[1], a[3])+1; ++k)
        //for (int k = min(a[1], a[3]); k <= max(a[1], a[3]); ++k)
        {
            sect_num.push_back(51*j + k);
        }


// wyznacz numery trojkatow w tych sektorach do zbadania

    for (std::vector<unsigned int>::iterator s = sect_num.begin(); s != sect_num.end(); ++s)
        for (int j = 0; j < p.sector[*s].polyCount; j++)
            trian_num.insert(p.sector[*s].polys[j]-1);


    for (std::set<unsigned int>::iterator i = trian_num.begin(); i != trian_num.end(); ++i)
    {

        if (p.polygon[*i].polyType != p.ptONLY_BULLETS_COLLIDE && p.polygon[*i].polyType != p.ptNO_COLLIDE)
        {
            ta = TVector2D(p.polygon[*i].vertex[0].x, p.polygon[*i].vertex[0].y);
            tb = TVector2D(p.polygon[*i].vertex[1].x, p.polygon[*i].vertex[1].y);
            tc = TVector2D(p.polygon[*i].vertex[2].x, p.polygon[*i].vertex[2].y);

            //normal tests
            // first
            norm = TVector2D(p.polygon[*i].perpendicular[0].x, p.polygon[*i].perpendicular[0].y);
            d0 = (ta - pos) * norm;
            d1 = (pos - tc) * norm;
            col_dist = d0 > d1 ? d0 : d1;
            if (col_dist > r)
            {
                continue;
            }

            // second
            norm = TVector2D(p.polygon[*i].perpendicular[1].x, p.polygon[*i].perpendicular[1].y);
            d0 = (tb - pos) * norm;
            d1 = (pos - ta) * norm;
            d0 = d0 > d1 ? d0 : d1;
            if (d0 > r)
            {
                continue;
            }
            if (d0 > col_dist)
            {
                col_dist = d0;
            }

            // third
            norm = TVector2D(p.polygon[*i].perpendicular[2].x, p.polygon[*i].perpendicular[2].y);
            d0 = (tc - pos) * norm;
            d1 = (pos - tb) * norm;
            d0 = d0 > d1 ? d0 : d1;
            if (d0 > r)
            {
                continue;
            }
            if (d0 > col_dist)
            {
                col_dist = d0;
            }

            //  vertex test
            // first
            norm = ta - pos;
            norm.normalize();
            d0 = tb * norm;
            d1 = tc * norm;
            max = d0 > d1 ? d0 : d1;
            min = d0 + d1 - max;
            d0 = ta * norm;
            max = d0 > max ? d0 : max;
            min = d0 < min ? d0 : min;
            d0 = pos * norm;
            max = d0 - max;
            min = min - d0;
            max = max > min ? max : min;
            if (max > r)
            {
                continue;
            }
            if (max > col_dist)
            {
                col_dist = max;
            }

            // second
            norm = tb - pos;
            norm.normalize();
            d0 = tb * norm;
            d1 = tc * norm;
            max = d0 > d1 ? d0 : d1;
            min = d0 + d1 - max;
            d0 = ta * norm;
            max = d0 > max ? d0 : max;
            min = d0 < min ? d0 : min;
            d0 = pos * norm;
            max = d0 - max;
            min = min - d0;
            max = max > min ? max : min;
            if (max > r)
            {
                continue;
            }
            if (max > col_dist)
            {
                col_dist = max;
            }

            // third
            norm = tc - pos;
            norm.normalize();
            d0 = tb * norm;
            d1 = tc * norm;
            max = d0 > d1 ? d0 : d1;
            min = d0 + d1 - max;
            d0 = ta * norm;
            max = d0 > max ? d0 : max;
            min = d0 < min ? d0 : min;
            d0 = pos * norm;
            max = d0 - max;
            min = min - d0;
            max = max > min ? max : min;
            if (max > r)
            {
                continue;
            }

            return *i;
        }
    }
    return -1;
}

