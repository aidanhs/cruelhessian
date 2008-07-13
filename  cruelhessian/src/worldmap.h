/*   worldmap.h
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

#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "main.h"
#include <iostream>
#include "SDL_opengl.h"


extern float SOL_WIDTH[50], SOL_HEIGHT[50];
extern float AMMO_WIDTH[20], AMMO_HEIGHT[20];

class WorldMap
{
public:
    //Bot *bot;

    float TimeStepSize; // Krok czasowy fizyki
    float sGravity; // Przyspieszenie ziemskie
    float sDrag; // Wspólczynnik oporu
    float sDragWalking; // Wspólczynnik oporu podczas chodzenia
    float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
    //static float sElasticity = 0.05; // Elastycznosc odbicia
    //float sFriction = 1.5; // Tarcie
    //float sFriction = 1; // Tarcie

    float sWalking; // Szybkosc chodzenia
    float sFlying; // Szybkosc latania
    float sJumping; // Sila skoku
    float sBulletTime; //Zwolnienie czasu
    float dDeltaTime;
    std::string MESSAGE[700];
    int MAX_RESPAWN_TIME;
    float REL_TIME;
    float GUN_MENU_START_X;
    float GUN_MENU_START_Y;
    bool CHOICE_GUN;
    bool CHOICE_GUN_MENU;
    bool CHOICE_EXIT;
    bool CHOICE_ESC;



    GLuint texture_temp;

    GLuint *text_scen;
    GLfloat **text_scen_xy;

    GLuint text_poly;
    GLfloat text_poly_xy[4];

    GLuint text_mouse;
    GLfloat text_mouse_xy[4];

    GLuint text_health;
    GLfloat text_health_xy[4];

    GLuint text_healthbar;
    GLfloat text_healthbar_xy[4];

    GLuint text_font;
    GLfloat text_font_xy[4];

    GLuint text_nade;
    GLfloat text_nade_xy[4];

    GLuint text_ammo;
    GLfloat text_ammo_xy[4];

    GLuint text_reloadbar;
    GLfloat text_reloadbar_xy[4];

    GLuint text_jet;
    GLfloat text_jet_xy[4];

    GLuint text_jetbar;
    GLfloat text_jetbar_xy[4];

    GLuint text_weath;
    GLfloat text_weath_xy[4];



    GLuint text_ammos[11];//, text_ammo_1, text_ammo_2, text_ammo_3, text_ammo_4, text_ammo_5, text_ammo_6, text_ammo_7, text_ammo_8, text_ammo_9, text_ammo_10;
    GLfloat text_ammos_xy[11][4];//, text_ammo_1_xy[4], text_ammo_2_xy[4], text_ammo_3_xy[4], text_ammo_4_xy[4], text_ammo_5_xy[4], text_ammo_6_xy[4], text_ammo_7_xy[4], text_ammo_8_xy[4], text_ammo_9_xy[4], text_ammo_10_xy[4];

    GLuint text_grenade[16];
    GLfloat text_grenade_xy[16][4];

    GLuint gost_x;
    GLfloat gost_x_xy[4];

    GLuint text_0, text_1, text_2, text_3, text_4, text_5, text_6, text_7, text_8, text_9, text_10, text_knife, text_chain, text_law;
    GLfloat text_0_xy[4], text_1_xy[4], text_2_xy[4], text_3_xy[4], text_4_xy[4], text_5_xy[4], text_6_xy[4], text_7_xy[4], text_8_xy[4], text_9_xy[4], text_10_xy[4], text_knife_xy[4], text_chain_xy[4], text_law_xy[4];


    GLuint gost_stopa;
    GLfloat gost_stopa_xy[4];

    GLuint gost_klata;
    GLfloat gost_klata_xy[4];

    GLuint gost_ramie;
    GLfloat gost_ramie_xy[4];

    GLuint gost_morda;
    GLfloat gost_morda_xy[4];

    GLuint gost_reka;
    GLfloat gost_reka_xy[4];

    GLuint gost_dlon;
    GLfloat gost_dlon_xy[4];

    GLuint gost_udo;
    GLfloat gost_udo_xy[4];

    GLuint gost_biodro;
    GLfloat gost_biodro_xy[4];

    GLuint gost_noga;
    GLfloat gost_noga_xy[4];


    int bgX, bgY;//, sX, sY;
    int BOTS_NUM;

    float tr_maxx[10000];
    float tr_maxy[10000];
    float tr_minx[10000];
    float tr_miny[10000];
    float aa[10000][3];
    float con[10000][3];

    int isServer;
    int MY_BOT_NR;

    WorldMap();
    ~WorldMap();

    void calculate_physics(float aDeltaTime);
    void keys_control();
    void bots_control();
    void draw_background();
    void draw_arms();
    void draw_interface();
    void draw_screen();
    void draw_mouse();
    void draw_infos();
    void draw_gostek();
    void oblicz_min_max();
    void load_textures();
    void init_gl();
    void read_animations();
    //int read_map(const char *name);
    int read_map(std::string name);
    void draw_gostek_help(float dx, float dy, int part);
    void hurt_bot(int shooted, int owner);

    void add_bullet(int bot_nr, float dest_x, float dest_y);
    void add_grenade(int bot_nr, float dest_x, float dest_y, float power);
    bool do_bots_see(int first, int second);
    void game_loop();


};

extern WorldMap *world;


#endif
