/*   draw.cpp
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

#include "SDL_mixer.h"
#include "SDL_opengl.h"
#include <sstream>
#include "worldmap.h"
#include "globals.h"
#include "bot.h"
#include "grenade.h"
#include "bullet.h"
#include "physics.h"
#include "draw_extra.h"



const float _180overpi = 57.29578049;

void WorldMap::draw_gostek()
{

    // calc current frame
    for (int bot_nr = 0; bot_nr < world->BOTS_NUM; bot_nr++)
    {
        if (!bot[bot_nr]->isKilled && (SDL_GetTicks() - bot[bot_nr]->timerChangeFrame >= 30))
        {
            bot[bot_nr]->timerChangeFrame = SDL_GetTicks();
            if (bot[bot_nr]->currentFrame >= FRAMES_MAX[bot[bot_nr]->movementType]-1)
                bot[bot_nr]->currentFrame = 0;
            else
                bot[bot_nr]->currentFrame++;
        }
    }

    for (int bot_nr = 0; bot_nr < world->BOTS_NUM; bot_nr++)
    {
        if (!bot[bot_nr]->isKilled)
        {
            glPushMatrix();
            glTranslatef(bot[bot_nr]->position.x, bot[bot_nr]->position.y, 0.0f);
            make_lines(bot[bot_nr]->movementType, bot[bot_nr]->currentFrame, bot[bot_nr]->movementDirection);
            glPopMatrix();
        }
    }
}


void WorldMap::draw_arms()
{
    glEnable(GL_TEXTURE_2D);

    for (list<Grenade *>::iterator temp = gren_list.begin(); temp != gren_list.end(); temp++)
    {
        if (SDL_GetTicks() - (*temp)->timer_throw <= 4000)
        {
            glPushMatrix();
            glTranslatef((*temp)->position.x, (*temp)->position.y, 0.0f);

            glBindTexture(GL_TEXTURE_2D, text_nade);

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f((float)text_nade_xy[0], 0.0f);
            glVertex2f((float)text_nade_xy[2], 0.0f);
            glTexCoord2f((float)text_nade_xy[0], (float)text_nade_xy[1]);
            glVertex2f((float)text_nade_xy[2], (float)text_nade_xy[3]);
            glTexCoord2f(0.0f, (float)text_nade_xy[1]);
            glVertex2f(0.0f, (float)text_nade_xy[3]);
            glEnd();

            glPopMatrix();
        }

        else
        {
            // explode after 4 second
            if (SOUNDS_VOL > 0)
                Mix_PlayChannel(-1, Mix_LoadWAV((SOL_PATH+"Sfx/grenade-explosion.wav").c_str()), 0);

            if ((SDL_GetTicks() - (*temp)->timer_change_frame >= 20))
            {
                (*temp)->timer_change_frame = SDL_GetTicks();
                if ((*temp)->currentFrame < 16)
                {
                    glPushMatrix();

                    glTranslatef((*temp)->position.x-text_grenade_xy[(*temp)->currentFrame][2]/2,
                                 (*temp)->position.y-text_grenade_xy[(*temp)->currentFrame][3]/2, 0.0);

                    glBindTexture(GL_TEXTURE_2D, text_grenade[(*temp)->currentFrame]);

                    glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2f(0.0f, 0.0f);
                    glTexCoord2f((float)text_grenade_xy[(*temp)->currentFrame][0], 0.0f);
                    glVertex2f((float)text_grenade_xy[(*temp)->currentFrame][2], 0.0f);
                    glTexCoord2f((float)text_grenade_xy[(*temp)->currentFrame][0], (float)text_grenade_xy[(*temp)->currentFrame][1]);
                    glVertex2f((float)text_grenade_xy[(*temp)->currentFrame][2], (float)text_grenade_xy[(*temp)->currentFrame][3]);
                    glTexCoord2f(0.0f, (float)text_grenade_xy[(*temp)->currentFrame][1]);
                    glVertex2f(0.0f, (float)text_grenade_xy[(*temp)->currentFrame][3]);
                    glEnd();

                    glPopMatrix();

                    (*temp)->currentFrame++;
                }
                // remove grenade form the list
            //    else
             //       temp = gren_list.erase(temp);
            }


        }
    }

    for (list<Bullet *>::iterator temp = ammo_list.begin(); temp != ammo_list.end(); temp++)
    {
        glPushMatrix();
        //cout << (*temp)->position.x << " " << (*temp)->position.y << endl;
        glTranslatef((*temp)->position.x, (*temp)->position.y, 0.0f);

        glRotatef((GLfloat)(_180overpi*atan((*temp)->velocity.y / (*temp)->velocity.x)), 0.0f, 0.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, text_ammos[(*temp)->gunModel]);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f((float)text_ammos_xy[(*temp)->gunModel][0], 0.0f);
        glVertex2f((float)text_ammos_xy[(*temp)->gunModel][2], 0.0f);
        glTexCoord2f((float)text_ammos_xy[(*temp)->gunModel][0], (float)text_ammos_xy[(*temp)->gunModel][1]);
        glVertex2f((float)text_ammos_xy[(*temp)->gunModel][2], (float)text_ammos_xy[(*temp)->gunModel][3]);
        glTexCoord2f(0.0f, (float)text_ammos_xy[(*temp)->gunModel][1]);
        glVertex2f(0.0f, (float)text_ammos_xy[(*temp)->gunModel][3]);
        glEnd();

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
}



void WorldMap::draw_background()
{
    int dx, dy;

    // przesun tlo ekranu w zaleznosci od polozenia Bota

    if (CHOICE_GUN_MENU && ONLY_ONCE)
    {
        ONLY_ONCE = false;
        glTranslatef(OLD_POS_X-bot[MY_BOT_NR]->position.x, OLD_POS_Y-bot[MY_BOT_NR]->position.y, 0.0f);
    }

    else if (!CHOICE_GUN_MENU && ((int)CURRENT_MOUSE_POS.x <= (int)bot[MY_BOT_NR]->position.x + MAX_WIDTH &&
                                  (int)CURRENT_MOUSE_POS.x >= (int)bot[MY_BOT_NR]->position.x - MAX_WIDTH &&
                                  (int)CURRENT_MOUSE_POS.y <= (int)bot[MY_BOT_NR]->position.y + MAX_HEIGHT &&
                                  (int)CURRENT_MOUSE_POS.y >= (int)bot[MY_BOT_NR]->position.y - MAX_HEIGHT))
    {
        SDL_GetRelativeMouseState(&dx, &dy);
        glTranslatef((GLfloat)-dx, (GLfloat)-dy, 0.0f);
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
    glColor4ub((GLubyte)p.bgColorTop.red, (GLubyte)p.bgColorTop.green, (GLubyte)p.bgColorTop.blue, (GLubyte)p.bgColorTop.alpha);
    glVertex2f((GLfloat)p.leftoffs, (GLfloat)-p.topoffs);
    glVertex2f((GLfloat)p.rightoffs, (GLfloat)-p.topoffs);
    glColor4ub((GLubyte)p.bgColorBottom.red, (GLubyte)p.bgColorBottom.green, (GLubyte)p.bgColorBottom.blue, (GLubyte)p.bgColorBottom.alpha);
    glVertex2f((GLfloat)p.rightoffs, (GLfloat)-p.bottomoffs);
    glVertex2f((GLfloat)p.leftoffs, (GLfloat)-p.bottomoffs);
    glEnd();

}

void WorldMap::draw_screen()
{
    int k, i;
//    int dx, dy;


    /*

        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        glColor4ub((GLubyte)TP_red, (GLubyte)TP_green, (GLubyte)TP_blue, (GLubyte)TP_alpha);
        glVertex2f(PMS_FILE_leftoffs, -PMS_FILE_topoffs);
        glVertex2f(PMS_FILE_rightoffs, -PMS_FILE_topoffs);
        glColor4ub((GLubyte)BM_red, (GLubyte)BM_green, (GLubyte)BM_blue, (GLubyte)BM_alpha);
        glVertex2f(PMS_FILE_rightoffs, -PMS_FILE_Bottomoffs);
        glVertex2f(PMS_FILE_leftoffs, -PMS_FILE_Bottomoffs);
        glEnd();
    */

    glEnable(GL_TEXTURE_2D);

    //printf("\tRysowanie scenerii (z tylu) ...\n");
    for (i = 0; i < p.propCount; i++)
    {
        if (p.prop[i].level != dbBEHIND_NONE)
        {
            k = p.prop[i].style-1;

            glPushMatrix();

            glTranslatef(p.prop[i].x, p.prop[i].y, 0.0f);
            glRotatef(-p.prop[i].rotation * _180overpi, 0.0f, 0.0f, 1.0f);
            glScalef(p.prop[i].scaleX, p.prop[i].scaleY, 1.0f);

            glBindTexture(GL_TEXTURE_2D, text_scen[k]);

            glBegin(GL_QUADS);
            glColor4ub((GLubyte)p.prop[i].color.red, (GLubyte)p.prop[i].color.green, (GLubyte)p.prop[i].color.blue, (GLubyte)p.prop[i].alpha);

            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);

            glTexCoord2f((float)text_scen_xy[k][0], 0.0f);
            glVertex2f((float)text_scen_xy[k][2], 0.0f);

            glTexCoord2f((float)text_scen_xy[k][0], (float)text_scen_xy[k][1]);
            glVertex2f((float)text_scen_xy[k][2], (float)text_scen_xy[k][3]);

            glTexCoord2f(0.0f, (float)text_scen_xy[k][1]);
            glVertex2f(0.0f, (float)text_scen_xy[k][3]);

            glEnd();

            glPopMatrix();
        }
    }


    //printf("\tTeksturowanie trojkatow ...\n");
    glBindTexture(GL_TEXTURE_2D, text_poly);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < p.polygonCount; i++)
    {
        glTexCoord3f(p.polygon[i].vertex[0].tu, -p.polygon[i].vertex[0].tv, (GLfloat)(1.0 / p.polygon[i].vertex[0].rhw));
        glColor4ub((GLubyte)p.polygon[i].vertex[0].color.red,
                   (GLubyte)p.polygon[i].vertex[0].color.green,
                   (GLubyte)p.polygon[i].vertex[0].color.blue,
                   (GLubyte)p.polygon[i].vertex[0].color.alpha);
        glVertex3f(p.polygon[i].vertex[0].x, p.polygon[i].vertex[0].y, p.polygon[i].vertex[0].z);

        glTexCoord3f(p.polygon[i].vertex[1].tu, -p.polygon[i].vertex[1].tv, (GLfloat)(1.0 / p.polygon[i].vertex[1].rhw));
        glColor4ub((GLubyte)p.polygon[i].vertex[1].color.red,
                   (GLubyte)p.polygon[i].vertex[1].color.green,
                   (GLubyte)p.polygon[i].vertex[1].color.blue,
                   (GLubyte)p.polygon[i].vertex[1].color.alpha);
        glVertex3f(p.polygon[i].vertex[1].x, p.polygon[i].vertex[1].y, p.polygon[i].vertex[1].z);

        glTexCoord3f(p.polygon[i].vertex[2].tu, -p.polygon[i].vertex[2].tv, (GLfloat)(1.0 / p.polygon[i].vertex[2].rhw));
        glColor4ub((GLubyte)p.polygon[i].vertex[2].color.red,
                   (GLubyte)p.polygon[i].vertex[2].color.green,
                   (GLubyte)p.polygon[i].vertex[2].color.blue,
                   (GLubyte)p.polygon[i].vertex[2].color.alpha);
        glVertex3f(p.polygon[i].vertex[2].x, p.polygon[i].vertex[2].y, p.polygon[i].vertex[2].z);
    }
    glEnd();


    //printf("\tRysowanie scenerii (z przodu) ...\n");
    for (i = 0; i < p.propCount; i++)
    {
        if (p.prop[i].level == dbBEHIND_NONE)
        {
            k = (int)p.prop[i].style-1;

            glPushMatrix();

            glTranslatef(p.prop[i].x, p.prop[i].y, 0.0f);
            glRotatef(-p.prop[i].rotation * _180overpi, 0.0f, 0.0f, 1.0f);
            glScalef((GLfloat)p.prop[i].scaleX, (GLfloat)p.prop[i].scaleY, 1.0f);

            glBindTexture(GL_TEXTURE_2D, text_scen[k]);

            glBegin(GL_QUADS);
            glColor4ub((GLubyte)p.prop[i].color.red, (GLubyte)p.prop[i].color.green, (GLubyte)p.prop[i].color.blue, (GLubyte)p.prop[i].alpha);

            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);

            glTexCoord2f((float)text_scen_xy[k][0], 0.0f);
            glVertex2f((float)text_scen_xy[k][2], 0.0f);

            glTexCoord2f((float)text_scen_xy[k][0], (float)text_scen_xy[k][1]);
            glVertex2f((float)text_scen_xy[k][2], (float)text_scen_xy[k][3]);

            glTexCoord2f(0.0f, (float)text_scen_xy[k][1]);
            glVertex2f(0.0f, (float)text_scen_xy[k][3]);

            glEnd();

            glPopMatrix();
        }
    }

    //printf("\tUwzglednianie pogody ...\n");
    if (p.weather != wtNONE)
    {
        glBindTexture(GL_TEXTURE_2D, text_weath);

        glBegin(GL_QUADS);
        //glColor4ub(xprop[i].color.red, xprop[i].color.green, xprop[i].color.blue, xprop[i].alpha);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2f((float)text_weath_xy[0], 0.0f);
        glVertex2f((float)text_weath_xy[2], 0.0f);

        glTexCoord2f((float)text_weath_xy[0], (float)text_weath_xy[1]);
        glVertex2f((float)text_weath_xy[2], (float)text_weath_xy[3]);

        glTexCoord2f(0.0f, (float)text_weath_xy[1]);
        glVertex2f(0.0f, (float)text_weath_xy[3]);

        glEnd();
    }/*
    glVertex2f(PMS_FILE_leftoffs, -PMS_FILE_topoffs);
    glVertex2f(PMS_FILE_rightoffs, -PMS_FILE_topoffs);
    glColor4ub((GLubyte)BM_red, (GLubyte)BM_green, (GLubyte)BM_blue, (GLubyte)BM_alpha);
    glVertex2f(PMS_FILE_rightoffs, -PMS_FILE_Bottomoffs);
    glVertex2f(PMS_FILE_leftoffs, -PMS_FILE_Bottomoffs);*/

    glDisable(GL_TEXTURE_2D);

}

