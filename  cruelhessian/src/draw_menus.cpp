/*   draw_menus.cpp
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

#include <sstream>
#include <algorithm>
#include "worldmap.h"


void WorldMap::gun_menu()
{
    float startx = GUN_MENU_START_X;
    float starty = GUN_MENU_START_Y;
    float delta = 20;
    bool stan1 = false;
    std::ostringstream oss;

    if ((NMOUSE_X > startx+40) || (NMOUSE_X < startx+200))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(bgX, bgY, 0.0f);

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

    printText(font2_12, "Primary Weapon:", textGunColor, startx, starty);

    startx += 85;
    for (int i = 1; i <= 10; i++)
    {
        starty += delta;
        draw_int_help(weapon[i].textureGun, startx-85, starty);
        oss << i%10;
        oss << " " + weapon[i].name;
        printText(font2_12, oss.str(),
                  (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7) ? textColorGunOnTouch : textColorGunNormal,
                  startx, starty);
        oss.str("");
    }

    starty += 30;
    printText(font2_12, "Secondary Weapon:", textGunColor, startx-85, starty);

    starty += delta;
    draw_int_help(weapon[0].textureGun, startx-85, starty);
    printText(font2_12, weapon[0].name,
              (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7) ? textColorGunOnTouch : textColorGunNormal,
              startx, starty);

    starty += delta;
    draw_int_help(weapon[14].textureGun, startx-85, starty);
    printText(font2_12, weapon[14].name,
              (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7) ? textColorGunOnTouch : textColorGunNormal,
              startx, starty);

    starty += delta;
    draw_int_help(weapon[15].textureGun, startx-85, starty);
    printText(font2_12, weapon[15].name,
              (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7) ? textColorGunOnTouch : textColorGunNormal,
              startx, starty);

    starty += delta;
    draw_int_help(weapon[16].textureGun, startx-85, starty);
    printText(font2_12, weapon[16].name,
              (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7) ? textColorGunOnTouch : textColorGunNormal,
              startx, starty);

    glDisable(GL_TEXTURE_2D);
}


void WorldMap::exit_menu()
{
    float startx = GUN_MENU_START_X;
    float starty = GUN_MENU_START_Y;
    bool stan1 = false;


    if ((NMOUSE_X > startx-10) && (NMOUSE_X < startx+140))
        stan1 = true;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(bgX, bgY, 0.0f);

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

    starty += 20;
    printText(font1_16, "Exit",
              (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7) ? textColorGunOnTouch : textColorGunNormal,
              startx, starty);


    /* starty += 20;
     if (stan1 && NMOUSE_Y > starty-7 && NMOUSE_Y < starty+7)
     {
         PrintText(font2_12, "Kick player", textColorGunOnTouch, startx, starty);
     }
     else
     {
         PrintText(font2_12, "Kick player", textColorGunNormal, startx, starty);
     }*/

    //glDisable(GL_TEXTURE_2D);
}



