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
#include "regex.h"
#include <dirent.h>

#include "worldmap.h"
#include "parser/SimpleIni.h"



void WorldMap::load_weapons()
{
    std::cout << "Loading weapons ..." << std::endl;

    CSimpleIni ini(false, false, false);

    if (ini.LoadFile((SOL_PATH + "weapons.ini").c_str()) < 0)
    {
        std::cout << "Cannot read 'weapons.ini' file" << std::endl;
        return;
    }

    weapon[0].name = "USSOCOM";
    weapon[1].name = "Desert Eagles";
    weapon[2].name = "HK MP5";
    weapon[3].name = "Ak-74";
    weapon[4].name = "Steyr AUG";
    weapon[5].name = "Spas-12";
    weapon[6].name = "Ruger 77";
    weapon[7].name = "M79";
    weapon[8].name = "Barret M82A1";
    weapon[9].name = "FN Minimi";
    weapon[10].name = "XM214 Minigun";
    weapon[11].name = "Flamer";
    weapon[12].name = "Rambo Bow";
    weapon[13].name = "Flamed Arrows";
    weapon[14].name = "Combat Knife";
    weapon[15].name = "Chainsaw";
    weapon[16].name = "M72 LAW";
    weapon[17].name = "Punch";
    weapon[18].name = "Grenade";
    weapon[19].name = "Stationary Gun";

    /*
    CSimpleIniA::TNamesDepend l;
    ini.GetAllSections(l);

    std::vector<std::string> sect_vec;
    for (std::list<CSimpleIni::Entry>::iterator i = l.begin(); i != l.end(); ++i)
        sect_vec.push_back(i->pItem);
    */
    for (int i = 0; i < 20; ++i)
    {
        //weapon[i].name = sect_vec[i+1];
        weapon[i].damage = ini.GetLongValue(weapon[i].name.c_str(), "Damage");
        weapon[i].fireInterval = static_cast<Uint32>(16.67 * ini.GetLongValue(weapon[i].name.c_str(), "FireInterval"));
        weapon[i].ammo = ini.GetLongValue(weapon[i].name.c_str(), "Ammo");
        weapon[i].reloadTime = static_cast<Uint32>(16.67 * ini.GetLongValue(weapon[i].name.c_str(), "ReloadTime"));
        weapon[i].speed = ini.GetLongValue(weapon[i].name.c_str(), "Speed");
        weapon[i].bulletStyle = ini.GetLongValue(weapon[i].name.c_str(), "BulletStyle");
        weapon[i].startUpTime = 16.67f * ini.GetLongValue(weapon[i].name.c_str(), "StartUpTime");
        weapon[i].bink = 16.67f * ini.GetLongValue(weapon[i].name.c_str(), "Bink");
        weapon[i].movementAcc = ini.GetLongValue(weapon[i].name.c_str(), "MovementAcc");
        weapon[i].recoil = ini.GetLongValue(weapon[i].name.c_str(), "Recoil");
    }
}

void WorldMap::load_configs()
{
    std::cout << "Loading configs ..." << std::endl;

    /* aGravity = TVector2D(0.0f, 9.81f);
     aWalkingLeft = TVector2D(8.0f, 0.0f);
     aWalkingRight = TVector2D(-8.0f, 0.0f);
     aJumpUp = TVector2D(0.0f, -17.0f);
    */

    JET_CHANGE = 0.001f;
    MAX_RESPAWN_TIME = 5;
    GUN_MENU_START_X = static_cast<float>(MAX_WIDTH/4);
    GUN_MENU_START_Y = static_cast<float>(MAX_HEIGHT/7);
    CHOICE_GUN = false;
    SHOW_GUN_MENU = true;
    SHOW_SCORES = false;
    SHOW_ESC = false;
    CHOICE_EXIT = false;
    SHOW_STATS = false;
    FRIENDLY_FIRE = false;
    YOU_KILLED = true;
    REL_MOUSE_X = 0;
    REL_MOUSE_Y = 0;

    fTimeStepMS = 5;
    fTimeStep = fTimeStepMS / 1000;
    sGravity = -9.81f;
    sDrag = 1;
    sDragWalking = 300;
    sWalking = 80;
    sFlying = 300.0;
    sJumping = 200.0;

    calc_min_max();

    column_names.push_back("Player:");
    column_names.push_back("Points:");
    column_names.push_back("Deaths:");
    //column_names.push_back("Deathsdsd:");
    //column_names.push_back("Deathssd:");
}



