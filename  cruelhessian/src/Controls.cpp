/*   Controls.cpp
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


#include "WorldMap.h"
#include "Game.h"
#include "AudioManager.h"
#include "ParserManager.h"
#include "BotManager.h"
#include "WeaponManager.h"
#include "Bot.h"
#include "Map.h"
#include "Body.h"
#include "Mouse.h"
#include "WindowExit.h"
#include "WindowGuns.h"
#include "textinout/CommandLine.h"
#include "textinout/ChatLine.h"
#include <SFML/Graphics.hpp>


void WorldMap::inputUser()
{

    sf::Event event;

    while (game.App.GetEvent(event))
    {
        if (event.Type == sf::Event::Closed)
        {
            //    App.Close();
            CHOICE_EXIT = true;
        }

        if (event.Type == sf::Event::KeyPressed)
        {

            onKeyDown(event.Key.Code);

            KEY_PRESSED = event.Key.Code;

            if (KEY_PRESSED == sf::Key::Escape)
            {
                if (window_exit->GetSingletonPtr() != NULL)
                {
                    delete window_exit;
                }
                else
                {
                    window_exit = new WindowExit();
                }
            }
            else if (KEY_PRESSED == sf::Key::F1)
            {
                SHOW_SCORES = !SHOW_SCORES;
            }
            else if (KEY_PRESSED == sf::Key::F3)
            {
                SHOW_STATS = !SHOW_STATS;
            }
            else if (KEY_PRESSED == sf::Key::F4)
            {
                takeScreenshot();
            }
            else if (KEY_PRESSED == sf::Key::F5 && Parser.MUSIC_VOL > 0)// && !Audio.gMusicList.empty())
            {
                if (Audio.music->GetStatus() == sf::Music::Paused)
                    Audio.music->Pause();
                else
                    Audio.music->Play();
            }
            else if (KEY_PRESSED == sf::Key::F6)
            {
                Audio.playMusic(-1);
            }
            else if (KEY_PRESSED == sf::Key::F7)
            {
                Audio.playMusic(1);
            }

            if (window_guns->GetSingletonPtr() != NULL || window_exit->GetSingletonPtr() != NULL || CHOICE_EXIT) return;

            if (KEY_PRESSED == Parser.KEY_GRENADE)
            {
                if (!CHOICE_GUN && (bot[MY_BOT_NR]->numGrenades >= 1))
                {
                    bot[MY_BOT_NR]->ThrowGrenade(mouse->getGlobalPosition(), 200);
                }
                else if (!CHOICE_GUN && (bot[MY_BOT_NR]->numClusters >= 1))
                {
                    bot[MY_BOT_NR]->ThrowCluster(mouse->getGlobalPosition(), 200);
                }
            }
            if (KEY_PRESSED == Parser.KEY_DOWN)
            {
                bot[MY_BOT_NR]->movementType = KUCA;
            }

            if (command_line->GetSingletonPtr() != NULL && chat_line->GetSingletonPtr() == NULL)
            {
                command_line->Query(KEY_PRESSED);
            }
            if (KEY_PRESSED == sf::Key::Slash && chat_line->GetSingletonPtr() == NULL && command_line->GetSingletonPtr() == NULL)
            {
                command_line = new CommandLine();
            }

            if (chat_line->GetSingletonPtr() != NULL && command_line->GetSingletonPtr() == NULL)
            {
                chat_line->Query(KEY_PRESSED);
            }
            if (KEY_PRESSED == Parser.KEY_CHAT && command_line->GetSingletonPtr() == NULL && chat_line->GetSingletonPtr() == NULL)
            {
                chat_line = new ChatLine();
            }

        }
        else if (event.Type == sf::Event::KeyReleased)
        {
            onKeyUp(event.Key.Code);
        }

        if (window_guns->GetSingletonPtr() != NULL || window_exit->GetSingletonPtr() != NULL || CHOICE_EXIT || command_line->GetSingletonPtr() != NULL || chat_line->GetSingletonPtr() != NULL) return;

        if (event.Type == sf::Event::MouseButtonPressed)
        {
            onMouseButtonDown(event.MouseButton.Button);
        }
        else if (event.Type == sf::Event::MouseButtonReleased)
        {
            onMouseButtonUp(event.MouseButton.Button);
        }

    }

    if (window_guns->GetSingletonPtr() != NULL || window_exit->GetSingletonPtr() != NULL || CHOICE_EXIT || bot[MY_BOT_NR]->isKilled) return;


// ************  mouse

    if (mouse->getGlobalX() <= bot[MY_BOT_NR]->GetPosition().x)
    {
        // std::cout << "LEFT" << std::endl;
        bot[MY_BOT_NR]->movementDirection = LEFT;
    }
    else if (mouse->getGlobalX() > bot[MY_BOT_NR]->GetPosition().x)
    {
        //std::cout << "RIGHT" << std::endl;
        bot[MY_BOT_NR]->movementDirection = RIGHT;
    }

// right mouse button
    const sf::Input& Input = game.App.GetInput();

    //if ((SDL_GetMouseState(NULL, NULL)&SDL_BUTTON_RMASK) && (bot[MY_BOT_NR]->procJet >= 0.01))
    if (Input.IsMouseButtonDown(sf::Mouse::Right) && (bot[MY_BOT_NR]->procJet >= 0.01))
    {

        //if (SOUNDS_VOL > 0) Mix_PlayChannel(-1, Mix_LoadWAV("Sfx/jump.wav"), 0);
        if (bot[MY_BOT_NR]->procJet >= 2*JET_CHANGE)
        {
            bot[MY_BOT_NR]->procJet -= JET_CHANGE;
            bot[MY_BOT_NR]->movementType = GORA;
        }

    }
    else if (bot[MY_BOT_NR]->procJet >= 0.000)
    {
        if (bot[MY_BOT_NR]->procJet <= 1-JET_CHANGE)
        {
            bot[MY_BOT_NR]->procJet += JET_CHANGE;
        }

    }

// left mouse button
// don't stop fire
    if (Input.IsMouseButtonDown(sf::Mouse::Left))
    {
        if (bot[MY_BOT_NR]->gunModel == 2 || bot[MY_BOT_NR]->gunModel == 3 || bot[MY_BOT_NR]->gunModel == 4 ||
                bot[MY_BOT_NR]->gunModel == 7 || bot[MY_BOT_NR]->gunModel == 9 || bot[MY_BOT_NR]->gunModel == 10)
        {
            bot[MY_BOT_NR]->Shot(mouse->getGlobalPosition());
        }
    }

}


void WorldMap::onMouseButtonDown(sf::Mouse::Button& butt)
{

    if (butt == sf::Mouse::Right)
    {
        bot[MY_BOT_NR]->isAbleToFly = true;
    }
    // single shot
    // only USSCOM, Dessert Eagles, M79, ...
    else if (butt == sf::Mouse::Left)
    {
        if (bot[MY_BOT_NR]->gunModel == 0 || bot[MY_BOT_NR]->gunModel == 1 || bot[MY_BOT_NR]->gunModel == 5 ||
                bot[MY_BOT_NR]->gunModel == 6 || bot[MY_BOT_NR]->gunModel == 8)
        {
            bot[MY_BOT_NR]->Shot(mouse->getGlobalPosition());
        }

    }
}

void WorldMap::onMouseButtonUp(sf::Mouse::Button& butt)
{

    //std::cout << "W GORE PRAWY" << std::endl;
    if (butt == sf::Mouse::Right)
    {
        bot[MY_BOT_NR]->isAbleToFly = false;
    }

}



void WorldMap::onKeyDown(sf::Key::Code sym)
{
//std::cout << "donw" << std::endl;
    if (sym == Parser.KEY_LEFT)
        bot[MY_BOT_NR]->setMoveLeft(true);
    else if (sym == Parser.KEY_RIGHT)
        bot[MY_BOT_NR]->setMoveRight(true);
    else if (sym == Parser.KEY_UP)
        bot[MY_BOT_NR]->setMoveUp(true);
    else if (sym == Parser.KEY_DOWN)
        bot[MY_BOT_NR]->setMoveDown(true);
    else if (sym == Parser.KEY_RELOAD && bot[MY_BOT_NR]->leftAmmos < Weapons[bot[MY_BOT_NR]->gunModel].ammo)
        gunReloading(MY_BOT_NR);

}


void WorldMap::onKeyUp(sf::Key::Code sym)
{
//std::cout << "up" << std::endl;
    if (sym == Parser.KEY_LEFT)
        bot[MY_BOT_NR]->setMoveLeft(false);
    else if (sym == Parser.KEY_RIGHT)
        bot[MY_BOT_NR]->setMoveRight(false);
    else if (sym == Parser.KEY_UP)
        bot[MY_BOT_NR]->setMoveUp(false);
    else if (sym == Parser.KEY_DOWN)
        bot[MY_BOT_NR]->setMoveDown(false);

}


static inline bool ccw(const TVector2D& A, float bx, float by, float cx, float cy)
{
    return ((cy-A.y)*(bx-A.x) > (by-A.y)*(cx-A.x));
}

static inline bool ccw(const TVector2D& A, const TVector2D& B, float cx, float cy)
{
    return ((cy-A.y)*(B.x-A.x) > (B.y-A.y)*(cx-A.x));
}


// sprawdzanie, czy boty sie widza
// first - numer pierwszego bota
// second numer drugiego
bool WorldMap::do_bots_see(unsigned int first, unsigned int second)
{

    // A - bot[first].position.x, bot[first].position.y
    // B - bot[second].position.x, bot[second].position.y
    // C - xpoly[i].vertex[k].x, xpoly[i].vertex[k].y
    // D - xpoly[i].vertex[k+1].x, xpoly[i].vertex[k+1].y

    /*for (unsigned int i = 0; i < static_cast<unsigned int>(map->polygonCount); ++i) // sprawdz kazdy trojkat, czy koliduje z linia
    {
        if (map->polygon[i].polyType != map->ptONLY_BULLETS_COLLIDE && map->polygon[i].polyType != map->ptNO_COLLIDE)
        {
            // 0, 1
            if ((ccw(bot[first]->position, map->polygon[i].vertex[0].x, map->polygon[i].vertex[0].y, map->polygon[i].vertex[1].x, map->polygon[i].vertex[1].y) !=
                    ccw(bot[second]->position, map->polygon[i].vertex[0].x, map->polygon[i].vertex[0].y, map->polygon[i].vertex[1].x, map->polygon[i].vertex[1].y)) &&
                    (ccw(bot[first]->position, bot[second]->position, map->polygon[i].vertex[0].x, map->polygon[i].vertex[0].y) !=
                     ccw(bot[first]->position, bot[second]->position, map->polygon[i].vertex[1].x, map->polygon[i].vertex[1].y)))
                return false;

            // 1, 2
            if ((ccw(bot[first]->position, map->polygon[i].vertex[1].x, map->polygon[i].vertex[1].y, map->polygon[i].vertex[2].x, map->polygon[i].vertex[2].y) !=
                    ccw(bot[second]->position, map->polygon[i].vertex[1].x, map->polygon[i].vertex[1].y, map->polygon[i].vertex[2].x, map->polygon[i].vertex[2].y)) &&
                    (ccw(bot[first]->position, bot[second]->position, map->polygon[i].vertex[1].x, map->polygon[i].vertex[1].y) !=
                     ccw(bot[first]->position, bot[second]->position, map->polygon[i].vertex[2].x, map->polygon[i].vertex[2].y)))
                return false;

        }
    }*/
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
                if (!bot[i]->isKilled && !bot[j]->isKilled && do_bots_see(i, j))
                {
                    if ((bot[i]->team == bot[j]->team && FRIENDLY_FIRE) || bot[i]->team != bot[j]->team)
                    {
                        // bot i saw bot j
                        ostrz_list.push_back(j);
                    }
                }
            }
            if (!ostrz_list.empty())
            {
                // j - wylosuj ostrzeliwanego
                j = ostrz_list[rand() % ostrz_list.size()];
                if (bot[j]->GetPosition().x > bot[i]->GetPosition().x)
                {
                    if (bot[i]->movementDirection == LEFT)
                        bot[i]->movementDirection = RIGHT;
                }
                else
                {
                    if (bot[i]->movementDirection == RIGHT)
                        bot[i]->movementDirection = LEFT;
                }
                bot[i]->Shot(bot[j]->GetPosition());
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
            // fix it !!!!!!!!!!!!!!
            if (dest <= 0 || !map->waypoint[dest].active)
                continue;
            if (map->waypoint[dest].right)
            {
                //Bots.moveRight(i);
                //bot[i]->movementDirection = RIGHT;
                bot[i]->setMoveRight(true);
            }
            else if (map->waypoint[dest].left)
            {
                //Bots.moveLeft(i);
                //bot[i]->movementDirection = LEFT;
                bot[i]->setMoveLeft(true);
            }
            if (map->waypoint[dest].up)
            {
                //Bots.moveUp(i);
                bot[i]->setMoveUp(true);
            }
            else if (map->waypoint[dest].down)
            {
                //   Bots.moveDown(i);
                bot[i]->setMoveDown(true);
            }
            if (map->waypoint[dest].jet)
            {
                //   Bots.moveJet(i);
            }


            //if (doszedl(i))
            /*            if (bot[i]->is_inside(map->waypoint[bot[i]->destinationPoint].x, map->waypoint[bot[i]->destinationPoint].y))
                        {
                            //cout << "Doszedl";
                            if (map->waypoint[dest].numConnections == 0)
                                bot[i]->destinationPoint = map->waypoint[dest].connections[0]-1;
                            else
                                bot[i]->destinationPoint = map->waypoint[dest].connections[static_cast<int>(rand() % map->waypoint[dest].numConnections)]-1;
                        }*/
        }
    }

}