void WorldMap::draw_interface()
{
    float startx, starty;
    int i = 0;
    string xtmp, ans;
    ostringstream oss;

    glEnable(GL_TEXTURE_2D);


// ----- Health -----

    startx = (float)0.01*MAX_WIDTH;
    starty = (float)(MAX_HEIGHT-40);
    draw_int_help(text_health, text_health_xy, startx, starty);

    startx = (float)(0.01*MAX_WIDTH+40);
    starty = (float)(MAX_HEIGHT-30);
    text_healthbar_xy[2] = (GLfloat)115.0*bot[MY_BOT_NR]->procLife;
    draw_int_help(text_healthbar, text_healthbar_xy, startx, starty);


// ----- Gun's name -----

    startx = (float)(0.01*MAX_WIDTH+170);
    starty = (float)(MAX_HEIGHT-34);
    text_font = SDL_GL_LoadFont(font2_12, gun_name(bot[MY_BOT_NR]->gunModel), textYellow, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &texture_temp);


// ----- Ammo -----

    startx = (float)(0.46*MAX_WIDTH);
    starty = (float)(MAX_HEIGHT-44);
    draw_int_help(text_ammo, text_ammo_xy, startx, starty);

    // ilosc amunicji
    if (bot[MY_BOT_NR]->leftAmmos == 0)
    {
        bot[MY_BOT_NR]->isReloading = true;
        SDL_CreateThread(reload_gun, NULL);
    }
    oss << bot[MY_BOT_NR]->leftAmmos;

    startx = (float)(0.46*MAX_WIDTH+33);
    starty = (float)(MAX_HEIGHT-32);
    text_font = SDL_GL_LoadFont(font2_12, oss.str(), textYellow, text_font_xy);
    draw_int_help(text_font, text_font_xy, startx, starty);
    glDeleteTextures(1, &texture_temp);


    // ilosc granatow
    starty = (float)(MAX_HEIGHT-15);
    for (i = 0; i < bot[MY_BOT_NR]->numGrenades; i++)
    {
        draw_int_help(text_nade, text_nade_xy, startx, starty);
        startx += 10;
    }

    startx = (float)(0.46*MAX_WIDTH+67);
    starty = (float)(MAX_HEIGHT-34);
    text_reloadbar_xy[2] = (GLfloat)(115*bot[MY_BOT_NR]->leftAmmos / gun_ammos(bot[MY_BOT_NR]->gunModel));
    draw_int_help(text_reloadbar, text_reloadbar_xy, startx, starty);


// ----- Flying -----

    startx = (float)(0.76*MAX_WIDTH);
    starty = (float)(MAX_HEIGHT-44);
    draw_int_help(text_jet, text_jet_xy, startx, starty);

    startx = (float)(0.76*MAX_WIDTH+40);
    starty = (float)(MAX_HEIGHT-34);
    text_jetbar_xy[2] = (GLfloat)115.0*bot[MY_BOT_NR]->procJet;
    draw_int_help(text_jetbar, text_jetbar_xy, startx, starty);

    glDisable(GL_TEXTURE_2D);

}


