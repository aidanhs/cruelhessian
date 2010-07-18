/*   load.cpp
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
#include <ctime>
#include "boost/filesystem/fstream.hpp"
#include "regexp.h"
#include "WorldMap.h"
#include "TexturesLoader.h"
#include "WeaponManager.h"
#include "Mouse.h"
#include "Game.h"
#include "parser/SimpleIni.h"



void WorldMap::load_configs()
{
    std::cout << "Loading configs ..." << std::endl;

    /* aGravity = TVector2D(0.0f, 9.81f);
     aWalkingLeft = TVector2D(8.0f, 0.0f);
     aWalkingRight = TVector2D(-8.0f, 0.0f);
     aJumpUp = TVector2D(0.0f, -17.0f);
    */

    MAX_RESPAWN_TIME = 5;
    CHOICE_GUN = false;
    SHOW_GUN_MENU = true;
    SHOW_SCORES = false;
    SHOW_COMMAND_LINE = false;
    SHOW_MYCHAT_LINE = false;
    SHOW_ESC = false;
    SHOW_STATS = true;
    CHOICE_EXIT = false;
    FRIENDLY_FIRE = false;
    YOU_KILLED = false;
    prev_time = 0;

    JET_CHANGE = 0.0005f;
    sGravity = 9.81f;
    sDrag = 0.3f;
    sDragWalking = 300.0f;
    sWalking = 10.0f;
    sFlying = 1000.0f;
    sJumping = 20.0f;
    fullLife = 495.0f;
    _180overpi = 57.29f;

    DISTANCE_SCORE = MY_CURRENT_POS = 0;
    KEY_PRESSED = SDLK_UNKNOWN;
    currentFPS = 0;
    getCurrentTime = 0;

    mouse = new Mouse(text_mouse);
    chat = new Chat();
    window_guns = new WindowGuns();
    arrow = new Arrow(text_arrow);

}


void WorldMap::load_bonuses()
{
    std::cout << "Loading bonuses ..." << std::endl;

    bonus.resize(7);

    for (int i = 0; i < map->spawnpointCount; ++i)
    {
        if (map->spawnpoint[i].active)
        {
            if (map->spawnpoint[i].team == map->stBERSERKER && game.BONUSES[BONUS_BERSERKER])
                bonus[BONUS_BERSERKER].push_back(i);
            else if (map->spawnpoint[i].team == map->stCLUSTERS && game.BONUSES[BONUS_CLUSTERS])
                bonus[BONUS_CLUSTERS].push_back(i);
            else if (map->spawnpoint[i].team == map->stFLAMER && game.BONUSES[BONUS_FLAMER])
                bonus[BONUS_FLAMER].push_back(i);
            else if (map->spawnpoint[i].team == map->stGRENADES)
                bonus[BONUS_GRENADES].push_back(i);
            else if (map->spawnpoint[i].team == map->stMEDKITS)
                bonus[BONUS_MEDIKITS].push_back(i);
            else if (map->spawnpoint[i].team == map->stPREDATOR && game.BONUSES[BONUS_PREDATOR])
                bonus[BONUS_PREDATOR].push_back(i);
            else if (map->spawnpoint[i].team == map->stVEST && game.BONUSES[BONUS_VESTKIT])
                bonus[BONUS_VESTKIT].push_back(i);
        }
    }

    // laduj wszystko co jest
    // fix it !!!!!!!!!!!!
    TVector2D p;
    int num;
    for (unsigned int i = 0; i < 7; ++i)
    {
        if (game.BONUSES[i])
            for (unsigned int j = 0; j < bonus[i].size(); ++j)
            {
                num = bonus[i][j];
                p.x = static_cast<float>(map->spawnpoint[num].x);
                p.y = static_cast<float>(map->spawnpoint[num].y);
                Bonus *newbon = new Bonus(p, text_bonus[i], i);
                //Bonus newbon(p, text_bonus[i], i);
                bonus_list.push_back(newbon);
//            m_objects.push_back(newbon);
            }
    }


}


void WorldMap::load_spawnpoints()
{
    std::cout << "Loading spawnpoints ..." << std::endl;

    spawnpoint.resize(5);

    for (int i = 0; i < map->spawnpointCount; ++i)
    {
        if (map->spawnpoint[i].active)
        {
            if (map->spawnpoint[i].team == map->stGENERAL) spawnpoint[TEAM_GENERAL].push_back(i);
            else if (map->spawnpoint[i].team == map->stALPHA) spawnpoint[TEAM_ALPHA].push_back(i);
            else if (map->spawnpoint[i].team == map->stBRAVO) spawnpoint[TEAM_BRAVO].push_back(i);
            else if (map->spawnpoint[i].team == map->stCHARLIE) spawnpoint[TEAM_CHARLIE].push_back(i);
            else if (map->spawnpoint[i].team == map->stDELTA) spawnpoint[TEAM_DELTA].push_back(i);
        }
    }

}