void WorldMap::load_spawnpoints()
{
    std::cout << "Loading spawnpoints ..." << std::endl;

    spawnpoint.resize(4);
    if (p.spawnpoint[2].team == p.stALPHA) {}

    for (int i = 0; i < p.spawnpointCount; ++i)
    {
        if (p.spawnpoint[i].active)
        {
            if (p.spawnpoint[i].team == p.stALPHA) spawnpoint[TEAM_ALPHA].push_back(i);
            else if (p.spawnpoint[i].team == p.stBRAVO) spawnpoint[TEAM_BRAVO].push_back(i);
            else if (p.spawnpoint[i].team == p.stCHARLIE) spawnpoint[TEAM_CHARLIE].push_back(i);
            else if (p.spawnpoint[i].team == p.stDELTA) spawnpoint[TEAM_DELTA].push_back(i);
        }
    }
}



int WorldMap::load_bots(int alpha_cnt, int bravo_cnt, int charlie_cnt, int delta_cnt)
{
    std::cout << "Loading bots ..." << std::endl;

    int il_al = 0, il_br = 0, il_ch = 0, il_del = 0;
    struct dirent *dp;
    std::vector<Bots> botlist;
    regex_t preg;
    regcomp(&preg, ".+.(bot)$", REG_EXTENDED);
    std::string fold = SOL_PATH + "Bots/";
    DIR *dirp;
    CSimpleIni ini(false, false, false);

    srand(static_cast<unsigned int>(time(0)));

    // add me to alpha team

    MY_BOT_NR = static_cast<unsigned int>(bot.size());
    int point = rand()%spawnpoint[TEAM_ALPHA].size();
    addBot("Major", spawnpoint[TEAM_ALPHA][point], 3, TEAM_ALPHA, MY_BOT_NR);

    bgX = static_cast<float>(p.spawnpoint[spawnpoint[TEAM_ALPHA][point]].x - MAX_WIDTH/2);
    bgY = static_cast<float>(p.spawnpoint[spawnpoint[TEAM_ALPHA][point]].y - MAX_HEIGHT/2);


    // reading Bots directory

    dirp = opendir(fold.c_str());
    if (dirp == NULL)
    {
        std::cout << "'Bots' directory doesn't exist !" << std::endl;
        closedir(dirp);
        regfree(&preg);
        return 1;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        if (regexec(&preg, dp->d_name, 0, NULL, 0) == 0)
        {
            Bots temp;
            if (ini.LoadFile((fold + dp->d_name).c_str()) < 0)
            {
                std::cout << "Error in parsing " << fold + dp->d_name << std::endl;
                return 1;
            }
            temp.name = ini.GetValue("BOT", "Name");
            temp.chatKill = ini.GetValue("BOT", "Chat_Kill");
            temp.chatDead = ini.GetValue("BOT", "Chat_Dead");
            temp.chatLowhealth = ini.GetValue("BOT", "Chat_Lowhealth");
            temp.chatSeeEnemy = ini.GetValue("BOT", "Chat_SeeEnemy");
            temp.chatWinning = ini.GetValue("BOT", "Chat_Winning");

            botlist.push_back(temp);
        }
    }
    if (botlist.size() == 0)
    {
        std::cout << "'Bots' directory hasn't *.bot files !" << std::endl;
        return 1;
    }

    regfree(&preg);
    closedir(dirp);


    while (il_al < alpha_cnt)
    {
        //addBot(botlist[rand()%botlist.size()].name, spawnpoint[TEAM_ALPHA][rand()%spawnpoint[TEAM_ALPHA].size()], 3, TEAM_ALPHA, static_cast<unsigned int>(bot.size()));
        addBot(botlist[rand()%botlist.size()], spawnpoint[TEAM_ALPHA][rand()%spawnpoint[TEAM_ALPHA].size()], TEAM_ALPHA, static_cast<unsigned int>(bot.size()));
        il_al++;
    }
    while (il_br < bravo_cnt)
    {
        //addBot(botlist[rand()%botlist.size()].name, spawnpoint[TEAM_BRAVO][rand()%spawnpoint[TEAM_BRAVO].size()], 3, TEAM_BRAVO, static_cast<unsigned int>(bot.size()));
        addBot(botlist[rand()%botlist.size()], spawnpoint[TEAM_BRAVO][rand()%spawnpoint[TEAM_BRAVO].size()], TEAM_BRAVO, static_cast<unsigned int>(bot.size()));
        il_br++;
    }
    while (il_ch < charlie_cnt)
    {
        //addBot(botlist[rand()%botlist.size()].name, spawnpoint[TEAM_CHARLIE][rand()%spawnpoint[TEAM_CHARLIE].size()], 3, TEAM_CHARLIE, static_cast<unsigned int>(bot.size()));
        addBot(botlist[rand()%botlist.size()], spawnpoint[TEAM_CHARLIE][rand()%spawnpoint[TEAM_CHARLIE].size()], TEAM_CHARLIE, static_cast<unsigned int>(bot.size()));
        il_ch++;
    }
    while (il_del < delta_cnt)
    {
        //addBot(botlist[rand()%botlist.size()].name, spawnpoint[TEAM_DELTA][rand()%spawnpoint[TEAM_DELTA].size()], 3, TEAM_DELTA, static_cast<unsigned int>(bot.size()));
        addBot(botlist[rand()%botlist.size()], spawnpoint[TEAM_DELTA][rand()%spawnpoint[TEAM_DELTA].size()], TEAM_DELTA, static_cast<unsigned int>(bot.size()));
        il_del++;
    }
    return 0;
}


