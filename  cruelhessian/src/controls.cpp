/*   controls.cpp
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


#include "worldmap.h"


SDL_Event event;
Uint8* key;


void WorldMap::inputUser()
{
    key = SDL_GetKeyState(NULL);

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            CHOICE_EXIT = true;
        }
        getGLpos();
        if (event.type == SDL_KEYDOWN)
        {
            KEY_PRESSED = event.key.keysym.sym;
            if (KEY_PRESSED == SDLK_ESCAPE)
            {
                SHOW_ESC = !SHOW_ESC;
            }
            if (KEY_PRESSED == SDLK_F1)
            {
                SHOW_SCORES = !SHOW_SCORES;
            }
            if (KEY_PRESSED == SDLK_F3)
            {
                SHOW_STATS = !SHOW_STATS;
            }
            if (KEY_PRESSED == SDLK_F4)
            {
                takeScreenshot();
            }
            if (KEY_PRESSED == SDLK_F5 && MUSIC_VOL > 0 && !gMusicList.empty())
            {
                if (Mix_PausedMusic() == 0)
                    //Mix_Pause(-1);
                    Mix_PauseMusic();
                else
                    //Mix_Resume(-1);
                    Mix_ResumeMusic();
            }
            if (KEY_PRESSED == SDLK_F6)
            {
                playMusic(-1);
            }
            if (KEY_PRESSED == SDLK_F7)
            {
                playMusic(1);
            }
            if (SHOW_GUN_MENU || SHOW_ESC || CHOICE_EXIT) return;

            if (KEY_PRESSED == KEY_GRENADE)
            {
                if (!CHOICE_GUN && (bot[MY_BOT_NR]->numGrenades >= 1))
                {
                    addGrenade(MY_BOT_NR, static_cast<float>(MOUSE_X), static_cast<float>(MOUSE_Y), 200);
                }
            }
            if (KEY_PRESSED == SDLK_s)
            {
                bot[MY_BOT_NR]->movementType = KUCA;
            }
        }
        if (SHOW_GUN_MENU || SHOW_ESC || CHOICE_EXIT) return;

        // single shot
        // only USSCOM, Dessert Eagles, M79, ...
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            if (bot[MY_BOT_NR]->gunModel == 0 || bot[MY_BOT_NR]->gunModel == 1 || bot[MY_BOT_NR]->gunModel == 5 ||
                    bot[MY_BOT_NR]->gunModel == 6 || bot[MY_BOT_NR]->gunModel == 8)
            {
                addBullet(MY_BOT_NR, static_cast<float>(MOUSE_X), static_cast<float>(MOUSE_Y));
            }
        }
    }
    if (SHOW_GUN_MENU || SHOW_ESC || CHOICE_EXIT || bot[MY_BOT_NR]->isKilled) return;


    // ************  mouse

    if (MOUSE_X <= bot[MY_BOT_NR]->position.x)
    {
        bot[MY_BOT_NR]->movementDirection = LEFT;
        bot[MY_BOT_NR]->movementType = STOI;
    }
    else if (MOUSE_X > bot[MY_BOT_NR]->position.x)
    {
        bot[MY_BOT_NR]->movementDirection = RIGHT;
        bot[MY_BOT_NR]->movementType = STOI;
    }

    // right mouse button
    if ((SDL_GetMouseState(NULL, NULL)&SDL_BUTTON_RMASK) && (bot[MY_BOT_NR]->procJet >= 0.004))
    {
        bot[MY_BOT_NR]->isFlying = true;
        bot[MY_BOT_NR]->movementType = GORA;

        //if (SOUNDS_VOL > 0) Mix_PlayChannel(-1, Mix_LoadWAV("Sfx/jump.wav"), 0);
        if (bot[MY_BOT_NR]->procJet >= 2*JET_CHANGE)
            bot[MY_BOT_NR]->procJet -= JET_CHANGE;
    }
    else if (bot[MY_BOT_NR]->procJet >= 0.000)
    {
        if (bot[MY_BOT_NR]->procJet <= 1-JET_CHANGE)
        {
            bot[MY_BOT_NR]->procJet += JET_CHANGE;
        }
        bot[MY_BOT_NR]->isFlying = false;

    }

    // left mouse button
    // don't stop fire
    if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON_LMASK)
    {
        if (bot[MY_BOT_NR]->gunModel == 2 || bot[MY_BOT_NR]->gunModel == 3 || bot[MY_BOT_NR]->gunModel == 4 ||
                bot[MY_BOT_NR]->gunModel == 7 || bot[MY_BOT_NR]->gunModel == 9 || bot[MY_BOT_NR]->gunModel == 10)
        {
            addBullet(MY_BOT_NR, static_cast<float>(MOUSE_X), static_cast<float>(MOUSE_Y));
        }
    }

    // **********  keyboard

    if (key[KEY_DOWN])
    {
        moveBotDown(MY_BOT_NR);
    }

    if (key[KEY_LEFT])
    {
        moveBotLeft(MY_BOT_NR);
        if (key[KEY_DOWN])
            bot[MY_BOT_NR]->movementType = (bot[MY_BOT_NR]->movementDirection == LEFT) ? SKOK_DOL_OBROT : SKOK_DOL_OBROT_TYL;
    }

    if (key[KEY_RIGHT])
    {
        moveBotRight(MY_BOT_NR);
        if (key[KEY_DOWN])
            bot[MY_BOT_NR]->movementType = (bot[MY_BOT_NR]->movementDirection == RIGHT) ? SKOK_DOL_OBROT : SKOK_DOL_OBROT_TYL;
    }

    if (key[KEY_UP])
    {
        moveBotUp(MY_BOT_NR);
    }

    if (key[KEY_LEFT] && key[KEY_UP])
    {
        moveBotJumpLeft(MY_BOT_NR);
    }

    if (key[KEY_RIGHT] && key[KEY_UP])
    {
        moveBotJumpRight(MY_BOT_NR);
    }

    if ((key[KEY_RELOAD] && bot[MY_BOT_NR]->leftAmmos < weapon[bot[MY_BOT_NR]->gunModel].ammo) || bot[MY_BOT_NR]->isReloading)
    {
        gunReloading(MY_BOT_NR);
    }

}


static inline bool ccw(TVector2D A, float bx, float by, float cx, float cy)
{
    return ((cy-A.y)*(bx-A.x) > (by-A.y)*(cx-A.x));
}

static inline bool ccw(TVector2D A, TVector2D B, float cx, float cy)
{
    return ((cy-A.y)*(B.x-A.x) > (B.y-A.y)*(cx-A.x));
}


// sprawdzanie, czy boty sie widza
// first - numer pierwszego bota
// second numer drugiego
bool WorldMap::do_bots_see(int first, int second)
{

    // A - bot[first].position.x, bot[first].position.y
    // B - bot[second].position.x, bot[second].position.y
    // C - xpoly[i].vertex[k].x, xpoly[i].vertex[k].y
    // D - xpoly[i].vertex[k+1].x, xpoly[i].vertex[k+1].y

    for (int i = 0; i < (int)p.polygonCount; ++i) // sprawdz kazdy trojkat, czy koliduje z linia
    {
        if (p.polygon[i].polyType != p.ptONLY_BULLETS_COLLIDE && p.polygon[i].polyType != p.ptNO_COLLIDE)
        {
            // 0, 1
            if ((ccw(bot[first]->position, p.polygon[i].vertex[0].x, p.polygon[i].vertex[0].y, p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y) !=
                    ccw(bot[second]->position, p.polygon[i].vertex[0].x, p.polygon[i].vertex[0].y, p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y)) &&
                    (ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[0].x, p.polygon[i].vertex[0].y) !=
                     ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y)))
                return false;

            // 1, 2
            if ((ccw(bot[first]->position, p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y, p.polygon[i].vertex[2].x, p.polygon[i].vertex[2].y) !=
                    ccw(bot[second]->position, p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y, p.polygon[i].vertex[2].x, p.polygon[i].vertex[2].y)) &&
                    (ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y) !=
                     ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[2].x, p.polygon[i].vertex[2].y)))
                return false;

        }
    }
    return true;

}


void WorldMap::bots_control()
{
    int dest;
    unsigned int j;
    std::vector<int> ostrz_list;

    // shooting (strzelajacy)
    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (i != MY_BOT_NR)
        {
            // optimize it !!!!!!!!!!!!!!!!!!!!!!!!1
            for (j = 0; j < bot.size(); ++j)
            {
                // stworz liste mozliwych ostrzeliwanych
                if (!bot[i]->isKilled && !bot[j]->isKilled && i != j && do_bots_see(i, j))
                {
                    if ((bot[i]->team == bot[j]->team && FRIENDLY_FIRE) || bot[i]->team != bot[j]->team)
                    {
                        // bot i saw bot j
                        ostrz_list.push_back(j);
                        //chat_list.push_back(bot[i]->chatSeeEnemy);
                    }
                }
            }
            if (ostrz_list.size() > 0)
            {
                // j - wylosuj ostrzeliwanego
                j = ostrz_list[rand() % ostrz_list.size()];
                if (bot[j]->position.x > bot[i]->position.x)
                {
                    if (bot[i]->movementDirection == LEFT)
                        bot[i]->movementDirection = RIGHT;
                }
                else
                {
                    if (bot[i]->movementDirection == RIGHT)
                        bot[i]->movementDirection = LEFT;
                }
                addBullet(i, bot[j]->position.x, bot[j]->position.y);
                ostrz_list.clear();
            }
        }
    }

    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (i != MY_BOT_NR)
        {
            // waypoints
            dest = bot[i]->destinationPoint;

            if (p.waypoint[dest].right)
            {
                moveBotRight(i);
                bot[i]->movementDirection = RIGHT;
            }
            if (p.waypoint[dest].left)
            {
                moveBotLeft(i);
                bot[i]->movementDirection = LEFT;
            }
            if (p.waypoint[dest].up) moveBotUp(i);
            if (p.waypoint[dest].down) moveBotDown(i);
            if (p.waypoint[dest].jet) moveBotJet(i);


            //if (doszedl(i))
            if (bot[i]->is_inside(p.waypoint[bot[i]->destinationPoint].x, p.waypoint[bot[i]->destinationPoint].y))
            {
                //cout << "Doszedl";
                if (p.waypoint[dest].numConnections == 0)
                    bot[i]->destinationPoint = p.waypoint[dest].connections[0]-1;
                else
                    bot[i]->destinationPoint = p.waypoint[dest].connections[static_cast<int>(rand() % p.waypoint[dest].numConnections)]-1;
            }
        }
    }

}