int WorldMap::load_bots(unsigned int bot_count, TEAM team)
{

    if (bot_count == 0)
    {
        std::cerr << "   no bots assigned for " << team << " team" << std::endl;
        return 0;
    }

    /* losujemy boty, zeby byly niepowtarzalne w teamie (CTF, ...) lub w calej rozgrywce (DM, ...)
       (numery umieszczamy w unikalnej liscie - u nas vector).
       jesli liczba botow, ktore maja zostac dodane przekracza
       liczbe zaladowanych botow z dysku,
       to dodawaj nazwom botow liczbe od 10 do 99

       flist zawiera tyle elementow, ile wynosi liczba botow z kalatogu "Bots" */

    if (!spawnpoint[team].empty())
    {
        unsigned int b_cnt = 0, sol_nr, bot_nr;
        std::vector<unsigned int>::iterator it;
        std::vector<unsigned int> flist;
        std::ostringstream oss;

        while (b_cnt < bot_count)
        {

            for (unsigned int k = 0; k < Bots.element.size(); ++k)
            {
                flist.push_back(k);
            }

            while (b_cnt < bot_count && !flist.empty())
            {
                sol_nr = rand() % flist.size();
                bot_nr = addBot(Bots.element[flist[sol_nr]], spawnpoint[team][rand()%spawnpoint[team].size()], team);

                // check, if there is already bot with the same name
                for (unsigned int k = 0; k < bot.size(); ++k)
                {
                    if (bot[k]->name == bot[bot_nr]->name && bot[k]->team == bot[bot_nr]->team && k != bot_nr)
                    {
                        oss << rand() % 90 + 10;
                        bot[bot_nr]->name += " " + oss.str();
                        oss.str("");
                        break;
                    }
                }

                std::cout << "   assigning " << bot[bot_nr]->name << " to " << bot[bot_nr]->team << " team" << std::endl;

                it = flist.begin();
                it += sol_nr;
                flist.erase(it);

                b_cnt++;
            }
        }
        flist.clear();
    }
    else
    {
        std::cerr << "Cannot find spawnpoints for " << team << " team !" << std::endl;
        return -1;
    }

    return 0;
}




