#ifndef WORLDMAP_H
#define WORLDMAP_H


/*   WorldMap.h
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



#include <iostream>
#include <vector>
#include <list>

#include "Enums.h"
#include "Singleton.h"
#include "TVector2D.h"
#include "interface/JetBar.h"
#include <SFML/Graphics.hpp>


class Mouse;
class TextOutput;
class CommandLine;
class ChatLine;
class Arrow;
class Background;
class Grenade;
class Bonus;
class Bullet;
class Map;
class MapFront;
class MapBack;
class Bot;
class Body;
class BotsBase;
class WindowScores;
class WindowExit;
class WindowGuns;
class InterfaceBaseManager;
class InterfaceManager;
class RenderManager;
class SpatialManager;
class PhysicsManager;


class WorldMap : public Singleton<WorldMap>
{

public:

    WorldMap(const std::string& mapp);
    ~WorldMap();

    void SetPlayers(int amount, ...);

    void onReceiveShot(unsigned int shooted, unsigned int owner, float damage);
//    void onBonusFall();
//    void onTouchBonus(Bot* xbot, Bonus* xbonus);

    std::string REST_TIME;
    bool CHOICE_GUN;
    bool SHOW_STATS;
    TVector2D OLD_POS;
    unsigned int MY_CURRENT_POS;
    int DISTANCE_SCORE;
    std::vector<std::vector<int> > spawnpoint;


    int currentFPS;

    bool SHOW_SCORES;

//    float _180overpi;
    unsigned int MY_BOT_NR;

    sf::Key::Code KEY_PRESSED; // = int
    bool YOU_KILLED;

    float getCurrentTime;

   // float fTimeStep; // Krok czasowy fizyki

    std::vector<Bot *> bot;
    void onMouseButtonDown(sf::Mouse::Button& butt);
    void onMouseButtonUp(sf::Mouse::Button& butt);

// --------------------------------------   loading interface  ------------------------------------------
    InterfaceBaseManager *interfaceBaseMgr;// = new InterfaceBaseManager();
    InterfaceManager *interfaceMgr;// = new InterfaceManager();
    RenderManager *renderMgr;
    SpatialManager *spatialMgr;
// ------------------------------------------------------------------------------------------------------

    //PhysicsManager* physicsMgr;

    Map *map;
    MapFront *mapFront;
    MapBack *mapBack;
    Mouse *mouse;
    Arrow* arrow;
    Background *backg;
    WindowExit *window_exit;
    WindowScores *window_scores;
    WindowGuns *window_guns;

	TextOutput *text_output;
    CommandLine *command_line;
	ChatLine *chat_line;

    bool CHOICE_EXIT;
//    void command_line();
//    void mychat_line();
    void setGL();
    unsigned int addBot(const BotsBase& bots, int spawn_nr, TEAM team);
    //void gunReloading(unsigned int bot_nr);
    void ThrowFive(unsigned int bot_nr, const TVector2D& src);
    PhysicsManager *physicsMgr;


//    JetBar jetBar;

private:

    WorldMap& operator=(const WorldMap&) {return *this;}
    void run();

    int nearestWaypoint(int spawn_nr);

    bool FRIENDLY_FIRE;

    void addMessageToChat(const std::string& part1, const std::string& part2);
    void addMessageToChat(const std::string& part);
    void displayChat();



    float getStartGameTime;//, getTimePassed;

    int takeScreenshot();
//  void hurt_bot(unsigned int shooted, unsigned int owner, float damage);
    void collisions();
    void game_control();


    void onKeyDown(sf::Key::Code sym);
    void onKeyUp(sf::Key::Code sym);

    void inputUser();
    int prev_time;

    int MAX_RESPAWN_TIME;


    void bots_control();

    void load_spawnpoints();

    bool do_bots_see(unsigned int first, unsigned int second);

};


#define world WorldMap::GetSingleton()



#endif
