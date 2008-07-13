/*   gun.cpp
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

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "draw_extra.h"
#include "globals.h"
#include "main.h"
#include "bot.h"
#include "worldmap.h"
#include "gun.h"

#define GUN_MENU_NORMAL_COLOR glColor4ub(200, 200, 200, 250);
#define GUN_MENU_ON_TOUCH_COLOR glColor4ub(255, 255, 255, 255);



const char *gun_name(int model)
{
    switch (model)
    {
    case 1:
        return "Dessert Eagles";
    case 2:
        return "HK MP5";
    case 3:
        return "Ak-74";
    case 4:
        return "Steyr AUG";
    case 5:
        return "Spas-12";
    case 6:
        return "Ruger 77";
    case 7:
        return "M79";
    case 8:
        return "Barret M82A1";
    case 9:
        return "FN Minimi";
    case 0:
        return "XM214 Minigun";
    case 10:
        return "USSCOM";
    case 11:
        return "Combat Knife";
    case 12:
        return "Chainsaw";
    case 13:
        return "M72 LAW";
    }
    return "error";
}

const char *gun_reload(int model)
{
    switch (model)
    {
    case 1:
        return "Sfx/deserteagle-reload.wav";
    case 2:
        return "Sfx/mp5-reload.wav";
    case 3:
        return "Sfx/ak74-reload.wav";
    case 4:
        return "Sfx/steyraug-reload.wav";
    case 5:
        return "Sfx/spas12-reload.wav";
    case 6:
        return "Sfx/ruger77-reload.wav";
    case 7:
        return "Sfx/m79-reload.wav";
    case 8:
        return "Sfx/barretm82-reload.wav";
    case 9:
        return "";
    case 0:
        return "Sfx/minigun-reload.wav";
    case 10:
        return "";
    }
    return "error";
}

Uint32 gun_reload_time(int model)
{
    switch (model)
    {
    case 1:
        return 103*world->REL_TIME;
    case 2:
        return 95*world->REL_TIME;
    case 3:
        return 150*world->REL_TIME;
    case 4:
        return 115*world->REL_TIME;
    case 5:
        return 175*world->REL_TIME;
    case 6:
        return 100*world->REL_TIME;
    case 7:
        return 180*world->REL_TIME;
    case 8:
        return 300*world->REL_TIME;
    case 9:
        return 250*world->REL_TIME;
    case 0:
        return 366*world->REL_TIME;
    case 10:
        return 65*world->REL_TIME;
    }
    return 0;
}

Uint32 gun_interval_time(int model)
{
    switch (model)
    {
    case 1:
        return 23*world->REL_TIME;
    case 2:
        return 6*world->REL_TIME;
    case 3:
        return 11*world->REL_TIME;
    case 4:
        return 7*world->REL_TIME;
    case 5:
        return 34*world->REL_TIME;
    case 6:
        return 40*world->REL_TIME;
    case 7:
        return 6*world->REL_TIME;
    case 8:
        return 240*world->REL_TIME;
    case 9:
        return 9*world->REL_TIME;
    case 0:
        return 4*world->REL_TIME;
    case 10:
        return 12*world->REL_TIME;
    }
    return 0;
}

float gun_speed(int model)
{
    switch (model)
    {
    case 1:
        return 190;
    case 2:
        return 6*world->REL_TIME;
    case 3:
        return 11*world->REL_TIME;
    case 4:
        return 7*world->REL_TIME;
    case 5:
        return 137;
    case 6:
        return 330;
    case 7:
        return 110;
    case 8:
        return 550;
    case 9:
        return 9*world->REL_TIME;
    case 0:
        return 290;
    case 10:
        return 180;
    }
    return 0;
}

float gun_mass(int model)
{
    switch (model)
    {
    case 1:
        return 1000;
    case 2:
        return 1000;
    case 3:
        return 1000;
    case 4:
        return 1000;
    case 5:
        return 1000;
    case 6:
        return 1000;
    case 7:
        return 1000;
    case 8:
        return 1000;
    case 9:
        return 1000;
    case 0:
        return 1000;
    case 10:
        return 1000;
    }
    return 0;
}



const char *gun_fire(int model)
{
    switch (model)
    {
    case 1:
        return "Sfx/deserteagle-fire.wav";
    case 2:
        return "Sfx/mp5-fire.wav";
    case 3:
        return "Sfx/ak74-fire.wav";
    case 4:
        return "Sfx/steyraug-fire.wav";
    case 5:
        return "Sfx/spas12-fire.wav";
    case 6:
        return "Sfx/ruger77-fire.wav";
    case 7:
        return "Sfx/m79-fire.wav";
    case 8:
        return "Sfx/barretm82-fire.wav";
    case 9:
        return "";
    case 0:
        return "Sfx/minigun-fire.wav";
    case 10:
        return "";
    }
    return "error";
}


int gun_ammos(int model)
{
    switch (model)
    {
    case 1:
        return 7;
    case 2:
        return 30;
    case 3:
        return 40;
    case 4:
        return 30;
    case 5:
        return 7;
    case 6:
        return 4;
    case 7:
        return 1;
    case 8:
        return 10;
    case 9:
        return 50;
    case 0:
        return 100;
    case 10:
        return 12;
    }
    return 0;
}



void gun_menu_select(int x, int y)
{
    float delta = 20;
    float startya = world->GUN_MENU_START_Y+delta-7;
    float startyb = world->GUN_MENU_START_Y+delta+7;

    if ((x >= world->GUN_MENU_START_X+10) && (x <= world->GUN_MENU_START_X+250))
    {
        if ((y >= startya) && (y <= startyb))
        {
            bot[world->MY_BOT_NR]->gunModel = 1;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+delta) && (y <= startyb+delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 2;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+2*delta) && (y <= startyb+2*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 3;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+3*delta) && (y <= startyb+3*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 4;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+4*delta) && (y <= startyb+4*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 5;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+5*delta) && (y <= startyb+5*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 6;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+6*delta) && (y <= startyb+6*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 7;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+7*delta) && (y <= startyb+7*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 8;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+8*delta) && (y <= startyb+8*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 9;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+9*delta) && (y <= startyb+9*delta))
        {
            bot[world->MY_BOT_NR]->gunModel = 0;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
        else if ((y >= startya+10*delta+30) && (y <= startyb+10*delta+30))
        {
            bot[world->MY_BOT_NR]->gunModel = 10;
            world->CHOICE_GUN = true;
            world->CHOICE_GUN_MENU = false;
        }
    }
    bot[world->MY_BOT_NR]->leftAmmos = gun_ammos(bot[world->MY_BOT_NR]->gunModel);
}



void gun_menu()
{
    float startx = world->GUN_MENU_START_X;
    float starty = world->GUN_MENU_START_Y;
    int mx, my;
    float delta = 20;
    bool stan1 = false;
    GLuint text_font;
    GLfloat text_font_xy[4];

    SDL_GetMouseState(&mx, &my);

    if ((mx > startx+40) && (mx < startx+200)) stan1 = true;


    glPushMatrix();
    glLoadIdentity();
    glTranslatef(world->bgX, world->bgY, 0.0f);

    glColor4ub(247, 247, 247, 50);
    glRectf(startx-10, starty-10, startx+250, starty+350);

    glColor4ub(0, 80, 140, 25);
    glBegin(GL_QUADS);
    glVertex2f(startx-11, starty-11);
    glVertex2f(startx+251, starty-11);
    glVertex2f(startx+251, starty+351);
    glVertex2f(startx-11, starty+351);
    glEnd();
    glPopMatrix();


    glEnable(GL_TEXTURE_2D);

    glColor4ub(247, 247, 247, 250);

    text_font = SDL_GL_LoadFont(font2_12, "Primary Weapon", textGunColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_1, world->text_1_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           1 Desert Eagles", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_2, world->text_2_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           2 HK MP5", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_3, world->text_3_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           3 Ak-74", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_4, world->text_4_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           4 Steyr AUG", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_5, world->text_5_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           5 Spas-12", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7&& my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_6, world->text_6_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           6 Ruger 77", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_7, world->text_7_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           7 M79", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_8, world->text_8_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           8 Barret M82A1", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_9, world->text_9_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           9 FN Minimi", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_0, world->text_0_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           0 XM214 Minigun", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += 30;
    glColor4ub(247, 247, 247, 250);
    text_font = SDL_GL_LoadFont(font2_12, "Secondary Weapon:", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_10, world->text_10_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           USSOCOM", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_knife, world->text_knife_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           Combat Knife", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_chain, world->text_chain_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           Chainsaw", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += delta;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_law, world->text_law_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font2_12, "           M72 LAW", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    glDisable(GL_TEXTURE_2D);
}

// to jest tylko raz wywolywane !!!!!!!!
int reload_gun(void *data)
{

    SDL_Delay(gun_reload_time(bot[world->MY_BOT_NR]->gunModel));
//    if (SOUNDS_VOL > 0) Mix_PlayChannel(-1, Mix_LoadWAV(gun_reload(GUN_MODEL)), 0);
    bot[world->MY_BOT_NR]->leftAmmos = gun_ammos(bot[world->MY_BOT_NR]->gunModel);
    bot[world->MY_BOT_NR]->isReloading = 0;
    return 0;
}


void exit_menu()
{
    float startx = world->GUN_MENU_START_X;
    float starty = world->GUN_MENU_START_Y;
    int mx, my;
    bool stan1 = false;
    GLuint text_font;
    GLfloat text_font_xy[4];

    SDL_GetMouseState(&mx, &my);

    if ((mx > startx-10) && (mx < startx+140)) stan1 = true;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(world->bgX, world->bgY, 0.0f);

    glColor4ub(0, 10, 200, 50);
    glRectf(startx, starty, startx+150, starty+70);

    glColor4ub(0, 0, 210, 25);
    glBegin(GL_QUADS);
    glVertex2f(startx-1, starty-1);
    glVertex2f(startx+151, starty-1);
    glVertex2f(startx+151, starty+71);
    glVertex2f(startx-1, starty+71);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    starty += 20;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_knife, world->text_knife_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font1_16, "Exit", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    starty += 20;
    if (stan1 && my > starty-7 && my < starty+7)
        GUN_MENU_ON_TOUCH_COLOR
    else
        GUN_MENU_NORMAL_COLOR;
    draw_int_help(world->text_knife, world->text_knife_xy, startx, starty);
    text_font = SDL_GL_LoadFont(font1_16, "Kick player", textColor, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &text_font);

    glDisable(GL_TEXTURE_2D);
}

void exit_menu_select(int x, int y)
{
    float delta = 20;
    float startya = world->GUN_MENU_START_Y+delta-7;
    float startyb = world->GUN_MENU_START_Y+delta+7;

    if ((x >= world->GUN_MENU_START_X+10) && (x <= world->GUN_MENU_START_X+250))
    {
        if ((y >= startya) && (y <= startyb))
        {
            world->CHOICE_EXIT = true;
            GAME_LOOP = false;
            free_init_all2();
        }
        else if ((y >= startya+delta) && (y <= startyb+delta))
        {
            //bot[MY_BOT_NR]->gunModel = 2;
            //CHOICE_GUN = 0;
        }
    }
}