int WorldMap::load_audio()
{

    game.SOUNDS_VOL_INT = static_cast<int>(game.SOUNDS_VOL*MIX_MAX_VOLUME/100);
    music = NULL;

    if (game.SOUNDS_VOL + game.MUSIC_VOL > 0)
    {
        std::cout << "Loading audio ... " << std::endl;

        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            std::cout << "  Unable to init audio: " << SDL_GetError() << std::endl;
            return 1;
        }

        if (Mix_OpenAudio(game.AUDIO_QUAL, AUDIO_S16SYS, 1, 2048) != 0)
        {
            std::cout << "  Unable to initialize audio: " << Mix_GetError() << std::endl;
            return 1;
        }

        if (game.SOUNDS_VOL > 0)
        {

            if ((Weapons[0].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "colt1911-fire")) != NULL) Mix_VolumeChunk(Weapons[0].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[1].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "deserteagle-fire")) != NULL) Mix_VolumeChunk(Weapons[1].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[2].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "mp5-fire")) != NULL) Mix_VolumeChunk(Weapons[2].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[3].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "ak74-fire")) != NULL) Mix_VolumeChunk(Weapons[3].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[4].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "steyraug-fire")) != NULL) Mix_VolumeChunk(Weapons[4].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[5].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "spas12-fire")) != NULL) Mix_VolumeChunk(Weapons[5].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[6].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "ruger77-fire")) != NULL) Mix_VolumeChunk(Weapons[6].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[7].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "m79-fire")) != NULL) Mix_VolumeChunk(Weapons[7].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[8].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "barretm82-fire")) != NULL) Mix_VolumeChunk(Weapons[8].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[9].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "m249-fire")) != NULL) Mix_VolumeChunk(Weapons[9].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[10].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "minigun-fire")) != NULL) Mix_VolumeChunk(Weapons[10].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[11].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "flamer")) != NULL) Mix_VolumeChunk(Weapons[11].fireSound, game.SOUNDS_VOL_INT);
            if ((Weapons[15].fireSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "chainsaw-r")) != NULL) Mix_VolumeChunk(Weapons[15].fireSound, game.SOUNDS_VOL_INT);

            if ((Weapons[0].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "colt1911-reload")) != NULL) Mix_VolumeChunk(Weapons[0].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[1].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "deserteagle-reload")) != NULL) Mix_VolumeChunk(Weapons[1].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[2].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "mp5-reload")) != NULL) Mix_VolumeChunk(Weapons[2].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[3].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "ak74-reload")) != NULL) Mix_VolumeChunk(Weapons[3].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[4].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "steyraug-reload")) != NULL) Mix_VolumeChunk(Weapons[4].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[5].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "spas12-reload")) != NULL) Mix_VolumeChunk(Weapons[5].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[6].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "ruger77-reload")) != NULL) Mix_VolumeChunk(Weapons[6].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[7].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "m79-reload")) != NULL) Mix_VolumeChunk(Weapons[7].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[8].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "barretm82-reload")) != NULL) Mix_VolumeChunk(Weapons[8].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[9].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "m249-reload")) != NULL) Mix_VolumeChunk(Weapons[9].reloadSound, game.SOUNDS_VOL_INT);
            if ((Weapons[10].reloadSound = loadSoundFile2(game.SOL_PATH + "Sfx/", "minigun-reload")) != NULL) Mix_VolumeChunk(Weapons[10].reloadSound, game.SOUNDS_VOL_INT);

            if ((grenade_pullout = loadSoundFile2(game.SOL_PATH + "Sfx/", "grenade-pullout")) != NULL) Mix_VolumeChunk(grenade_pullout, game.SOUNDS_VOL_INT);
            if ((grenade_throw = loadSoundFile2(game.SOL_PATH + "Sfx/", "grenade-throw")) != NULL) Mix_VolumeChunk(grenade_throw, game.SOUNDS_VOL_INT);
            if ((grenade_bounce = loadSoundFile2(game.SOL_PATH + "Sfx/", "grenade-bounce")) != NULL) Mix_VolumeChunk(grenade_bounce, game.SOUNDS_VOL_INT);
            if ((grenade_explosion = loadSoundFile2(game.SOL_PATH + "Sfx/", "grenade-explosion")) != NULL) Mix_VolumeChunk(grenade_explosion, game.SOUNDS_VOL_INT);
            if ((sound_new_life = loadSoundFile2(game.SOL_PATH + "Sfx/", "wermusic")) != NULL) Mix_VolumeChunk(sound_new_life, game.SOUNDS_VOL_INT);
            if ((sound_death[0] = loadSoundFile2(game.SOL_PATH + "Sfx/", "death")) != NULL) Mix_VolumeChunk(sound_death[0], game.SOUNDS_VOL_INT);
            if ((sound_death[1] = loadSoundFile2(game.SOL_PATH + "Sfx/", "death2")) != NULL) Mix_VolumeChunk(sound_death[1], game.SOUNDS_VOL_INT);
            if ((sound_death[2] = loadSoundFile2(game.SOL_PATH + "Sfx/", "death3")) != NULL) Mix_VolumeChunk(sound_death[2], game.SOUNDS_VOL_INT);
            //if ((sound_kitfall[0] = loadSoundFile2(game.SOL_PATH + "Sfx/", "kit-fall")) != NULL) Mix_VolumeChunk(sound_kitfall[0], SOUNDS_VOL_INT);
            //if ((sound_kitfall[1] = loadSoundFile2(game.SOL_PATH + "Sfx/", "kit-fall2")) != NULL) Mix_VolumeChunk(sound_kitfall[1], SOUNDS_VOL_INT);
            if ((sound_heaven = loadSoundFile2(game.SOL_PATH + "Sfx/", "playerdeath")) != NULL) Mix_VolumeChunk(sound_heaven, game.SOUNDS_VOL_INT);
            if ((sound_spawn = loadSoundFile2(game.SOL_PATH + "Sfx/", "spawn")) != NULL) Mix_VolumeChunk(sound_spawn, game.SOUNDS_VOL_INT);
            if ((menu_click = loadSoundFile2(game.SOL_PATH + "Sfx/", "menuclick")) != NULL) Mix_VolumeChunk(menu_click, game.SOUNDS_VOL_INT);

            sound_kitfall[0] = NULL;
            sound_kitfall[1] = NULL;
        }

        if (game.MUSIC_VOL > 0 && !game.gMusicList.empty())
        {
            playMusic(0);
            Mix_VolumeMusic(static_cast<int>(game.MUSIC_VOL*MIX_MAX_VOLUME/100.0f));
        }
    }
    return 0;
}