void WorldMap::load_animations()
{
    std::cout << "Loading animations ... " << std::endl;

    read_poa(STOI);
    read_poa(RZUCA);
    read_poa(BIEGA);
    read_poa(BIEGA_TYL);
    read_poa(CIESZY);
    read_poa(KUCA);
    read_poa(SKOK);
    read_poa(SKOK_DOL_OBROT);
    read_poa(SKOK_DOL_OBROT_TYL);
    read_poa(SKOK_W_BOK);
    read_poa(GORA);
    read_poa(ZMIEN_BRON);

}


int WorldMap::load_audio()
{
    SOUNDS_VOL_INT = static_cast<int>(SOUNDS_VOL*MIX_MAX_VOLUME/100);
    music = NULL;

    if (SOUNDS_VOL + MUSIC_VOL > 0)
    {
        std::cout << "Loading audio ... " << std::endl;

        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            std::cout << "  Unable to init audio: " << SDL_GetError() << std::endl;
            return 1;
        }

        if (Mix_OpenAudio(AUDIO_QUAL, AUDIO_S16SYS, 1, 2048) != 0)
        {
            std::cout << "  Unable to initialize audio: " << Mix_GetError() << std::endl;
            return 1;
        }

        if (SOUNDS_VOL > 0)
        {
            if ((weapon[0].fireSound = loadSoundFile(SOL_PATH + "Sfx/colt1911-fire.wav")) != NULL) Mix_VolumeChunk(weapon[0].fireSound, SOUNDS_VOL_INT);
            if ((weapon[1].fireSound = loadSoundFile(SOL_PATH + "Sfx/deserteagle-fire.wav")) != NULL) Mix_VolumeChunk(weapon[1].fireSound, SOUNDS_VOL_INT);
            if ((weapon[2].fireSound = loadSoundFile(SOL_PATH + "Sfx/mp5-fire.wav")) != NULL) Mix_VolumeChunk(weapon[2].fireSound, SOUNDS_VOL_INT);
            if ((weapon[3].fireSound = loadSoundFile(SOL_PATH + "Sfx/ak74-fire.wav")) != NULL) Mix_VolumeChunk(weapon[3].fireSound, SOUNDS_VOL_INT);
            if ((weapon[4].fireSound = loadSoundFile(SOL_PATH + "Sfx/steyraug-fire.wav")) != NULL) Mix_VolumeChunk(weapon[4].fireSound, SOUNDS_VOL_INT);
            if ((weapon[5].fireSound = loadSoundFile(SOL_PATH + "Sfx/spas12-fire.wav")) != NULL) Mix_VolumeChunk(weapon[5].fireSound, SOUNDS_VOL_INT);
            if ((weapon[6].fireSound = loadSoundFile(SOL_PATH + "Sfx/ruger77-fire.wav")) != NULL) Mix_VolumeChunk(weapon[6].fireSound, SOUNDS_VOL_INT);
            if ((weapon[7].fireSound = loadSoundFile(SOL_PATH + "Sfx/m79-fire.wav")) != NULL) Mix_VolumeChunk(weapon[7].fireSound, SOUNDS_VOL_INT);
            if ((weapon[8].fireSound = loadSoundFile(SOL_PATH + "Sfx/barretm82-fire.wav")) != NULL) Mix_VolumeChunk(weapon[8].fireSound, SOUNDS_VOL_INT);
            if ((weapon[9].fireSound = loadSoundFile(SOL_PATH + "Sfx/m249-fire.wav")) != NULL) Mix_VolumeChunk(weapon[9].fireSound, SOUNDS_VOL_INT);
            if ((weapon[10].fireSound = loadSoundFile(SOL_PATH + "Sfx/minigun-fire.wav")) != NULL) Mix_VolumeChunk(weapon[10].fireSound, SOUNDS_VOL_INT);
            if ((weapon[11].fireSound = loadSoundFile(SOL_PATH + "Sfx/flamer.wav")) != NULL) Mix_VolumeChunk(weapon[11].fireSound, SOUNDS_VOL_INT);
            if ((weapon[15].fireSound = loadSoundFile(SOL_PATH + "Sfx/chainsaw-r.WAV")) != NULL) Mix_VolumeChunk(weapon[15].fireSound, SOUNDS_VOL_INT);

            if ((weapon[0].reloadSound = loadSoundFile(SOL_PATH + "Sfx/colt1911-reload.wav")) != NULL) Mix_VolumeChunk(weapon[0].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[1].reloadSound = loadSoundFile(SOL_PATH + "Sfx/deserteagle-reload.wav")) != NULL) Mix_VolumeChunk(weapon[1].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[2].reloadSound = loadSoundFile(SOL_PATH + "Sfx/mp5-reload.wav")) != NULL) Mix_VolumeChunk(weapon[2].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[3].reloadSound = loadSoundFile(SOL_PATH + "Sfx/ak74-reload.wav")) != NULL) Mix_VolumeChunk(weapon[3].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[4].reloadSound = loadSoundFile(SOL_PATH + "Sfx/steyraug-reload.wav")) != NULL) Mix_VolumeChunk(weapon[4].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[5].reloadSound = loadSoundFile(SOL_PATH + "Sfx/spas12-reload.wav")) != NULL) Mix_VolumeChunk(weapon[5].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[6].reloadSound = loadSoundFile(SOL_PATH + "Sfx/ruger77-reload.wav")) != NULL) Mix_VolumeChunk(weapon[6].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[7].reloadSound = loadSoundFile(SOL_PATH + "Sfx/m79-reload.wav")) != NULL) Mix_VolumeChunk(weapon[7].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[8].reloadSound = loadSoundFile(SOL_PATH + "Sfx/barretm82-reload.wav")) != NULL) Mix_VolumeChunk(weapon[8].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[9].reloadSound = loadSoundFile(SOL_PATH + "Sfx/m249-reload.wav")) != NULL) Mix_VolumeChunk(weapon[9].reloadSound, SOUNDS_VOL_INT);
            if ((weapon[10].reloadSound = loadSoundFile(SOL_PATH + "Sfx/minigun-reload.wav")) != NULL) Mix_VolumeChunk(weapon[10].reloadSound, SOUNDS_VOL_INT);

            if ((grenade_throw = loadSoundFile(SOL_PATH + "Sfx/grenade-throw.wav")) != NULL) Mix_VolumeChunk(grenade_throw, SOUNDS_VOL_INT);
            if ((grenade_explosion = loadSoundFile(SOL_PATH + "Sfx/grenade-explosion.wav")) != NULL) Mix_VolumeChunk(grenade_explosion, SOUNDS_VOL_INT);
            if ((sound_new_life = loadSoundFile(SOL_PATH + "Sfx/wermusic.wav")) != NULL) Mix_VolumeChunk(sound_new_life, SOUNDS_VOL_INT);
            if ((sound_death[0] = loadSoundFile(SOL_PATH + "Sfx/death.wav")) != NULL) Mix_VolumeChunk(sound_death[0], SOUNDS_VOL_INT);
            if ((sound_death[1] = loadSoundFile(SOL_PATH + "Sfx/death2.wav")) != NULL) Mix_VolumeChunk(sound_death[1], SOUNDS_VOL_INT);
            if ((sound_death[2] = loadSoundFile(SOL_PATH + "Sfx/death3.wav")) != NULL) Mix_VolumeChunk(sound_death[2], SOUNDS_VOL_INT);
            if ((sound_heaven = loadSoundFile(SOL_PATH + "Sfx/playerdeath.wav")) != NULL) Mix_VolumeChunk(sound_heaven, SOUNDS_VOL_INT);
            if ((sound_spawn = loadSoundFile(SOL_PATH + "Sfx/spawn.wav")) != NULL) Mix_VolumeChunk(sound_spawn, SOUNDS_VOL_INT);
            if ((menu_click = loadSoundFile(SOL_PATH + "Sfx/menuclick.wav")) != NULL) Mix_VolumeChunk(menu_click, SOUNDS_VOL_INT);
        }

        if (MUSIC_VOL > 0 && !gMusicList.empty())
        {
            playMusic(0);
            Mix_VolumeMusic(static_cast<int>(MUSIC_VOL*MIX_MAX_VOLUME/100.0f));
        }
    }
    return 0;
}

