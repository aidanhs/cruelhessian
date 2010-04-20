/*   interface.cpp
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
#include "tvector2d.h"
#include <sstream>



static void draw_int_help(const Tex& texture, float dx, float dy)
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(dx, dy, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture.tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2f(0.0, 0.0);
    glTexCoord2i(1, 1);
    glVertex2f(texture.w, 0.0);
    glTexCoord2i(1, 0);
    glVertex2f(texture.w, texture.h);
    glTexCoord2i(0, 0);
    glVertex2f(0.0, texture.h);
    glEnd();

    glPopMatrix();

}


void WorldMap::draw_interface()
{

    std::ostringstream oss;
    std::string temp;


    // ----- Gun's name -----

    printText(font[1][FontConsoleSize], weapon_base[bot[MY_BOT_NR]->gunModel].name, textCol[3], 0.01f*MAX_WIDTH+170, MAX_HEIGHT-34.0f);

    // ----- Ammo count -----

    oss << bot[MY_BOT_NR]->leftAmmos;
    printText(font[1][FontConsoleSize], oss.str(), textCol[3], 0.46f*MAX_WIDTH+33, MAX_HEIGHT-32.0f);
    oss.str("");


    glEnable(GL_TEXTURE_2D);

    // ----- Health -----

    draw_int_help(text_health, 0.01f*MAX_WIDTH, MAX_HEIGHT-38.0f);

    text_healthbar.w = 115.0f*bot[MY_BOT_NR]->actLife / fullLife;
    draw_int_help(text_healthbar, 0.01f*MAX_WIDTH+40, MAX_HEIGHT-29.0f);


    // ----- Ammo -----

    draw_int_help(text_ammo, 0.46f*MAX_WIDTH, MAX_HEIGHT-38.0f);

    // grenades' count
    for (unsigned int i = 0; i < bot[MY_BOT_NR]->numGrenades; ++i)
    {
        draw_int_help(text_nade, 0.46f*MAX_WIDTH+i*10, MAX_HEIGHT-15.0f);
    }

    if (!bot[MY_BOT_NR]->isReloading)
        text_reloadbar.w = 115.0f*bot[MY_BOT_NR]->leftAmmos / weapon_base[bot[MY_BOT_NR]->gunModel].ammo;
    draw_int_help(text_reloadbar, 0.46f*MAX_WIDTH+67, MAX_HEIGHT-29.0f);


    // -----  Little gold -----

    draw_int_help(text_firebar_r, 0.46f*MAX_WIDTH + text_ammo.w - text_firebar_r.w, MAX_HEIGHT-14.0f);
    float time_gold = static_cast<float>(getCurrentTime - bot[MY_BOT_NR]->lastShotTime) / static_cast<float>(weapon_base[bot[MY_BOT_NR]->gunModel].fireInterval);
    text_firebar.w = 57.0f*(time_gold > 1.0f ? 1.0f : time_gold);
    draw_int_help(text_firebar, 0.46f*MAX_WIDTH + text_ammo.w - text_firebar_r.w, MAX_HEIGHT-14.0f);


    // ----- Flying -----

    draw_int_help(text_jet, 0.75f*MAX_WIDTH, MAX_HEIGHT-38.0f);
    text_jetbar.w = 115.0f*bot[MY_BOT_NR]->procJet;
    draw_int_help(text_jetbar, 0.75f*MAX_WIDTH+40.0f, MAX_HEIGHT-29.0f);


    // ----- Player status -----

    // my position / players number
    oss << MY_CURRENT_POS;
    temp = oss.str() + "/";
    oss.str("");
    oss << bot.size();
    temp += oss.str();
    printText(font[1][FontConsoleSize], temp, textCol[4], 0.87f*MAX_WIDTH, MAX_HEIGHT-65.0f);
    oss.str("");

    // kills / points (points to leader)
    if (CURRENT_GAME_MODE == DM)
        oss << bot[MY_BOT_NR]->killedNr;
    else
        oss << bot[MY_BOT_NR]->points;
    temp = oss.str() + " (";
    oss.str("");
    if (DISTANCE_SCORE < 0)
        temp += "-";
    else
        temp += "+";
    oss << abs(DISTANCE_SCORE);
    temp += oss.str() + ")";
    printText(font[1][FontConsoleSize], temp, textCol[1], 0.87f*MAX_WIDTH, MAX_HEIGHT-55.0f);
    oss.str("");

    // kill / point / capture limit
    oss << FIRST_LIMIT;
    printText(font[1][FontConsoleSize], oss.str(), textCol[2], 0.87f*MAX_WIDTH, MAX_HEIGHT-45.0f);
    oss.str("");

    glDisable(GL_TEXTURE_2D);


    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (bot[i]->isKilled)
        {
            Uint32 new_time = getCurrentTime - bot[i]->timerRespawnTime;
            if (new_time > bot[i]->respawnTime)
            {
                bot[i]->isKilled = false;

                // set start position
                int point = static_cast<int>(rand()%spawnpoint[bot[i]->team].size());
                bot[i]->position.x = map->spawnpoint[spawnpoint[bot[i]->team][point]].x - bot[i]->w/2;
                bot[i]->position.y = map->spawnpoint[spawnpoint[bot[i]->team][point]].y - bot[i]->h/2;

                if (i == MY_BOT_NR)
                {
                    backg->setPosition(OLD_POS.x - bot[MY_BOT_NR]->position.x, OLD_POS.y - bot[MY_BOT_NR]->position.y);
                    if (SOUNDS_VOL > 0)
                        Mix_PlayChannel(-1, sound_new_life, 0);
                }

            }
            else if (i == MY_BOT_NR)
            {
                oss << static_cast<float>(bot[MY_BOT_NR]->respawnTime - new_time)/1000;
                printTextMiddle(font[1][FontMenuSize], "Respawn in " + oss.str(), textCol[1], 50.0f);
                printTextMiddle(font[1][FontMenuSize], "Killed by " + bot[bot[MY_BOT_NR]->killer]->name, textCol[1], MAX_HEIGHT-94.0f);
                oss.str("");

                // wait half a second after death and then show gun menu
                if (new_time >= 500 && !CHOICE_GUN && !SHOW_GUN_MENU)
                {
                    window_guns = new WindowGuns(weapon_base);
                    SHOW_GUN_MENU = true;
                }
            }
        }
    }

    if (!bot[MY_BOT_NR]->isKilled)
        CHOICE_GUN = false;

    if (YOU_KILLED)
    {
        if (getCurrentTime - bot[MY_BOT_NR]->youKilledTime <= 1000)
            printTextMiddle(font[1][FontMenuSize], "You killed " + bot[bot[MY_BOT_NR]->killed]->name, textCol[1], 50.0f);
        else
            YOU_KILLED = false;
    }


    if (SHOW_GUN_MENU && !CHOICE_GUN)
    {
        int x, y;
        //SDL_ShowCursor(SDL_ENABLE);
        window_guns->draw(mouse->getLocalPosition());

        // if select gun with mouse button
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, menu_click, 0);

            int nr = window_guns->select();

            if (nr != -1)
            {
                bot[MY_BOT_NR]->gunModel = nr;
                bot[MY_BOT_NR]->leftAmmos = weapon_base[bot[MY_BOT_NR]->gunModel].ammo;
                CHOICE_GUN = true;
                SHOW_GUN_MENU = false;
            }
        }
        // if select gun with key
        else if (KEY_PRESSED >= SDLK_0 && KEY_PRESSED <= SDLK_9)
        {
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, menu_click, 0);

            bot[MY_BOT_NR]->gunModel = KEY_PRESSED-48;
            bot[MY_BOT_NR]->leftAmmos = weapon_base[bot[MY_BOT_NR]->gunModel].ammo;
            CHOICE_GUN = true;
            SHOW_GUN_MENU = false;
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }

    chat->update();
    chat->draw();

    if (SHOW_STATS)
    {
        oss << "FPS: ";
        oss << currentFPS;
        printText(font[1][FontConsoleSize], oss.str(), textCol[3], 0.8f*MAX_WIDTH, 15.0f);
        oss.str("");
    }

    // scores
    if (SHOW_SCORES)
    {
        window_scores->draw();
    }

    if (SHOW_COMMAND_LINE)
    {
        command_line();
    }

    if (SHOW_MYCHAT_LINE)
    {
        mychat_line();
    }


    if (SHOW_ESC)
    {
        int x, y;
        //glutSetCursor(GLUT_CURSOR_INHERIT);
        window_exit->draw(mouse->getLocalPosition());
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            CHOICE_EXIT = window_exit->select();
            // std::cout << "C " << CHOICE_EXIT << std::endl;
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }


}