void WorldMap::draw_infos()
{

    glEnable(GL_TEXTURE_2D);

    if (bot[MY_BOT_NR]->isKilled)
    {
        string killer_name = bot[bot[MY_BOT_NR]->killer]->name;
        Uint32 new_time;
        if ((new_time = SDL_GetTicks() - bot[MY_BOT_NR]->timerRespawnTime) <= bot[MY_BOT_NR]->respawnTime)
        {
            ostringstream oss;
            oss << (float)(bot[MY_BOT_NR]->respawnTime - new_time)/1000;
            text_font = SDL_GL_LoadFont(font2_12, "Respawn in "+oss.str(), textRed, text_font_xy);
            draw_int_help(text_font, text_font_xy, (float)(0.5*MAX_WIDTH-20), (float)(50));
            text_font = SDL_GL_LoadFont(font2_28, "Killed by "+killer_name, textRed, text_font_xy);
            draw_int_help(text_font, text_font_xy, (float)(0.5*MAX_WIDTH-80), (float)(MAX_HEIGHT-94));
            oss.str("");
        }
        else
        {
            bot[MY_BOT_NR]->isKilled = false;
        }

        // half second later (after kill) turn on CHOICE_GUN_MENU state
        if (new_time >= 500 && !CHOICE_GUN)
        {
            CHOICE_GUN_MENU = true;
        }
    }
    else
        CHOICE_GUN = false;


    glDisable(GL_TEXTURE_2D);

    if (CHOICE_GUN_MENU && !CHOICE_GUN)
    {
        int x, y;
        //SDL_ShowCursor(1);
        gun_menu();
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
            gun_menu_select(x, y);
        //SDL_ShowCursor(0);
    }

    if (CHOICE_ESC)
    {
        int x, y;
        //SDL_ShowCursor(1);
        exit_menu();
        if (SDL_GetMouseState(&x, &y)&SDL_BUTTON(1))
            exit_menu_select(x, y);
        //SDL_ShowCursor(0);
    }

}



void WorldMap::draw_mouse()
{
    int x, y;

    glEnable(GL_TEXTURE_2D);

    SDL_GetMouseState(&x, &y);

    glPushMatrix();
    glLoadIdentity();

    glTranslatef((GLfloat)(world->bgX + x), (GLfloat)(world->bgY + y), 0.0f);

    glBindTexture(GL_TEXTURE_2D, text_mouse);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f((float)text_mouse_xy[0], 0.0f);
    glVertex2f((float)text_mouse_xy[2], 0.0f);
    glTexCoord2f((float)text_mouse_xy[0], (float)text_mouse_xy[1]);
    glVertex2f((float)text_mouse_xy[2], (float)text_mouse_xy[3]);
    glTexCoord2f(0.0f, (float)text_mouse_xy[1]);
    glVertex2f(0.0f, (float)text_mouse_xy[3]);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}
