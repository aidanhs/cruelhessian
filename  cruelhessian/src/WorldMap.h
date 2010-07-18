/*   worldmap.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Paweł Konieczny <konp84@gmail.com>
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

#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <iostream>
#include <vector>
#include <list>

#include "Globals.h"
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



class WorldMap : public Singleton<WorldMap>
{

public:

    WorldMap(const std::string& map, int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt);
    WorldMap(const std::string& map, int bots_cnt);
    ~WorldMap();

    void printText(freetype::font_data& font, const std::string& text, unsigned int* color, float x, float y);
    void printTextMiddle(freetype::font_data& font, const std::string& text, unsigned int* color, float y);

    float sGravity; // Przyspieszenie ziemskie
    float sDrag; // Wspólczynnik oporu
    float sDragWalking; // Wspólczynnik oporu podczas chodzenia
    float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
    float sWalking; // Szybkosc chodzenia
    float sFlying; // Szybkosc latania
    float sJumping; // Sila skoku
    float fTimeStep; // Krok czasowy fizyki

    freetype::font_data font[2][50];

    float _180overpi;
    Mix_Chunk *grenade_throw, *grenade_bounce, *grenade_explosion, *grenade_pullout, *sound_new_life, *sound_heaven, *sound_death[3], *sound_kitfall[2], *sound_spawn, *menu_click;

    unsigned int textColorGunOnTouch[4];
    unsigned int textColorGunNormal[4];
    unsigned int textCol[5][4];
    unsigned int textGunColor[4];



    Uint32 getCurrentTime;

//    float SOL_WIDTH[50], SOL_HEIGHT[50];
    float fullLife;


private:

    void run();

    Map *map;
    Mouse *mouse;
    Arrow* arrow;
    Background *backg;
    Chat *chat;
    WindowExit *window_exit;
    WindowScores *window_scores;
    WindowGuns *window_guns;
    Console cons, myChat;
    std::vector<Bot *> bot;
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
    Uint8 textColor[4];

    TVector2D OLD_POS;
    bool FRIENDLY_FIRE;

    void addMessageToChat(const std::string& part1, const std::string& part2);
    void addMessageToChat(const std::string& part);
    void displayChat();


    SDLKey KEY_PRESSED; // = int
    bool YOU_KILLED;
    Uint32 getStartGameTime;//, getTimePassed;
    void command_line();
    void mychat_line();
//    Mix_Chunk* loadSoundFile(const std::string& file);
//    Mix_Chunk* loadSoundFile2(const std::string& src_dir, const std::string& file);
    int takeScreenshot();
    void hurt_bot(unsigned int shooted, unsigned int owner, float damage);
    void collisions();
    void game_control();

//    std::string COMMAND_LINE_TEXT;
    std::vector<std::vector<int> > spawnpoint;
    std::vector<std::vector<int> > bonus;

    //float sUp;
    //float sBulletTime; //Zwolnienie czasu
    int currentFPS;
    bool SHOW_STATS;
    bool SHOW_SCORES;

    std::string Font[2];





    //OGLFT::Monochrome *font1_16, *font2_12, *font2_28;
    //std::string findInterface(const char* name);
//    std::string findInterface(const std::string& name);


    //void printText(OGLFT::Monochrome* font, const std::string& text, Uint8* color, float x, float y);
    //void printText(OGLFT::Monochrome* font, const std::string& text, unsigned int* color, float x, float y);
//    void printTextMovable(freetype::font_data& font, const std::string& text, Uint8* color, float x, float y);
    Uint8* keys;

    // std::vector<Constraints> constr[5];//, bconstr;

    void addBullet(unsigned int bot_nr, const TVector2D& dest);
    void addGrenade(unsigned int bot_nr, const TVector2D& dest, Uint32 push_time);
    unsigned int addBot(const BotsBase& bots, int spawn_nr, TEAM team);
//    void getGLpos();

    Mix_Music *music;

    void moveBotLeft(unsigned int bot_nr);
    void moveBotRight(unsigned int bot_nr);
    void moveBotUp(unsigned int bot_nr);
    void moveBotDown(unsigned int bot_nr);
    void moveBotJet(unsigned int bot_nr);
    void moveBotJumpLeft(unsigned int bot_nr);
    void moveBotJumpRight(unsigned int bot_nr);

    int playMusic(int pos);

    void inputUser();
    Uint32 prev_time;

//    bool ONLY_ONCE;
    bool CHOICE_EXIT;

    int MAX_RESPAWN_TIME;
    float JET_CHANGE;
    bool CHOICE_GUN;
    bool SHOW_GUN_MENU;
    bool SHOW_COMMAND_LINE;
    bool SHOW_MYCHAT_LINE;
    bool SHOW_ESC;




    Tex text_grenade[17];
    Tex text_bonus[7];


    unsigned int MY_BOT_NR;

    int read_po(OBJECT_TYPE type);
    //void make_lines(MT move, int frame, int kierunek, TEAM team);

    void bots_control();

    // definitions in draw.cpp
    void draw_background();
    void draw_interface();
//    void draw_int_help(const Tex& tex, float dx, float dy);
    //  void draw_screen();
    void draw_infos();


    void insertMe(TEAM team);

    // definitions in load.cpp
    void load_textures();
//    void load_animations();
//    void load_weapons_base();
    void load_configs();
    int load_map(const std::string& name);
    int load_audio();
    void load_spawnpoints();
    void load_bonuses();
    int load_fonts();
    int load_bots_base();
    int load_bots(unsigned int bots_count, TEAM team);
//    int getWeaponNumber(const std::string& gun);
    //Uint8* getRGB(const std::string& col);
    //public:
//    unsigned int* getRGB(const std::string& col);
    //private:
    void init_gl();

    bool do_bots_see(unsigned int first, unsigned int second);

//    const std::string anim_type(MT name) const;
//    int read_poa(const MT name);

    //void load_constraints();
    void gunReloading(unsigned int bot_nr);

    unsigned int MY_CURRENT_POS;
    int DISTANCE_SCORE;

    Tex text_arrow;
    Tex text_mouse;
    Tex text_health;
    Tex text_healthbar;
    Tex text_font;
    Tex text_nade;
    Tex text_ammo;
    Tex text_reloadbar;
    Tex text_jet;
    Tex text_jetbar;
    Tex text_firebar;
    Tex text_firebar_r;
    Tex text_weath;
    Tex text_deaddot;
    Tex text_smalldot;
};


#define world WorldMap::GetSingleton()



#endif