void WorldMap::load_textures()
{
    std::cout << "Loading textures ... " << std::endl;

    // Loading scenery textures
    text_scen = new GLuint[p.sceneryCount];

    for (int i = 0; i < p.sceneryCount; i++)
    {
        text_scen[i] = SOIL_LoadTexture(SOL_PATH + "Scenery-gfx/" + p.scenery[i].name);
    }

    // Loading triangles textures
    text_poly = SOIL_LoadTexture(SOL_PATH + "Textures/" + p.texture);

    // Loading mouse pointer textures
    text_mouse = SOIL_LoadTextureEx(findInterface("cursor.bmp"));

    // Loading interface textures
    text_health     = SOIL_LoadTextureEx(findInterface("health.bmp"));
    text_healthbar  = SOIL_LoadTextureEx(findInterface("health-bar.bmp"));
    text_ammo       = SOIL_LoadTextureEx(findInterface("ammo.bmp"));
    text_nade       = SOIL_LoadTextureEx(findInterface("nade.bmp"));
    text_reloadbar  = SOIL_LoadTextureEx(findInterface("reload-bar.bmp"));
    text_jet        = SOIL_LoadTextureEx(findInterface("jet.bmp"));
    text_jetbar     = SOIL_LoadTextureEx(findInterface("jet-bar.bmp"));
    text_firebar    = SOIL_LoadTextureEx(findInterface("fire-bar.bmp"));
    text_firebar_r  = SOIL_LoadTextureEx(findInterface("fire-bar-r.bmp"));

    // Loading guns' textures
    weapon[0].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/0.bmp");
    weapon[1].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/1.bmp");
    weapon[2].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/2.bmp");
    weapon[3].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/3.bmp");
    weapon[4].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/4.bmp");
    weapon[5].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/5.bmp");
    weapon[6].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/6.bmp");
    weapon[7].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/7.bmp");
    weapon[8].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/8.bmp");
    weapon[9].textureGun   = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/9.bmp");
    weapon[10].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/10.bmp");
    weapon[14].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/knife.bmp");
    weapon[15].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/chainsaw.bmp");
    weapon[16].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/law.bmp");

    // Loading bullets' textures
    weapon[0].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/minigun-bullet.bmp");
    weapon[1].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/eagles-bullet.bmp");
    weapon[2].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/ak74-bullet.bmp");
    weapon[3].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/ak74-bullet.bmp");
    weapon[4].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/steyraug-bullet.bmp");
    weapon[5].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/spas12-bullet.bmp");
    weapon[6].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/ruger77-bullet.bmp");
    weapon[7].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/m79-bullet.bmp");
    weapon[8].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/barretm82-bullet.bmp");
    weapon[9].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/spas12-bullet.bmp");
    weapon[10].textureAmmo = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/spas12-bullet.bmp");

    // Loading grenade explosion textures
    std::ostringstream oss;
    for (int i = 0; i < 15; i++)
    {
        oss << i+1;
        text_grenade[i] = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/explosion/explode" + oss.str() + ".bmp");
        oss.str("");
    }

    // Loading weather texture
    if (p.weather != p.wtNONE)
    {
        /*switch (p.weather)
        {
        case p.wtRAIN :
            text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/rain.bmp");
            break;
        case p.wtSANDSTORM :
            text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/sand.bmp");
            break;
        case p.wtSNOW :
            text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/snow.bmp");
        default :
            break;
        }*/
        if (p.weather == p.wtRAIN) text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/rain.bmp");
        else if (p.weather == p.wtSANDSTORM) text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/sand.bmp");
        else if (p.weather == p.wtSNOW) text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/snow.bmp");

    }

    // Loading gostek textures
    gost_stopa  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/stopa.bmp");
    gost_klata  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/klata.bmp");
    gost_ramie  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/ramie.bmp");
    gost_morda  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/morda.bmp");
    gost_reka   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/reka.bmp");
    gost_dlon   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/dlon.bmp");
    gost_biodro = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/biodro.bmp");
    gost_udo    = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/udo.bmp");
    gost_noga   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/noga.bmp");
//    gost_x = SDL_GL_LoadTexture(SOL_PATH+"Gostek-gfx/noga.bmp", gost_x_xy);

}