void WorldMap::load_textures()
{
    std::cout << "Loading textures ... " << std::endl;

    std::cout << "   loading scenery textures ... " << std::endl;
    GLuint *text_scen = new GLuint[map->sceneryCount];
    std::vector<std::string> names(map->getTextScen());
    for (unsigned int i = 0; i < names.size(); ++i)
    {
        text_scen[i] = SOIL_LoadTexture2(game.SOL_PATH + "Scenery-gfx/", names[i]);
    }
    map->setScen(text_scen);

    std::cout << "   loading triangles texture ... " << std::endl;
    GLuint text_poly = SOIL_LoadTexture2(game.SOL_PATH + "Textures/", map->getTextPoly());
    map->setPoly(text_poly);

    std::cout << "   loading interface textures ... " << std::endl;
    text_arrow     = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "arrow");
    text_mouse     = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "cursor");
    text_health    = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "health");
    text_healthbar = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "health-bar");
    text_ammo      = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "ammo");
    text_nade      = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "nade");
    text_reloadbar = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "reload-bar");
    text_jet       = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "jet");
    text_jetbar    = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "jet-bar");
    text_firebar   = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "fire-bar");
    text_firebar_r = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "fire-bar-r");
    text_deaddot   = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "deaddot");
    text_smalldot  = SOIL_LoadTextureEx2(game.INTERFACE_PATH, "smalldot");

    std::cout << "   loading bonuses' textures ... " << std::endl;
    text_bonus[BONUS_BERSERKER] = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "berserkerkit");
    text_bonus[BONUS_CLUSTERS]  = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "clusterkit");
    text_bonus[BONUS_FLAMER]    = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "flamerkit");
    text_bonus[BONUS_GRENADES]  = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "grenadekit");
    text_bonus[BONUS_MEDIKITS]  = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "medikit");
    text_bonus[BONUS_PREDATOR]  = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "predatorkit");
    text_bonus[BONUS_VESTKIT]   = SOIL_LoadTextureEx2(game.SOL_PATH + "Textures/Objects/", "vestkit");





    std::cout << "   loading grenade's textures ... " << std::endl;
    text_grenade[0] = text_nade;
    std::ostringstream oss;
    std::string path;
    for (int i = 1; i < 16; ++i)
    {
        oss << i+1;
        text_grenade[i] = SOIL_LoadTextureEx2(game.SOL_PATH + "Sparks-gfx/explosion/", "explode" + oss.str());
        oss.str("");
    }

    std::cout << "   loading weather textures ... " << std::endl;
    if (map->weather != map->wtNONE)
    {
        if (map->weather == map->wtRAIN)
            text_weath = SOIL_LoadTextureEx2(game.SOL_PATH + "Sparks-gfx/", "rain");
        else if (map->weather == map->wtSANDSTORM)
            text_weath = SOIL_LoadTextureEx2(game.SOL_PATH + "Sparks-gfx/", "sand");
        else if (map->weather == map->wtSNOW)
            text_weath = SOIL_LoadTextureEx2(game.SOL_PATH + "Sparks-gfx/", "snow");
    }



}


