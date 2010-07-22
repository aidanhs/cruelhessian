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

//#include "Globals.h"
#include "Arrow.h"
#include "Background.h"
#include "Bonus.h"
#include "Bot.h"
#include "Bullet.h"
#include "Grenade.h"
#include "MovingObject.h"
#include "Map.h"
#include "Console.h"
#include "Mouse.h"
#include "Chat.h"
#include "WindowScores.h"
#include "WindowExit.h"
#include "WindowGuns.h"
#include "Singleton.h"
#include "BotManager.h"
#include "InterfaceBaseManager.h"
#include "InterfaceManager.h"
#include "Tex.h"



class WorldMap : public Singleton<WorldMap>
{

public:

    WorldMap(const std::string& map, int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt);
    WorldMap(const std::string& map, int bots_cnt);
    virtual ~WorldMap();

    float sGravity; // Przyspieszenie ziemskie
    float sDrag; // Wspólczynnik oporu
    float sDragWalking; // Wspólczynnik oporu podczas chodzenia
    float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
    float sWalking; // Szybkosc chodzenia
    float sFlying; // Szybkosc latania
    float sJumping; // Sila skoku
    float fTimeStep; // Krok czasowy fizyki

    bool CHOICE_GUN;
    bool SHOW_GUN_MENU;
    bool SHOW_COMMAND_LINE;
    bool SHOW_MYCHAT_LINE;
    bool SHOW_ESC;
    bool SHOW_STATS;
    TVector2D OLD_POS;
    unsigned int MY_CURRENT_POS;
    int DISTANCE_SCORE;
    std::vector<std::vector<int> > spawnpoint;

    int currentFPS;

    bool SHOW_SCORES;

//    float _180overpi;
    unsigned int MY_BOT_NR;

    SDLKey KEY_PRESSED; // = int
    bool YOU_KILLED;

    Uint32 getCurrentTime;

    float fullLife;

    Chat *chat;
    std::vector<Bot *> bot;

// --------------------------------------   loading interface  ------------------------------------------
    InterfaceBaseManager *interfaceBaseMgr;// = new InterfaceBaseManager();
    InterfaceManager *interfaceMgr;// = new InterfaceManager();
// ------------------------------------------------------------------------------------------------------

    Tex text_weath;
    Tex text_grenade[17];


    Map *map;
    Mouse *mouse;
    Arrow* arrow;
    Background *backg;
    WindowExit *window_exit;
    WindowScores *window_scores;
    WindowGuns *window_guns;
    Console cons, myChat;
    bool CHOICE_EXIT;
    void command_line();
    void mychat_line();

private:

    void run();
    GLuint *text_scen;


    std::list<Bullet *> bullet_list;
    std::list<Bonus *> bonus_list;
    std::list<Grenade *> gren_list;
    int nearestWaypoint(int spawn_nr);
    bool collisionCircle2Wall(const MovingObject& ob, float dx, float dy, int& triangle, int& line);
    int collisionPoint2Wall(const MovingObject& ob, float dx, float dy);
    int collisionPoint2Circle(const MovingObject& ob1, const MovingObject& ob2);
    int collisionCircle2Circle(const MovingObject& ob1, const MovingObject& ob2);


//    float distance(const TVector2D &point, int triangle, int ver1, int ver2);
//    int nearestLine(const TVector2D &point, int triangle);



    bool FRIENDLY_FIRE;

    void addMessageToChat(const std::string& part1, const std::string& part2);
    void addMessageToChat(const std::string& part);
    void displayChat();



    Uint32 getStartGameTime;//, getTimePassed;

    int takeScreenshot();
    void hurt_bot(unsigned int shooted, unsigned int owner, float damage);
    void collisions();
    void game_control();


    std::vector<std::vector<int> > bonus;



    Uint8* keys;

    void addBullet(unsigned int bot_nr, const TVector2D& dest);
    void addGrenade(unsigned int bot_nr, const TVector2D& dest, Uint32 push_time);
    unsigned int addBot(const BotsBase& bots, int spawn_nr, TEAM team);

    void moveBotLeft(unsigned int bot_nr);
    void moveBotRight(unsigned int bot_nr);
    void moveBotUp(unsigned int bot_nr);
    void moveBotDown(unsigned int bot_nr);
    void moveBotJet(unsigned int bot_nr);
    void moveBotJumpLeft(unsigned int bot_nr);
    void moveBotJumpRight(unsigned int bot_nr);

    void inputUser();
    Uint32 prev_time;



    int MAX_RESPAWN_TIME;
    float JET_CHANGE;




    int read_po(OBJECT_TYPE type);

    void bots_control();

    void draw_background();
    //void draw_interface();
    void draw_infos();

    void insertMe(TEAM team);

    void load_textures();
    void load_configs();
    int load_map(const std::string& name);
    void load_spawnpoints();
    void load_bonuses();
    int load_bots(unsigned int bots_count, TEAM team);

    void init_gl();

    bool do_bots_see(unsigned int first, unsigned int second);

    void gunReloading(unsigned int bot_nr);





};


#define world WorldMap::GetSingleton()



#endif