int WorldMap::load_fonts()
{
    std::cout << "Loading fonts ... " << std::endl;

    std::string tmp_str;
    CSimpleIni ini(false, false, false);
    if (ini.LoadFile((SOL_PATH + "Txt/font.ini").c_str()) < 0)
    {
        std::cerr << "Cannot read 'Txt/font.ini' file" << std::endl;
        return 1;
    }

    tmp_str = ini.GetValue("FONTS", "Font1");

    if (tmp_str == "Verdana")
    {
        Font1 = "lucon.ttf";
    }
    else if (tmp_str == "Lucida Console")
    {
        Font1 = "lucon.ttf";
    }
    else if (tmp_str == "BankGothic Lt BT")
    {
        Font1 = "bgothl.ttf";
    }

    tmp_str = ini.GetValue("FONTS", "Font2");
    if (tmp_str == "Verdana")
    {
        Font2 = "lucon.ttf";
    }
    else if (tmp_str == "Lucida Console")
    {
        Font2 = "lucon.ttf";
    }
    else if (tmp_str == "BankGothic Lt BT")
    {
        Font2 = "bgothl.ttf";
    }

    FontMenuSize = ini.GetLongValue("FONTS", "FontMenuSize");
    FontConsoleSize = ini.GetLongValue("FONTS", "FontConsoleSize");
    FontBigSize = ini.GetLongValue("FONTS", "FontBigSize");
    FontWeaponMenuSize = ini.GetLongValue("FONTS", "FontWeaponMenuSize");
    FontConsoleSmallSize = ini.GetLongValue("FONTS", "FontConsoleSmallSize");
    FontHeightScale = ini.GetLongValue("FONTS", "FontHeightScale");
    FontMenuBold = ini.GetLongValue("FONTS", "FontMenuBold");
    FontConsoleBold = ini.GetLongValue("FONTS", "FontConsoleBold");
    FontBigBold = ini.GetLongValue("FONTS", "FontBigBold");
    FontWeaponMenuBold = ini.GetLongValue("FONTS", "FontWeaponMenuBold");
    FontConsoleSmallBold = ini.GetLongValue("FONTS", "FontConsoleSmallBold");
    KillConsoleNameSpace = ini.GetLongValue("FONTS", "KillConsoleNameSpace");

    font1_16.init((SOL_PATH + Font1).c_str(), 16);
    font2_12.init((SOL_PATH + Font2).c_str(), 10);
    font2_28.init((SOL_PATH + Font2).c_str(), 28);

    return 0;

}


int WorldMap::load_map(const std::string& name)
{
    std::cout << "Loading map file ... " << std::endl;

    return p.read(name);
}