int WorldMap::load_fonts()
{

    std::cout << "Loading fonts ... " << std::endl;

    //textColor[] = { 255, 255, 255, 0 };
    textColor[0] = 255;
    textColor[1] = 255;
    textColor[2] = 255;
    textColor[3] = 0;
    //textColorGunOnTouch[4] = { 255, 255, 255, 255 };
    textColorGunOnTouch[0] = 255;
    textColorGunOnTouch[1] = 255;
    textColorGunOnTouch[2] = 255;
    textColorGunOnTouch[3] = 255;
    //textColorGunNormal = { 200, 200, 200, 250 };
    textColorGunNormal[0] = 200;
    textColorGunNormal[1] = 200;
    textColorGunNormal[2] = 200;
    textColorGunNormal[3] = 250;
    //textGunColor = { 247, 247, 247, 250 };
    textGunColor[0] = 247;
    textGunColor[1] = 247;
    textGunColor[2] = 247;
    textGunColor[3] = 250;


// white, red, blue, yellow, green
    /*textCol[0] = {255, 255, 255, 0};
      textCol[1] = {255, 0, 0, 0};
      textCol[2] = {0, 0, 255, 0};
      textCol[3] = {255, 255, 0, 0};
      textCol[4] = {0, 255, 0, 0};
    */
    textCol[0][0] = 255;
    textCol[0][1] = 255;
    textCol[0][2] = 255;
    textCol[0][3] = 0;

    textCol[1][0] = 255;
    textCol[1][1] = 0;
    textCol[1][2] = 0;
    textCol[1][3] = 0;

    textCol[2][0] = 0;
    textCol[2][1] = 0;
    textCol[2][2] = 255;
    textCol[2][3] = 0;

    textCol[3][0] = 255;
    textCol[3][1] = 255;
    textCol[3][2] = 0;
    textCol[3][3] = 0;

    textCol[4][0] = 0;
    textCol[4][1] = 255;
    textCol[4][2] = 0;
    textCol[4][3] = 0;

    std::string tmp_str;
    CSimpleIni ini(false, false, false);
    if (ini.LoadFile((game.SOL_PATH + "Txt/font.ini").c_str()) < 0)
    {
        std::cerr << "Cannot read 'Txt/font.ini' file" << std::endl;
        return 1;
    }

    tmp_str = ini.GetValue("FONTS", "Font1");

    if (tmp_str == "Verdana")
    {
        Font[0] = "lucon.ttf";
    }
    else if (tmp_str == "Lucida Console")
    {
        Font[0] = "lucon.ttf";
    }
    else if (tmp_str == "BankGothic Lt BT")
    {
        Font[0] = "bgothl.ttf";
    }
    else if (tmp_str == "Arial")
    {
        Font[0] = "arial.ttf";
    }
    else if (tmp_str == "FreeSans")
    {
        Font[0] = "FreeSans.ttf";
    }

    tmp_str = ini.GetValue("FONTS", "Font2");
    if (tmp_str == "Verdana")
    {
        Font[1] = "lucon.ttf";
    }
    else if (tmp_str == "Lucida Console")
    {
        Font[1] = "lucon.ttf";
    }
    else if (tmp_str == "BankGothic Lt BT")
    {
        Font[1] = "bgothl.ttf";
    }
    else if (tmp_str == "Arial")
    {
        Font[1] = "arial.ttf";
    }
    else if (tmp_str == "FreeSans")
    {
        Font[1] = "FreeSans.ttf";
    }

    game.FontMenuSize = ini.GetLongValue("FONTS", "FontMenuSize");
    game.FontConsoleSize = ini.GetLongValue("FONTS", "FontConsoleSize");
    game.FontBigSize = ini.GetLongValue("FONTS", "FontBigSize");
    game.FontWeaponMenuSize = ini.GetLongValue("FONTS", "FontWeaponMenuSize");
    game.FontConsoleSmallSize = ini.GetLongValue("FONTS", "FontConsoleSmallSize");
    game.FontHeightScale = ini.GetLongValue("FONTS", "FontHeightScale");
    game.FontMenuBold = ini.GetLongValue("FONTS", "FontMenuBold");
    game.FontConsoleBold = ini.GetLongValue("FONTS", "FontConsoleBold");
    game.FontBigBold = ini.GetLongValue("FONTS", "FontBigBold");
    game.FontWeaponMenuBold = ini.GetLongValue("FONTS", "FontWeaponMenuBold");
    game.FontConsoleSmallBold = ini.GetLongValue("FONTS", "FontConsoleSmallBold");
    game.KillConsoleNameSpace = ini.GetLongValue("FONTS", "KillConsoleNameSpace");

    for (int i = 0; i < 2; ++i)
    {
        font[i][game.FontMenuSize].init((game.SOL_PATH + Font[i]).c_str(), game.FontMenuSize);
        font[i][game.FontConsoleSize].init((game.SOL_PATH + Font[i]).c_str(), game.FontConsoleSize);
        font[i][game.FontBigSize].init((game.SOL_PATH + Font[i]).c_str(), game.FontBigSize);
        font[i][game.FontWeaponMenuSize].init((game.SOL_PATH + Font[i]).c_str(), game.FontWeaponMenuSize);
        font[i][game.FontConsoleSmallSize].init((game.SOL_PATH + Font[i]).c_str(), game.FontConsoleSmallSize);
    }


    // font1_16.init((game.SOL_PATH + Font1).c_str(), 12);
    // font2_12.init((game.SOL_PATH + Font2).c_str(), 10);
    // font2_28.init((game.SOL_PATH + Font2).c_str(), 26);

//    font1_16 = new OGLFT::Monochrome((game.SOL_PATH + Font1).c_str(), 14);
//    font2_12 = new OGLFT::Monochrome((game.SOL_PATH + Font2).c_str(), 10);
//    font2_28 = new OGLFT::Monochrome((game.SOL_PATH + Font2).c_str(), 28);

    return 0;

}


int WorldMap::load_map(const std::string& name)
{
    std::cout << "Loading map file ... " << std::endl;

    if (boost::filesystem::exists(name))
    {
        map = new Map(name);
        return 0;
    }
    else
    {
        std::cerr << "Cannot find map file : " << name << std::endl;
        exit(1);
    }
}

