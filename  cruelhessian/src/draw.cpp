/*   draw.cpp
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

#include "SDL_mixer.h"
#include <sstream>
#include <algorithm>

#include "worldmap.h"
#include "globals.h"
#include "bot.h"
#include "grenade.h"
#include "bullet.h"
#include "soil/SOIL.h"
#include "fonts.h"

#define LINE(move, start, end, i, kier) \
        glVertex2f(part_x[move][start][i][kier], part_y[move][start][i][kier]);glVertex2f(part_x[move][end][i][kier], part_y[move][end][i][kier]);


const float _180overpi = 57.29f;

std::vector<Bot *> bot;


void WorldMap::make_lines(MT move, int frame, int kierunek, TEAM team)
{

    glBegin(GL_LINES);

    glColor3ub(textCol[team][0], textCol[team][1], textCol[team][2]);

    LINE(move, 0, 3, frame, kierunek);
    LINE(move, 0, 16, frame, kierunek);
    LINE(move, 1, 2, frame, kierunek);
    LINE(move, 1, 17, frame, kierunek);
    LINE(move, 2, 5, frame, kierunek);
    LINE(move, 3, 4, frame, kierunek);
    LINE(move, 4, 5, frame, kierunek);
    LINE(move, 4, 6, frame, kierunek);
    LINE(move, 4, 7, frame, kierunek);
    LINE(move, 5, 6, frame, kierunek);
    LINE(move, 5, 7, frame, kierunek);
    LINE(move, 6, 7, frame, kierunek);
    LINE(move, 6, 8, frame, kierunek);
    LINE(move, 6, 9, frame, kierunek);
    LINE(move, 6, 10, frame, kierunek);
    LINE(move, 6, 10, frame, kierunek);
    LINE(move, 7, 8, frame, kierunek);
    LINE(move, 7, 9, frame, kierunek);
    LINE(move, 7, 10, frame, kierunek);
    LINE(move, 8, 9, frame, kierunek);
    LINE(move, 8, 10, frame, kierunek);
    LINE(move, 8, 11, frame, kierunek);
    LINE(move, 9, 10, frame, kierunek);
    LINE(move, 9, 12, frame, kierunek);
    LINE(move, 10, 13, frame, kierunek);
    LINE(move, 12, 15, frame, kierunek);
    LINE(move, 13, 14, frame, kierunek);

    LINE(move, 14, 18, frame, kierunek);
    LINE(move, 15, 19, frame, kierunek);

    glEnd();

}


void WorldMap::draw_gostek()
{

    for (unsigned int bot_nr = 0; bot_nr < bot.size(); bot_nr++)
    {
        if (!bot[bot_nr]->isKilled)
        {
            glPushMatrix();
            glTranslatef(bot[bot_nr]->position.x, bot[bot_nr]->position.y, 0.0f);
            make_lines(bot[bot_nr]->movementType, bot[bot_nr]->currentFrame, bot[bot_nr]->movementDirection, bot[bot_nr]->team);
            glPopMatrix();
        }
        else
        {
            //Uint32 new_time;
            if ((getCurrentTime - bot[bot_nr]->timerRespawnTime) > bot[bot_nr]->respawnTime)
                bot[bot_nr]->isKilled = false;
        }
    }
}

void WorldMap::printText(freetype::font_data& font, const std::string& text, ubyte* color, float x, float y)
{
    glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    //glColor4ub(color[0], color[1], color[2], color[3]);
    glLoadIdentity();
    freetype::print(font, x, static_cast<float>(MAX_HEIGHT)-y-12.0f, text.c_str());
    glPopMatrix();
}

void WorldMap::printTextMovable(freetype::font_data& font, const std::string& text, ubyte* color, float x, float y)
{
    //  glPushMatrix();
    //  glLoadIdentity();
    //  font.draw(bgX + x, bgY + y, text.c_str());
    //  font.setForegroundColor(color);
    //  glPopMatrix();

    //glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    //glColor4ub(color[0], color[1], color[2], color[3]);
    //glLoadIdentity();
    freetype::print(font, x, static_cast<float>(MAX_HEIGHT)-y-12.0f, text.c_str());
    //glPopMatrix();
}


void WorldMap::draw_arms()
{
    glEnable(GL_TEXTURE_2D);

    for (std::list<Grenade *>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        if (getCurrentTime - (*temp)->timer_throw <= 4000)
        {
            glPushMatrix();
            glTranslatef((*temp)->position.x, (*temp)->position.y, 0.0f);

            glBindTexture(GL_TEXTURE_2D, text_nade.tex);

            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.0);
            glVertex2f(0.0, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(text_nade.w, 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex2f(text_nade.w, text_nade.h);
            glTexCoord2f(0.0, 0.0);
            glVertex2f(0.0, text_nade.h);
            glEnd();

            glPopMatrix();
        }

        else
        {
            // explode after 4 second
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, grenade_explosion, 0);

            if ((getCurrentTime - (*temp)->timer_change_frame >= 20))
            {
                (*temp)->timer_change_frame = getCurrentTime;
                if ((*temp)->currentFrame < 16)
                {
                    glPushMatrix();
                    glTranslatef((*temp)->position.x-text_grenade[(*temp)->currentFrame].w/2,
                                 (*temp)->position.y-text_grenade[(*temp)->currentFrame].h/2, 0.0);

                    glBindTexture(GL_TEXTURE_2D, text_grenade[(*temp)->currentFrame].tex);

                    glBegin(GL_QUADS);
                    glTexCoord2f(0.0, 1.0);
                    glVertex2f(0.0, 0.0);
                    glTexCoord2f(1.0, 1.0);
                    glVertex2f(text_grenade[(*temp)->currentFrame].w, 0.0);
                    glTexCoord2f(1.0, 0.0);
                    glVertex2f(text_grenade[(*temp)->currentFrame].w, text_grenade[(*temp)->currentFrame].h);
                    glTexCoord2f(0.0, 0.0);
                    glVertex2f(0.0, text_grenade[(*temp)->currentFrame].h);
                    glEnd();

                    glPopMatrix();

                    (*temp)->currentFrame++;
                }
                // remove grenade form the list
                else
                    temp = gren_list.erase(temp);
            }


        }
    }

    for (std::list<Bullet *>::iterator temp = ammo_list.begin(); temp != ammo_list.end(); temp++)
    {
        glPushMatrix();

        glTranslatef((*temp)->position.x, (*temp)->position.y, 0.0f);
        glRotatef((GLfloat)(_180overpi*atan((*temp)->velocity.y / (*temp)->velocity.x)), 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, weapon[(*temp)->gunModel].textureAmmo.tex);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(0.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex2f(weapon[(*temp)->gunModel].textureAmmo.w, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex2f(weapon[(*temp)->gunModel].textureAmmo.w, weapon[(*temp)->gunModel].textureAmmo.h);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(0.0, weapon[(*temp)->gunModel].textureAmmo.h);
        glEnd();
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
}



void WorldMap::draw_background()
{

    // przesun tlo ekranu w zaleznosci od polozenia Bota
    if (SHOW_GUN_MENU && ONLY_ONCE)
    {
        // when bot was killed
        ONLY_ONCE = false;
        glTranslatef(OLD_POS_X - bot[MY_BOT_NR]->position.x, OLD_POS_Y - bot[MY_BOT_NR]->position.y, 0.0f);
    }

    else
        if (MOUSE_X <= (int)bot[MY_BOT_NR]->position.x + MAX_WIDTH &&
                MOUSE_X >= (int)bot[MY_BOT_NR]->position.x - MAX_WIDTH &&
                MOUSE_Y <= (int)bot[MY_BOT_NR]->position.y + MAX_HEIGHT &&
                MOUSE_Y >= (int)bot[MY_BOT_NR]->position.y - MAX_HEIGHT)
        {
            SDL_GetRelativeMouseState(&REL_MOUSE_X, &REL_MOUSE_Y);
            glTranslatef((GLfloat)-REL_MOUSE_X, (GLfloat)-REL_MOUSE_Y, 0.0f);
        }

    glBegin(GL_POLYGON);
    glColor4ub((GLubyte)p.bgColorTop.red, (GLubyte)p.bgColorTop.green, (GLubyte)p.bgColorTop.blue, (GLubyte)p.bgColorTop.alpha);
    glVertex2f((GLfloat)p.leftoffs - MAX_WIDTH, (GLfloat)-p.topoffs);
    glVertex2f((GLfloat)p.rightoffs + MAX_WIDTH, (GLfloat)-p.topoffs);
    glColor4ub((GLubyte)p.bgColorBottom.red, (GLubyte)p.bgColorBottom.green, (GLubyte)p.bgColorBottom.blue, (GLubyte)p.bgColorBottom.alpha);
    glVertex2f((GLfloat)p.rightoffs + MAX_WIDTH, (GLfloat)-p.bottomoffs);
    glVertex2f((GLfloat)p.leftoffs - MAX_WIDTH, (GLfloat)-p.bottomoffs);
    glEnd();

}


void WorldMap::draw_screen()
{
    glEnable(GL_TEXTURE_2D);

    // draw scenery on the back
    for (int i = 0; i < p.propCount; i++)
    {
        if (p.prop[i].level != p.dbBEHIND_NONE)
        {
            glPushMatrix();
            glTranslatef(p.prop[i].x, p.prop[i].y, 0.0);
            glRotatef(-p.prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(p.prop[i].scaleX, p.prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, text_scen[p.prop[i].style-1]);

            glBegin(GL_QUADS);

            glColor4ub(p.prop[i].color.red, p.prop[i].color.green,
                       p.prop[i].color.blue, p.prop[i].alpha);

            glTexCoord2f(0.0, 1.0);
            glVertex2f(0.0, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(static_cast<float>(p.prop[i].width), 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex2f(static_cast<float>(p.prop[i].width), static_cast<float>(p.prop[i].height));
            glTexCoord2f(0.0, 0.0);
            glVertex2f(0.0, static_cast<float>(p.prop[i].height));

            glEnd();

            glPopMatrix();
        }
    }

    glBindTexture(GL_TEXTURE_2D, text_poly);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < p.polygonCount; i++)
    {
        /*for (int m = 0; m < 3; m++)
        {
            glTexCoord3f(p.polygon[i].vertex[m].tu, -p.polygon[i].vertex[m].tv, 1.0 / p.polygon[i].vertex[m].rhw);

            glColor4ub(p.polygon[i].vertex[m].color.red,
                       p.polygon[i].vertex[m].color.green,
                       p.polygon[i].vertex[m].color.blue,
                       p.polygon[i].vertex[m].color.alpha);

            //glVertex3f(p.polygon[i].vertex[m].x, p.polygon[i].vertex[m].y, 0.0);
            glVertex3f(p.polygon[i].vertex[m].x, p.polygon[i].vertex[m].y, p.polygon[i].vertex[m].z);
        }*/

        glTexCoord3f(p.polygon[i].vertex[0].tu, -p.polygon[i].vertex[0].tv, 1.0f / p.polygon[i].vertex[0].rhw);
        glColor4ub(p.polygon[i].vertex[0].color.red,
                   p.polygon[i].vertex[0].color.green,
                   p.polygon[i].vertex[0].color.blue,
                   p.polygon[i].vertex[0].color.alpha);
        glVertex3f(p.polygon[i].vertex[0].x, p.polygon[i].vertex[0].y, p.polygon[i].vertex[0].z);

        glTexCoord3f(p.polygon[i].vertex[1].tu, -p.polygon[i].vertex[1].tv, 1.0f / p.polygon[i].vertex[1].rhw);
        glColor4ub(p.polygon[i].vertex[1].color.red,
                   p.polygon[i].vertex[1].color.green,
                   p.polygon[i].vertex[1].color.blue,
                   p.polygon[i].vertex[1].color.alpha);
        glVertex3f(p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y, p.polygon[i].vertex[1].z);

        glTexCoord3f(p.polygon[i].vertex[2].tu, -p.polygon[i].vertex[2].tv, 1.0f / p.polygon[i].vertex[2].rhw);
        glColor4ub(p.polygon[i].vertex[2].color.red,
                   p.polygon[i].vertex[2].color.green,
                   p.polygon[i].vertex[2].color.blue,
                   p.polygon[i].vertex[2].color.alpha);
        glVertex3f(p.polygon[i].vertex[2].x, p.polygon[i].vertex[2].y, p.polygon[i].vertex[2].z);
    }
    glEnd();

    // draw scenery on the front
    for (size_t i = 0; i < p.prop.size(); i++)
    {
        if (p.prop[i].level == p.dbBEHIND_NONE)
        {
            glPushMatrix();
            glTranslatef(p.prop[i].x, p.prop[i].y, 0.0);
            glRotatef(-p.prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(p.prop[i].scaleX, p.prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, text_scen[p.prop[i].style-1]);

            glBegin(GL_QUADS);

            glColor4ub(p.prop[i].color.red, p.prop[i].color.green,
                       p.prop[i].color.blue, p.prop[i].alpha);

            glTexCoord2f(0.0, 1.0);
            glVertex2f(0.0, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(static_cast<float>(p.prop[i].width), 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex2f(static_cast<float>(p.prop[i].width), static_cast<float>(p.prop[i].height));
            glTexCoord2f(0.0, 0.0);
            glVertex2f(0.0, static_cast<float>(p.prop[i].height));
            glEnd();

            glPopMatrix();
        }
    }
    glDisable(GL_TEXTURE_2D);
}


void WorldMap::draw_interface()
{
    int x, y;
    std::ostringstream oss;

    // ----- Gun's name -----

    printText(font2_12, weapon[bot[MY_BOT_NR]->gunModel].name, textCol[2], 0.01f*MAX_WIDTH+170, static_cast<float>(MAX_HEIGHT-34));

    // ----- Ammo count -----

    oss << bot[MY_BOT_NR]->leftAmmos;
    printText(font2_12, oss.str(), textCol[2], 0.46f*MAX_WIDTH+33, static_cast<float>(MAX_HEIGHT-32));
    oss.str("");


    glEnable(GL_TEXTURE_2D);

    // ----- Health -----

    draw_int_help(text_health, 0.01f*MAX_WIDTH, static_cast<float>(MAX_HEIGHT-38));

    text_healthbar.w = 115.0f*bot[MY_BOT_NR]->actLife / fullLife;
    draw_int_help(text_healthbar, 0.01f*MAX_WIDTH+40, static_cast<float>(MAX_HEIGHT-31));


    // ----- Ammo -----

    draw_int_help(text_ammo, 0.46f*MAX_WIDTH, static_cast<float>(MAX_HEIGHT-44));

    // grenades' count
    for (int i = 0; i < bot[MY_BOT_NR]->numGrenades; ++i)
    {
        draw_int_help(text_nade, 0.46f*MAX_WIDTH+i*10, static_cast<float>(MAX_HEIGHT-15));
    }

    if (!bot[MY_BOT_NR]->isReloading)
        text_reloadbar.w = 115.0f*bot[MY_BOT_NR]->leftAmmos / weapon[bot[MY_BOT_NR]->gunModel].ammo;
    draw_int_help(text_reloadbar, 0.46f*MAX_WIDTH+67, static_cast<float>(MAX_HEIGHT-31));


    // -----  Little gold

    draw_int_help(text_firebar_r, 0.46f*MAX_WIDTH + text_ammo.w - text_firebar_r.w, static_cast<float>(MAX_HEIGHT-14));
    float time_gold = static_cast<float>(getCurrentTime - bot[MY_BOT_NR]->lastShotTime) / static_cast<float>(weapon[bot[MY_BOT_NR]->gunModel].fireInterval);
    text_firebar.w = 57.0f*(time_gold > 1.0f ? 1.0f : time_gold);
    draw_int_help(text_firebar, 0.46f*MAX_WIDTH + text_ammo.w - text_firebar_r.w, static_cast<float>(MAX_HEIGHT-14));


    // ----- Flying -----

    draw_int_help(text_jet, 0.75f*MAX_WIDTH, static_cast<float>(MAX_HEIGHT-44));
    text_jetbar.w = 115.0f*bot[MY_BOT_NR]->procJet;
    draw_int_help(text_jetbar, 0.75f*MAX_WIDTH+40, static_cast<float>(MAX_HEIGHT-31));

    glDisable(GL_TEXTURE_2D);




    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (bot[i]->isKilled)
        {
            Uint32 new_time = getCurrentTime - bot[i]->timerRespawnTime;
            if (new_time > bot[i]->respawnTime)
            {
                bot[i]->isKilled = false;
                if (i == MY_BOT_NR && SOUNDS_VOL > 0)
                    Mix_PlayChannel(-1, sound_new_life, 0);
            }
            else if (i == MY_BOT_NR)
            {
                oss << static_cast<float>(bot[MY_BOT_NR]->respawnTime - new_time)/1000;
                printText(font2_12, "Respawn in " + oss.str(), textCol[0], 0.5f*MAX_WIDTH-20, 50.0f);
                printText(font2_12, "Killed by " + bot[bot[MY_BOT_NR]->killer]->name, textCol[0], 0.5f*MAX_WIDTH-80, static_cast<float>(MAX_HEIGHT-94));
                oss.str("");
                if (new_time >= 500 && !CHOICE_GUN)
                {
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
            printText(font2_12, "You killed " + bot[bot[MY_BOT_NR]->killed]->name, textCol[0], 0.5f*MAX_WIDTH-20, 50.0f);
        else
            YOU_KILLED = false;
    }


    if (SHOW_GUN_MENU && !CHOICE_GUN)
    {
        //SDL_ShowCursor(SDL_ENABLE);
        gun_menu();
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1) || (KEY_PRESSED >= SDLK_0 && KEY_PRESSED <= SDLK_9))
        {
            gun_menu_select();
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }

    chat_window();

    if (SHOW_STATS)
    {
        oss << "FPS: ";
        oss << currentFPS;
        printText(font2_12, oss.str(), textCol[2], 0.8f*MAX_WIDTH, static_cast<float>(15));
        oss.str("");
    }

    // scores
    if (SHOW_SCORES)
    {
        scores_menu();
    }


    if (SHOW_ESC)
    {
        //glutSetCursor(GLUT_CURSOR_INHERIT);
        exit_menu();
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            exit_menu_select();
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }


}

void WorldMap::chat_window()
{
    // if chat_list isn't empty ...
    // ... delete the first entry after each 7 seconds
    if (getCurrentTime - lastOutChatEntity >= 7000)
    {
        if (!chat_list.empty())
            chat_list.pop_front();
        lastOutChatEntity = getCurrentTime;
    }

    int j = 0;

    for (std::list<string_pair>::iterator i = chat_list.begin(); i != chat_list.end(); ++i)
    {
        printText(font2_12, "<" + (*i).first + "> " + (*i).second, textColorGunOnTouch, 10, (j++)*10 + 15);
    }
}

/*
void WorldMap::draw_infos()
{
    int x, y;
    std::ostringstream oss;

    for (unsigned int i = 0; i < bot.size(); ++i)
    {
        if (bot[i]->isKilled)
        {
            Uint32 new_time = getCurrentTime - bot[i]->timerRespawnTime;
            if (new_time > bot[i]->respawnTime)
            {
                bot[i]->isKilled = false;
                if (i == MY_BOT_NR && SOUNDS_VOL > 0)
                    Mix_PlayChannel(-1, sound_new_life, 0);
            }
            else if (i == MY_BOT_NR)
            {
                oss << static_cast<float>(bot[MY_BOT_NR]->respawnTime - new_time)/1000;
                printText(font2_12, "Respawn in " + oss.str(), textCol[0], 0.5f*MAX_WIDTH-20, 50.0f);
                printText(font2_12, "Killed by " + bot[bot[MY_BOT_NR]->killer]->name, textCol[0], 0.5f*MAX_WIDTH-80, static_cast<float>(MAX_HEIGHT-94));
                oss.str("");
                if (new_time >= 500 && !CHOICE_GUN)
                {
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
            printText(font2_12, "You killed " + bot[bot[MY_BOT_NR]->killed]->name, textCol[0], 0.5f*MAX_WIDTH-20, 50.0f);
        else
            YOU_KILLED = false;
    }


    if (SHOW_GUN_MENU && !CHOICE_GUN)
    {
        //SDL_ShowCursor(SDL_ENABLE);
        gun_menu();
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1) || (KEY_PRESSED >= SDLK_0 && KEY_PRESSED <= SDLK_9))
        {
            gun_menu_select();
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }

    chat_window();

    if (SHOW_STATS)
    {
        oss << "FPS: ";
        oss << currentFPS;
        printText(font2_12, oss.str(), textCol[2], 0.8f*MAX_WIDTH, static_cast<float>(15));
        oss.str("");
    }

    // scores
    if (SHOW_SCORES)
    {
        scores_menu();
    }


    if (SHOW_ESC)
    {
        //glutSetCursor(GLUT_CURSOR_INHERIT);
        exit_menu();
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
        {
            exit_menu_select();
        }
        //glutSetCursor(GLUT_CURSOR_NONE);
    }

}
*/


void WorldMap::draw_mouse()
{
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glLoadIdentity();

    glTranslatef(bgX + NMOUSE_X, bgY + NMOUSE_Y, 0.0f);

    glBindTexture(GL_TEXTURE_2D, text_mouse.tex);

    glBegin(GL_QUADS);
    glTexCoord3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord3f(1.0, 1.0, 0.0);
    glVertex3f(text_mouse.w, 0.0, 0.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(text_mouse.w, text_mouse.h, 0.0);
    glTexCoord3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, text_mouse.h, 0.0);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


GLuint WorldMap::which_part(int nr)
{
    switch (nr)
    {
        //case 0 : return gost_morda;
        //case 1 : return gost_morda;
    case 2 :
        return gost_noga.tex;
    case 3 :
        return gost_noga.tex;
    case 4 :
        return gost_udo.tex;
    case 5 :
        return gost_udo.tex;
        //case 6 : return gost_morda;
    case 7 :
        return gost_biodro.tex;
        //case 8 : return gost_morda;
    case 9 :
        return gost_klata.tex;
    case 10 :
        return gost_ramie.tex;
    case 11 :
        return gost_morda.tex;
    case 12 :
        return gost_reka.tex;
    case 13 :
        return gost_reka.tex;
    case 14 :
        return gost_dlon.tex;
    case 15 :
        return gost_dlon.tex;
    case 16 :
        //return gost_x;
        //case 17 : return gost_stopa;
        //case 18 : return gost_dlon;
        //case 19 : return gost_dlon;
    default :
        return gost_dlon.tex;
    }
//    return 0;

}

GLfloat* WorldMap::which_part_xy(int nr)
{
    nr++;
    //fffffffffffff
    /*switch (nr)
    {
        //case 0 : return gost_morda_xy;
        //case 1 : return gost_morda_xy;
    case 2 :
        return gost_noga_xy;
    case 3 :
        return gost_noga_xy;
    case 4 :
        return gost_udo_xy;
    case 5 :
        return gost_udo_xy;
        //case 6 : return gost_morda_xy;
    case 7 :
        return gost_biodro_xy;
        //case 8 : return gost_morda_xy;
    case 9 :
        return gost_klata_xy;
    case 10 :
        return gost_ramie_xy;
    case 11 :
        return gost_morda_xy;
    case 12 :
        return gost_reka_xy;
    case 13 :
        return gost_reka_xy;
    case 14 :
        return gost_dlon_xy;
    case 15 :
        return gost_dlon_xy;
    case 16 :
        //return gost_x_xy;
        //case 17 : return gost_stopa_xy;
        //case 18 : return gost_dlon_xy;
        //case 19 : return gost_dlon_xy;
    default :
        return gost_dlon_xy;
    }*/
    return 0;

}



void WorldMap::draw_gostek_help(float dx, float dy, int part)
{

    GLfloat *xtex = (GLfloat *)malloc(4*sizeof(GLfloat));
    /*GLfloat xtex[4];
    xtex[0] = *which_part_xy(part);
    xtex[1] = *which_part_xy(part)+1;
    xtex[2] = *which_part_xy(part)+2;
    xtex[3] = *which_part_xy(part)+3;
    */
    xtex = which_part_xy(part);

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    glTranslatef(bot[MY_BOT_NR]->position.x+dx, bot[MY_BOT_NR]->position.y+dy, 0.0);

    glBindTexture(GL_TEXTURE_2D, which_part(part));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f((float)xtex[0], 0.0f);
    glVertex2f((float)xtex[2], 0.0f);
    glTexCoord2f((float)xtex[0], (float)xtex[1]);
    glVertex2f((float)xtex[2], (float)xtex[3]);
    glTexCoord2f(0.0f, (float)xtex[1]);
    glVertex2f(0.0f, (float)xtex[3]);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //free(xtex);
}



void WorldMap::draw_int_help(Tex& tex, float dx, float dy)
{

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(bgX + dx, bgY + dy, 0.0f);

    glBindTexture(GL_TEXTURE_2D, tex.tex);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(tex.w, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(tex.w, tex.h);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, tex.h);
    glEnd();

    glPopMatrix();

}

Tex WorldMap::SOIL_LoadTextureEx(const std::string& file)
{
    Tex res_tex;
    res_tex.tex = 0;
    int temp_w = 0;
    int temp_h = 0;

    if (file[file.size()-1] == 'p')
    {
        int channels;
        unsigned char *imgdata = SOIL_load_image
                                 (
                                     file.c_str(),
                                     &temp_w, &temp_h, &channels,
                                     SOIL_LOAD_RGBA //| SOIL_FLAG_INVERT_Y
                                     //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                                     //SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
                                 );

        if (imgdata == NULL)
        {
            std::cout << "Image was not loaded : " << file << std::endl;
            res_tex.w = static_cast<float>(temp_w);
            res_tex.h = static_cast<float>(temp_h);
            return res_tex;
        }

        for (int i = 0; i < temp_w*temp_h*channels; i += 4)
        {
            if (imgdata[i] == 0 && imgdata[i+1] == 255 && imgdata[i+2] == 0)
            {
                imgdata[i+1] = imgdata[i+3] = 0;
            }
        }
        res_tex.tex = SOIL_create_OGL_texture
                      (
                          imgdata, temp_w, temp_h, channels, SOIL_CREATE_NEW_ID,
                          SOIL_LOAD_RGBA //| SOIL_FLAG_INVERT_Y
                      );

        SOIL_free_image_data(imgdata);
    }

    else if (file[file.size()-1] == 'g')
    {
        res_tex.tex = SOIL_load_OGL_texture
                      (
                          file.c_str(),
                          SOIL_LOAD_AUTO,
                          SOIL_CREATE_NEW_ID,
                          SOIL_LOAD_RGBA | SOIL_FLAG_COMPRESS_TO_DXT// | SOIL_FLAG_INVERT_Y
                          //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                      );
    }


    res_tex.w = static_cast<float>(temp_w);
    res_tex.h = static_cast<float>(temp_h);
    return res_tex;
}

GLuint WorldMap::SOIL_LoadTexture(const std::string& file)
{
    GLuint texID = 0;

    if (file[file.size()-1] == 'p')
    {
        //glGenTextures(1,&texID);
        //glBindTexture(GL_TEXTURE_2D,texID);
        int width, height, channels;
        unsigned char *imgdata = SOIL_load_image
                                 (
                                     file.c_str(),
                                     &width, &height, &channels,
                                     SOIL_LOAD_RGBA
                                     //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                                     //SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
                                 );
        //ubyte v;
        if (imgdata == NULL)
        {
            std::cout << "Image was not loaded : " << file << std::endl;
            return texID;
        }

        for (int i = 0; i < width*height*channels; i += 4)
        {
            if (imgdata[i] == 0 && imgdata[i+1] == 255 && imgdata[i+2] == 0)
            {
                imgdata[i+1] = imgdata[i+3] = 0;
            }
        }
        texID = SOIL_create_OGL_texture
                (
                    imgdata, width, height, channels, SOIL_CREATE_NEW_ID,
                    SOIL_LOAD_RGBA | SOIL_FLAG_INVERT_Y
                );

        SOIL_free_image_data(imgdata);

    }

    else if (file[file.size()-1] == 'g')
    {
        texID = SOIL_load_OGL_texture
                (
                    file.c_str(),
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_LOAD_RGBA | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
                    //SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                );
    }

    return texID;
}
