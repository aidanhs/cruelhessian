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
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include "SDL_mixer.h"

#include "globals.h"
#include "bot.h"
//#include "gun.h"
#include "fonts.h"
#include "moving_object.h"


class Weapon
{
public:
    //Weapon() {};
    std::string name;
    Mix_Chunk *fireSound;
    Mix_Chunk *reloadSound;
    int damage;
    Uint32 fireInterval;
    int ammo;
    Uint32 reloadTime;
    int speed;
    int bulletStyle;
    float startUpTime;
    float bink;
    int movementAcc;
    int recoil;
    Tex textureAmmo;
    Tex textureGun;
};

class Bots
{
    public:
    std::string name;
    std::string chatKill;
    std::string chatDead;
    std::string chatLowhealth;
    std::string chatSeeEnemy;
    std::string chatWinning;
};


extern Weapon weapon[20];


class WorldMap
{

public:

    WorldMap(const std::string& map, int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt);
    ~WorldMap();
    void run();

private:


    void chat_window();
    Uint32 lastOutChatEntity;
    typedef std::pair<std::string, std::string> string_pair;
    std::list<string_pair> chat_list;
    SDLKey KEY_PRESSED; // = int
    bool YOU_KILLED;
    Uint32 getCurrentTime;
    std::vector<std::string> column_names;
    void scores_menu();
    Mix_Chunk* loadSoundFile(const std::string& file);
    int takeScreenshot();
    void hurt_bot(unsigned int shooted, unsigned int owner);
    void calculate_physics();
    void load_spawnpoints();

    std::vector<std::vector<int> > spawnpoint;
//cpFloat dt;
//cpBody *staticBodyMap;
    float sGravity; // Przyspieszenie ziemskie
    float sDrag; // Wspólczynnik oporu
    float sDragWalking; // Wspólczynnik oporu podczas chodzenia
    float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
    //static float sElasticity = 0.05; // Elastycznosc odbicia
    //float sFriction = 1.5; // Tarcie
    //float sFriction = 1; // Tarcie

    float sWalking; // Szybkosc chodzenia
    //float getsWalking(){return sWalking;};
    float sFlying; // Szybkosc latania
    float sJumping; // Sila skoku
    //float sUp;
    float sBulletTime; //Zwolnienie czasu
    int currentFPS;
    bool SHOW_STATS;
    bool SHOW_SCORES;

    std::string Font1;
    std::string Font2;
    int FontMenuSize;
    int FontConsoleSize;
    int FontBigSize;
    int FontWeaponMenuSize;
    int FontConsoleSmallSize;
    int FontHeightScale;
    int FontMenuBold;
    int FontConsoleBold;
    int FontBigBold;
    int FontWeaponMenuBold;
    int FontConsoleSmallBold;
    int KillConsoleNameSpace;
    int MOUSE_X;
    int NMOUSE_X;
    int MOUSE_Y;
    int NMOUSE_Y;

// roznica od ostatniego polozenia
    int REL_MOUSE_X;
    int REL_MOUSE_Y;

    int SOUNDS_VOL_INT;

    freetype::font_data font1_16, font2_12, font2_28;
    std::string findInterface(const char* name);
    void printText(freetype::font_data& font, const std::string& text, ubyte* color, float x, float y);
    void printTextMovable(freetype::font_data& font, const std::string& text, ubyte* color, float x, float y);
    Uint8* keys;

    // std::vector<Constraints> constr[5];//, bconstr;

    void addBullet(unsigned int bot_nr, float dest_x, float dest_y);
    void addGrenade(unsigned int bot_nr, float dest_x, float dest_y, Uint32 push_time);
    void addBot(const std::string& name, int spawn_nr, int gunmodel, TEAM team, unsigned int bot_nr);
    void addBot(const Bots& bots, int spawn_nr, TEAM team, unsigned int bot_nr);
    void getGLpos();

    GLuint SOIL_LoadTexture(const std::string& file);
    Tex SOIL_LoadTextureEx(const std::string& file);

    Mix_Music *music;

    void moveBotLeft(int bot_nr);
    void moveBotRight(int bot_nr);
    void moveBotUp(int bot_nr);
    void moveBotDown(int bot_nr);
    void moveBotJet(int bot_nr);
    void moveBotJumpLeft(int bot_nr);
    void moveBotJumpRight(int bot_nr);

    int playMusic(int pos);

    void inputUser();
    GLuint which_part(int nr);
    GLfloat* which_part_xy(int nr);

    //int reload_gun(void *data);

    float fTimeStep; // Krok czasowy fizyki
    float fTimeStepMS; // Krok czasowy fizyki ( ms )

//    TVector2D aGravity; // Przyspieszenie ziemskie
//    TVector2D aWalkingLeft;
//    TVector2D aWalkingRight;
//    TVector2D aJumpUp;

    int FRAMES_MAX[50], BODYPARTS_MAX[50];

    bool ONLY_ONCE;
    bool CHOICE_EXIT;

    std::string MESSAGE[700];
    int MAX_RESPAWN_TIME;
    float JET_CHANGE;
    //int REL_TIME;
    float GUN_MENU_START_X;
    float GUN_MENU_START_Y;
    //bool GAME_LOOP;
    bool CHOICE_GUN;
    bool SHOW_GUN_MENU;
    //bool CLEAN_WORLD;
    //bool CHOICE_EXIT;
    bool SHOW_ESC;

    void gun_menu();
    void gun_menu_select();
    void exit_menu();
    void exit_menu_select();

    GLuint *text_scen;

    GLuint text_poly;
    //GLfloat text_poly_xy[4];
    /*ubyte textColor[4];
    ubyte textGunColor[4];
    ubyte textYellow[4];
    ubyte textRed[4];
    */

    Mix_Chunk *grenade_throw, *grenade_explosion, *sound_new_life, *sound_heaven, *sound_death[3], *sound_spawn, *menu_click;

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
    Tex text_grenade[16];
    Tex gost_x;
    //Tex text_guns[14];
    Tex gost_stopa, gost_klata, gost_ramie, gost_morda, gost_reka, gost_dlon, gost_udo, gost_biodro, gost_noga;
    float bgX, bgY;
    unsigned int MY_BOT_NR;

    int read_po(OBJECT_TYPE type);
    void make_lines(MT move, int frame, int kierunek, TEAM team);

    void calc_min_max();

    void bots_control();

    // definitions in draw.cpp
    void draw_background();
    void draw_arms();
    void draw_interface();
    void draw_int_help(Tex& tex, float dx, float dy);
    void draw_screen();
    void draw_mouse();
    void draw_infos();
    void draw_gostek();
    void draw_gostek_help(float dx, float dy, int part);

    // definitions in load.cpp
    void load_textures();
    void load_animations();
    void load_weapons();
    void load_configs();
    int load_map(const std::string& name);
    int load_audio();
    int load_fonts();
    int load_bots(int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt);

    void init_gl();

    void hurtBot(unsigned int shooted, unsigned int owner);

    bool do_bots_see(int first, int second);

    std::string anim_type(MT name);
    int read_poa(MT name);

    //void load_constraints();
    void gunReloading(int bot_nr);

};


#endif
