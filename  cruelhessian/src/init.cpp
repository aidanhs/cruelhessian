/*   init.cpp
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

#include <sstream>
#include "draw_extra.h"
#include "worldmap.h"
#include "globals.h"


void WorldMap::init_gl()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, MAX_WIDTH, MAX_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(this->bgX, this->bgX+MAX_WIDTH, this->bgY+MAX_HEIGHT, this->bgY, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}


void WorldMap::oblicz_min_max()
{
    for (int i = 0; i < p.polygonCount; i++)
    {
        // minima i maksima trojkata
        tr_maxx[i] = tr_minx[i] = p.polygon[i].vertex[0].x;
        tr_maxy[i] = tr_miny[i] = p.polygon[i].vertex[0].y;

        if (p.polygon[i].vertex[1].x > tr_maxx[i]) tr_maxx[i] = p.polygon[i].vertex[1].x;
        if (p.polygon[i].vertex[1].x < tr_minx[i]) tr_minx[i] = p.polygon[i].vertex[1].x;
        if (p.polygon[i].vertex[1].y > tr_maxy[i]) tr_maxy[i] = p.polygon[i].vertex[1].y;
        if (p.polygon[i].vertex[1].y < tr_miny[i]) tr_miny[i] = p.polygon[i].vertex[1].y;

        if (p.polygon[i].vertex[2].x > tr_maxx[i]) tr_maxx[i] = p.polygon[i].vertex[2].x;
        if (p.polygon[i].vertex[2].x < tr_minx[i]) tr_minx[i] = p.polygon[i].vertex[2].x;
        if (p.polygon[i].vertex[2].y > tr_maxy[i]) tr_maxy[i] = p.polygon[i].vertex[2].y;
        if (p.polygon[i].vertex[2].y < tr_miny[i]) tr_miny[i] = p.polygon[i].vertex[2].y;

        aa[i][0] = p.polygon[i].perpendicular[0].y / p.polygon[i].perpendicular[0].x;
        aa[i][1] = p.polygon[i].perpendicular[1].y / p.polygon[i].perpendicular[1].x;
        aa[i][2] = p.polygon[i].perpendicular[2].y / p.polygon[i].perpendicular[2].x;

        con[i][0] = 1 / (aa[i][0] * aa[i][0] + 1);
        con[i][1] = 1 / (aa[i][1] * aa[i][1] + 1);
        con[i][2] = 1 / (aa[i][2] * aa[i][2] + 1);
    }
}


void WorldMap::load_textures()
{
    int i;

    // Loading scenery textures
    text_scen = new GLuint[p.sceneryCount];
    text_scen_xy = new GLfloat *[p.sceneryCount];

    for (i = 0; i < p.sceneryCount; i++)
    {
        text_scen_xy[i] = new GLfloat[4];
        text_scen[i] = SDL_GL_LoadTexture(SOL_PATH+"Scenery-gfx/" + p.scenery[i].name, text_scen_xy[i]);
    }

    // Loading triangles textures
    text_poly = SDL_GL_LoadTexture(SOL_PATH+"Textures/" + p.texture, text_poly_xy);

    // Loading mouse pointer textures
    text_mouse = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/cursor.bmp", text_mouse_xy);

    // Loading iterface textures
    text_health = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/health.bmp", text_health_xy);
    text_healthbar = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/health-bar.bmp", text_healthbar_xy);
    text_ammo = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/ammo.bmp", text_ammo_xy);
    text_nade = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/nade.bmp", text_nade_xy);
    text_reloadbar = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/reload-bar.bmp", text_reloadbar_xy);
    text_jet = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/jet.bmp", text_jet_xy);
    text_jetbar = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/jet-bar.bmp", text_jetbar_xy);

    // Loading guns textures
    text_0 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/0.bmp", text_0_xy);
    text_1 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/1.bmp", text_1_xy);
    text_2 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/2.bmp", text_2_xy);
    text_3 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/3.bmp", text_3_xy);
    text_4 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/4.bmp", text_4_xy);
    text_5 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/5.bmp", text_5_xy);
    text_6 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/6.bmp", text_6_xy);
    text_7 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/7.bmp", text_7_xy);
    text_8 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/8.bmp", text_8_xy);
    text_9 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/9.bmp", text_9_xy);
    text_10 = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/10.bmp", text_10_xy);
    text_knife = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/knife.bmp", text_knife_xy);
    text_chain = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/chainsaw.bmp", text_chain_xy);
    text_law = SDL_GL_LoadTexture(SOL_PATH+"Interface-gfx/Guns/law.bmp", text_law_xy);

    // Loading bullets textures
    text_ammos[0] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/minigun-bullet.bmp", text_ammos_xy[0]);
    AMMO_WIDTH[0] = text_ammos_xy[0][2]; AMMO_HEIGHT[0] = text_ammos_xy[0][3];
    text_ammos[1] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/eagles-bullet.bmp", text_ammos_xy[1]);
    AMMO_WIDTH[1] = text_ammos_xy[1][2]; AMMO_HEIGHT[1] = text_ammos_xy[1][3];
    text_ammos[2] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/ak74-bullet.bmp", text_ammos_xy[2]);
    AMMO_WIDTH[2] = text_ammos_xy[2][2]; AMMO_HEIGHT[2] = text_ammos_xy[2][3];
    text_ammos[3] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/ak74-bullet.bmp", text_ammos_xy[3]);
    AMMO_WIDTH[3] = text_ammos_xy[3][2]; AMMO_HEIGHT[3] = text_ammos_xy[3][3];
    text_ammos[4] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/steyraug-bullet.bmp", text_ammos_xy[4]);
    AMMO_WIDTH[4] = text_ammos_xy[4][2]; AMMO_HEIGHT[4] = text_ammos_xy[4][3];
    text_ammos[5] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/spas12-bullet.bmp", text_ammos_xy[5]);
    AMMO_WIDTH[5] = text_ammos_xy[5][2]; AMMO_HEIGHT[5] = text_ammos_xy[5][3];
    text_ammos[6] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/ruger77-bullet.bmp", text_ammos_xy[6]);
    AMMO_WIDTH[6] = text_ammos_xy[6][2]; AMMO_HEIGHT[6] = text_ammos_xy[6][3];
    text_ammos[7] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/m79-bullet.bmp", text_ammos_xy[7]);
    AMMO_WIDTH[7] = text_ammos_xy[7][2]; AMMO_HEIGHT[7] = text_ammos_xy[7][3];
    text_ammos[8] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/barretm82-bullet.bmp", text_ammos_xy[8]);
    AMMO_WIDTH[8] = text_ammos_xy[8][2]; AMMO_HEIGHT[8] = text_ammos_xy[8][3];
    text_ammos[9] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/spas12-bullet.bmp", text_ammos_xy[9]);
    AMMO_WIDTH[9] = text_ammos_xy[9][2]; AMMO_HEIGHT[9] = text_ammos_xy[9][3];
    text_ammos[10] = SDL_GL_LoadTexture(SOL_PATH+"Weapons-gfx/spas12-bullet.bmp", text_ammos_xy[10]);
    AMMO_WIDTH[10] = text_ammos_xy[10][2]; AMMO_HEIGHT[10] = text_ammos_xy[10][3];

    // Loading grenades textures
    ostringstream oss;
    for (i = 0; i < 16; i++)
    {
        //oss << i+1;
        //ostringstream oss;
        oss << i+1;
        text_grenade[i] = SDL_GL_LoadTexture(SOL_PATH+"Sparks-gfx/explosion/explode"+oss.str()+".bmp", text_grenade_xy[i]);
        oss.str("");
    }

    // Weather
    if (p.weather != wtNONE)
    {
        switch (p.weather)
        {
        case wtRAIN :
            text_weath = SDL_GL_LoadTexture(SOL_PATH+"Sparks-gfx/rain.bmp", text_weath_xy);
            break;
        case wtSANDSTORM :
            text_weath = SDL_GL_LoadTexture(SOL_PATH+"Sparks-gfx/sand.bmp", text_weath_xy);
            break;
        case wtSNOW :
            text_weath = SDL_GL_LoadTexture(SOL_PATH+"Sparks-gfx/snow.bmp", text_weath_xy);
        default :
            break;
        }
    }

    // Gostek
    gost_stopa = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/stopa.bmp", gost_stopa_xy);
    gost_klata = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/klata.bmp", gost_klata_xy);
    gost_ramie = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/ramie.bmp", gost_ramie_xy);
    gost_morda = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/morda.bmp", gost_morda_xy);
    gost_reka = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/reka.bmp", gost_reka_xy);
    gost_dlon = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/dlon.bmp", gost_dlon_xy);
    gost_biodro = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/biodro.bmp", gost_biodro_xy);
    gost_udo = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/udo.bmp", gost_udo_xy);
    gost_noga = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/noga.bmp", gost_noga_xy);
    gost_x = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/noga.bmp", gost_x_xy);

}