void WorldMap::gun_menu_select()
{
    int x = NMOUSE_X;
    int y = NMOUSE_Y;
    float delta = 20;
    float startya = GUN_MENU_START_Y+delta-7;
    float startyb = GUN_MENU_START_Y+delta+7;

    if (SOUNDS_VOL > 0)
        Mix_PlayChannel(-1, menu_click, 0);

    // if pressed key
    if (KEY_PRESSED >= SDLK_0 && KEY_PRESSED <= SDLK_9)
    {
        bot[MY_BOT_NR]->gunModel = KEY_PRESSED-48;
        goto ok;
    }

    // if pressed mouse button
    if ((x >= GUN_MENU_START_X+10 && x <= GUN_MENU_START_X+250))
    {
        if ((y >= startya && y <= startyb))
        {
            bot[MY_BOT_NR]->gunModel = 1;
            goto ok;
        }
        else if ((y >= startya+delta && y <= startyb+delta))
        {
            bot[MY_BOT_NR]->gunModel = 2;
            goto ok;
        }
        else if ((y >= startya+2*delta && y <= startyb+2*delta))
        {
            bot[MY_BOT_NR]->gunModel = 3;
            goto ok;
        }
        else if ((y >= startya+3*delta && y <= startyb+3*delta))
        {
            bot[MY_BOT_NR]->gunModel = 4;
            goto ok;
        }
        else if ((y >= startya+4*delta && y <= startyb+4*delta))
        {
            bot[MY_BOT_NR]->gunModel = 5;
            goto ok;
        }
        else if ((y >= startya+5*delta && y <= startyb+5*delta))
        {
            bot[MY_BOT_NR]->gunModel = 6;
            goto ok;
        }
        else if ((y >= startya+6*delta && y <= startyb+6*delta))
        {
            bot[MY_BOT_NR]->gunModel = 7;
            goto ok;
        }
        else if ((y >= startya+7*delta && y <= startyb+7*delta))
        {
            bot[MY_BOT_NR]->gunModel = 8;
            goto ok;
        }
        else if ((y >= startya+8*delta && y <= startyb+8*delta))
        {
            bot[MY_BOT_NR]->gunModel = 9;
            goto ok;
        }
        else if ((y >= startya+9*delta && y <= startyb+9*delta))
        {
            bot[MY_BOT_NR]->gunModel = 10;
            goto ok;
        }
        else if ((y >= startya+10*delta+30 && y <= startyb+10*delta+30))
        {
            bot[MY_BOT_NR]->gunModel = 10;
            goto ok;
        }
    }
    return;

ok:
    CHOICE_GUN = true;
    SHOW_GUN_MENU = false;
    bot[MY_BOT_NR]->leftAmmos = weapon[bot[MY_BOT_NR]->gunModel].ammo;
}



void WorldMap::exit_menu_select()
{
    int x = NMOUSE_X;
    int y = NMOUSE_Y;
    float delta = 20;
    float startya = GUN_MENU_START_Y+delta-7;
    float startyb = GUN_MENU_START_Y+delta+7;

    if ((x >= GUN_MENU_START_X+10) && (x <= GUN_MENU_START_X+250))
    {
        if ((y >= startya) && (y <= startyb))
        {
            CHOICE_EXIT = true;
        }
        else if ((y >= startya+delta) && (y <= startyb+delta))
        {
            //bot[MY_BOT_NR]->gunModel = 2;
            //CHOICE_GUN = 0;
        }
    }
}

bool cmp(const Bot& bot1, const Bot& bot2);

bool cmp(const Bot& bot1, const Bot& bot2)
{
    return (bot1.points > bot2.points);
}

void WorldMap::scores_menu()
{

    float offset = 20;
    std::ostringstream oss;

    // distance between columns
    std::vector<int> off(column_names.size());
    float xx = MAX_WIDTH / column_names.size();
    for (unsigned int i = 0; i < column_names.size(); ++i)
        off[i] = xx*(0.5 + i) - 50;

    // sort bots
    std::vector<Bot> deaths;
    for (unsigned int i = 0; i < bot.size(); i++)
    {
        deaths.push_back(*bot[i]);
    }
    std::sort(deaths.begin(), deaths.end(), cmp);

    // draw
    float list_long = static_cast<float>(bot.size()*15+80);

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(bgX, bgY, 0.0f);

    glColor4ub(247, 247, 247, 20);
    glRectf(offset-1, offset-1, MAX_WIDTH-offset+1, list_long-1);

    glColor4ub(0, 80, 140, 25);
    glBegin(GL_QUADS);
    glVertex2f(offset, offset);
    glVertex2f(MAX_WIDTH-offset, offset);
    glVertex2f(MAX_WIDTH-offset, list_long);
    glVertex2f(offset, list_long);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

    for (unsigned int i = 0; i < column_names.size(); ++i)
        printText(font2_12, column_names[i], textGunColor, off[i], offset+10);

    offset += 40;

    for (unsigned int j = 0; j < deaths.size(); ++j)
    {
        printText(font2_12, deaths[j].name, textCol[deaths[j].team], off[0], offset+j*15);
        oss << deaths[j].points;
        printText(font2_12, oss.str(), textCol[deaths[j].team], off[1], offset+j*15);
        oss.str("");
        oss << deaths[j].deaths;
        printText(font2_12, oss.str(), textCol[deaths[j].team], off[2], offset+j*15);
        oss.str("");
    }

    glDisable(GL_TEXTURE_2D);
}
