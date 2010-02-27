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
#include "boost/regex.hpp"

#include "worldmap.h"
#include "mouse.h"
#include "parser/SimpleIni.h"


void WorldMap::load_weapons_base()
{
    std::cout << "Loading weapons base ..." << std::endl;

    CSimpleIni ini(false, false, false);
    if (ini.LoadFile((SOL_PATH + "weapons.ini").c_str()) < 0)
    {
        std::cerr << "Cannot read 'weapons.ini' file" << std::endl;
        return;
    }

    WeaponBase temp;
    std::string str;
    std::ifstream file((SOL_PATH + "Txt/weaponnames.txt").c_str());

    for (int i = 0; i < 20; ++i)
    {
        std::getline(file, str);
        if (!str.empty() && (str[str.size() - 1] == '\r'))
            str.resize(str.size() - 1);

        temp.name = str;

            // change Barrett name to Barret
        if (temp.name == "Barrett M82A1")
            temp.name = "Barret M82A1";

        std::cout << "   loading gun " << i+1 << " : " << temp.name << std::endl;

        temp.damage = static_cast<float>(ini.GetLongValue(temp.name.c_str(), "Damage"));
        temp.fireInterval = static_cast<Uint32>(16.67 * ini.GetLongValue(temp.name.c_str(), "FireInterval"));
        temp.ammo = ini.GetLongValue(temp.name.c_str(), "Ammo");
        temp.reloadTime = static_cast<Uint32>(16.67 * ini.GetLongValue(temp.name.c_str(), "ReloadTime"));
        temp.speed = ini.GetLongValue(temp.name.c_str(), "Speed");
        temp.bulletStyle = ini.GetLongValue(temp.name.c_str(), "BulletStyle");
        temp.startUpTime = 16.67f * ini.GetLongValue(temp.name.c_str(), "StartUpTime");
        temp.bink = 16.67f * ini.GetLongValue(temp.name.c_str(), "Bink");
        temp.movementAcc = ini.GetLongValue(temp.name.c_str(), "MovementAcc");
        temp.recoil = ini.GetLongValue(temp.name.c_str(), "Recoil");

        weapon_base.push_back(temp);
    }


    file.close();
}


void WorldMap::load_configs()
{
    std::cout << "Loading configs ..." << std::endl;

    /* aGravity = TVector2D(0.0f, 9.81f);
     aWalkingLeft = TVector2D(8.0f, 0.0f);
     aWalkingRight = TVector2D(-8.0f, 0.0f);
     aJumpUp = TVector2D(0.0f, -17.0f);
    */

    MAX_RESPAWN_TIME = 5;
   // GUN_MENU_START = TVector2D(MAX_WIDTH/4, MAX_HEIGHT/7);
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
    sDrag = 0.3;
    sDragWalking = 300;
    sWalking = 100;
    sFlying = 2000.0;
    sJumping = 2200.0;

    DISTANCE_SCORE = MY_CURRENT_POS = 0;

    mouse = new Mouse(text_mouse);
    chat = new Chat();
    window_scores = new WindowScores(text_deaddot, text_smalldot);
    window_guns = new WindowGuns(weapon_base);

}


void WorldMap::load_bonuses()
{
    std::cout << "Loading bonuses ..." << std::endl;

    bonus.resize(7);

    for (int i = 0; i < map->spawnpointCount; ++i)
    {
        if (map->spawnpoint[i].active)
        {
            if (map->spawnpoint[i].team == map->stBERSERKER) bonus[BONUS_BERSERKER].push_back(i);
            else if (map->spawnpoint[i].team == map->stCLUSTERS) bonus[BONUS_CLUSTERS].push_back(i);
            else if (map->spawnpoint[i].team == map->stFLAMER) bonus[BONUS_FLAMER].push_back(i);
            else if (map->spawnpoint[i].team == map->stGRENADES) bonus[BONUS_GRENADES].push_back(i);
            else if (map->spawnpoint[i].team == map->stMEDKITS) bonus[BONUS_MEDIKITS].push_back(i);
            else if (map->spawnpoint[i].team == map->stPREDATOR) bonus[BONUS_PREDATOR].push_back(i);
            else if (map->spawnpoint[i].team == map->stVEST) bonus[BONUS_VESTKIT].push_back(i);
        }
    }

    // laduj wszystko co jest
    // fix it !!!!!!!!!!!!
    TVector2D p;
    int num;
    for (unsigned int i = 0; i < 7; ++i)
        for (unsigned int j = 0; j < bonus[i].size(); ++j)
        {
            num = bonus[i][j];
            p.x = static_cast<float>(map->spawnpoint[num].x);
            p.y = static_cast<float>(map->spawnpoint[num].y);
            Bonus *newbon = new Bonus(p, i, text_bonus[i], BONUS_GRENADES);
            bonus_list.push_back(newbon);
//            m_objects.push_back(newbon);
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

int WorldMap::getWeaponNumber(const std::string& gun)
{
    for (int i = 0; i < 20; ++i)
    {
        if (weapon_base[i].name == gun)
            return i;
    }
    std::cout << "Unknown gun : " << gun << std::endl;
    return -1;
}

// convert TColor to RGB
// TColor - $00bbggrr
unsigned int* WorldMap::getRGB(const std::string& col)
{

    unsigned int *res = new unsigned int[3];

    sscanf(col.substr(3, 2).c_str(), "%x", &res[2]);
    sscanf(col.substr(5, 2).c_str(), "%x", &res[1]);
    sscanf(col.substr(7, 2).c_str(), "%x", &res[0]);

    return res;
}



int WorldMap::load_bots_base()
{
    std::cout << "Loading bots base ..." << std::endl;

    std::string fold = SOL_PATH + "Bots/";
    CSimpleIni ini(false, false, false);

    srand(static_cast<unsigned int>(time(0)));

    //boost::regex re(fold + ".+.bot");
	boost::regex re(".+.bot");
    boost::filesystem::directory_iterator end;

    // reading Bots directory
    if (!boost::filesystem::exists(fold))
    {
        std::cout << "'Bots' directory doesn't exist !" << std::endl;
        return 1;
    }

    int nr = 0;
    BotsBase temp;

    for (boost::filesystem::directory_iterator iter(fold); iter != end; ++iter)
    {
        //if (boost::regex_match(iter->path().string(), re))
			if (boost::regex_match(iter->leaf(), re))
        {

            if (ini.LoadFile((iter->path().string()).c_str()) < 0)
            {
                std::cerr << "Error in parsing " << iter->path().string() << std::endl;
                return 1;
            }

            temp.name = ini.GetValue("BOT", "Name");

            std::cout << "   loading bot " << ++nr << " : " << temp.name << std::endl;

            temp.favouriteWeapon = getWeaponNumber(ini.GetValue("BOT", "Favourite_Weapon"));
            temp.color[SHIRT] = getRGB(ini.GetValue("BOT", "Color1"));
            temp.color[PANTS] = getRGB(ini.GetValue("BOT", "Color2"));
            temp.color[SKIN] = getRGB(ini.GetValue("BOT", "Skin_Color"));
            temp.color[HAIR] = getRGB(ini.GetValue("BOT", "Hair_Color"));
            temp.chatKill = ini.GetValue("BOT", "Chat_Kill");
            temp.chatDead = ini.GetValue("BOT", "Chat_Dead");
            temp.chatLowhealth = ini.GetValue("BOT", "Chat_Lowhealth");
            temp.chatSeeEnemy = ini.GetValue("BOT", "Chat_SeeEnemy");
            temp.chatWinning = ini.GetValue("BOT", "Chat_Winning");

            bots_base.push_back(temp);
        }
    }
    if (bots_base.empty())
    {
        std::cerr << "'Bots' directory hasn't got *.bot files !" << std::endl;
        return 1;
    }
    return 0;
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
       to dodawaj nazwom botow liczbe od 10 do 99 */

    // flist zawiera tyle elementow, ile wynosi liczba botow z kalatogu "Bots"

    if (!spawnpoint[team].empty())
    {
        unsigned int b_cnt = 0, sol_nr, bot_nr;
        std::vector<unsigned int>::iterator it;
        std::vector<unsigned int> flist;
        std::ostringstream oss;

        while (b_cnt < bot_count)
        {
            for (unsigned int k = 0; k < bots_base.size(); ++k)
            {
                flist.push_back(k);
            }

            while (b_cnt < bot_count && !flist.empty())
            {
                sol_nr = rand() % flist.size();
                bot_nr = addBot(bots_base[flist[sol_nr]], spawnpoint[team][rand()%spawnpoint[team].size()], team);

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
            if ((weapon_base[0].fireSound = loadSoundFile(SOL_PATH + "Sfx/colt1911-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[0].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[1].fireSound = loadSoundFile(SOL_PATH + "Sfx/deserteagle-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[1].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[2].fireSound = loadSoundFile(SOL_PATH + "Sfx/mp5-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[2].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[3].fireSound = loadSoundFile(SOL_PATH + "Sfx/ak74-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[3].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[4].fireSound = loadSoundFile(SOL_PATH + "Sfx/steyraug-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[4].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[5].fireSound = loadSoundFile(SOL_PATH + "Sfx/spas12-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[5].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[6].fireSound = loadSoundFile(SOL_PATH + "Sfx/ruger77-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[6].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[7].fireSound = loadSoundFile(SOL_PATH + "Sfx/m79-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[7].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[8].fireSound = loadSoundFile(SOL_PATH + "Sfx/barretm82-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[8].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[9].fireSound = loadSoundFile(SOL_PATH + "Sfx/m249-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[9].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[10].fireSound = loadSoundFile(SOL_PATH + "Sfx/minigun-fire.wav")) != NULL) Mix_VolumeChunk(weapon_base[10].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[11].fireSound = loadSoundFile(SOL_PATH + "Sfx/flamer.wav")) != NULL) Mix_VolumeChunk(weapon_base[11].fireSound, SOUNDS_VOL_INT);
            if ((weapon_base[15].fireSound = loadSoundFile(SOL_PATH + "Sfx/chainsaw-r.WAV")) != NULL) Mix_VolumeChunk(weapon_base[15].fireSound, SOUNDS_VOL_INT);

            if ((weapon_base[0].reloadSound = loadSoundFile(SOL_PATH + "Sfx/colt1911-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[0].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[1].reloadSound = loadSoundFile(SOL_PATH + "Sfx/deserteagle-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[1].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[2].reloadSound = loadSoundFile(SOL_PATH + "Sfx/mp5-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[2].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[3].reloadSound = loadSoundFile(SOL_PATH + "Sfx/ak74-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[3].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[4].reloadSound = loadSoundFile(SOL_PATH + "Sfx/steyraug-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[4].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[5].reloadSound = loadSoundFile(SOL_PATH + "Sfx/spas12-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[5].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[6].reloadSound = loadSoundFile(SOL_PATH + "Sfx/ruger77-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[6].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[7].reloadSound = loadSoundFile(SOL_PATH + "Sfx/m79-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[7].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[8].reloadSound = loadSoundFile(SOL_PATH + "Sfx/barretm82-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[8].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[9].reloadSound = loadSoundFile(SOL_PATH + "Sfx/m249-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[9].reloadSound, SOUNDS_VOL_INT);
            if ((weapon_base[10].reloadSound = loadSoundFile(SOL_PATH + "Sfx/minigun-reload.wav")) != NULL) Mix_VolumeChunk(weapon_base[10].reloadSound, SOUNDS_VOL_INT);

            if ((grenade_pullout = loadSoundFile(SOL_PATH + "Sfx/grenade-pullout.wav")) != NULL) Mix_VolumeChunk(grenade_pullout, SOUNDS_VOL_INT);
            if ((grenade_throw = loadSoundFile(SOL_PATH + "Sfx/grenade-throw.wav")) != NULL) Mix_VolumeChunk(grenade_throw, SOUNDS_VOL_INT);
            if ((grenade_bounce = loadSoundFile(SOL_PATH + "Sfx/grenade-bounce.wav")) != NULL) Mix_VolumeChunk(grenade_bounce, SOUNDS_VOL_INT);
            if ((grenade_explosion = loadSoundFile(SOL_PATH + "Sfx/grenade-explosion.wav")) != NULL) Mix_VolumeChunk(grenade_explosion, SOUNDS_VOL_INT);
            if ((sound_new_life = loadSoundFile(SOL_PATH + "Sfx/wermusic.wav")) != NULL) Mix_VolumeChunk(sound_new_life, SOUNDS_VOL_INT);
            if ((sound_death[0] = loadSoundFile(SOL_PATH + "Sfx/death.wav")) != NULL) Mix_VolumeChunk(sound_death[0], SOUNDS_VOL_INT);
            if ((sound_death[1] = loadSoundFile(SOL_PATH + "Sfx/death2.wav")) != NULL) Mix_VolumeChunk(sound_death[1], SOUNDS_VOL_INT);
            if ((sound_death[2] = loadSoundFile(SOL_PATH + "Sfx/death3.wav")) != NULL) Mix_VolumeChunk(sound_death[2], SOUNDS_VOL_INT);
            if ((sound_kitfall[0] = loadSoundFile(SOL_PATH + "Sfx/kit-fall.wav")) != NULL) Mix_VolumeChunk(sound_kitfall[0], SOUNDS_VOL_INT);
            if ((sound_kitfall[1] = loadSoundFile(SOL_PATH + "Sfx/kit-fall2.wav")) != NULL) Mix_VolumeChunk(sound_kitfall[1], SOUNDS_VOL_INT);
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

    std::cout << "   loading scenery textures ... " << std::endl;
	GLuint *text_scen = new GLuint[map->sceneryCount];
	std::vector<std::string> names(map->getTextScen());
	for (int i = 0; i < names.size(); ++i)
	{
		text_scen[i] = SOIL_LoadTexture(SOL_PATH + "Scenery-gfx/" + names[i]);
	}
	map->setScen(text_scen);

    std::cout << "   loading triangles texture ... " << std::endl;
	GLuint text_poly = SOIL_LoadTexture(SOL_PATH + "Textures/" + map->getTextPoly());
	map->setPoly(text_poly);

    std::cout << "   loading interface textures ... " << std::endl;
    text_mouse     = SOIL_LoadTextureEx(findInterface("cursor.bmp"));
    text_health    = SOIL_LoadTextureEx(findInterface("health.bmp"));
    text_healthbar = SOIL_LoadTextureEx(findInterface("health-bar.bmp"));
    text_ammo      = SOIL_LoadTextureEx(findInterface("ammo.bmp"));
    text_nade      = SOIL_LoadTextureEx(findInterface("nade.bmp"));
    text_reloadbar = SOIL_LoadTextureEx(findInterface("reload-bar.bmp"));
    text_jet       = SOIL_LoadTextureEx(findInterface("jet.bmp"));
    text_jetbar    = SOIL_LoadTextureEx(findInterface("jet-bar.bmp"));
    text_firebar   = SOIL_LoadTextureEx(findInterface("fire-bar.bmp"));
    text_firebar_r = SOIL_LoadTextureEx(findInterface("fire-bar-r.bmp"));
    text_deaddot   = SOIL_LoadTextureEx(findInterface("deaddot.bmp"));
    text_smalldot  = SOIL_LoadTextureEx(findInterface("smalldot.bmp"));

    std::cout << "   loading bonuses' textures ... " << std::endl;
    text_bonus[BONUS_BERSERKER] = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/berserkerkit.bmp");
    text_bonus[BONUS_CLUSTERS]  = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/clusterkit.bmp");
    text_bonus[BONUS_FLAMER]    = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/flamerkit.bmp");
    text_bonus[BONUS_GRENADES]  = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/grenadekit.bmp");
    text_bonus[BONUS_MEDIKITS]  = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/medikit.bmp");
    text_bonus[BONUS_PREDATOR]  = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/predatorkit.bmp");
    text_bonus[BONUS_VESTKIT]   = SOIL_LoadTextureEx(SOL_PATH + "Textures/Objects/vestkit.bmp");

    std::cout << "   loading guns' textures ... " << std::endl;
    weapon_base[0].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/10.bmp");
    weapon_base[1].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/1.bmp");
    weapon_base[2].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/2.bmp");
    weapon_base[3].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/3.bmp");
    weapon_base[4].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/4.bmp");
    weapon_base[5].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/5.bmp");
    weapon_base[6].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/6.bmp");
    weapon_base[7].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/7.bmp");
    weapon_base[8].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/8.bmp");
    weapon_base[9].textureGun  = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/9.bmp");
    weapon_base[10].textureGun = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/0.bmp");
    weapon_base[14].textureGun = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/knife.bmp");
    weapon_base[15].textureGun = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/chainsaw.bmp");
    weapon_base[16].textureGun = SOIL_LoadTextureEx(SOL_PATH + "Interface-gfx/Guns/law.bmp");

    std::cout << "   loading bullets' textures ... " << std::endl;
    weapon_base[0].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/minigun-bullet.png");
    weapon_base[1].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/eagles-bullet.png");
    weapon_base[2].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/ak74-bullet.png");
    weapon_base[3].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/ak74-bullet.png");
    weapon_base[4].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/steyraug-bullet.png");
    weapon_base[5].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/spas12-bullet.png");
    weapon_base[6].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/ruger77-bullet.png");
    weapon_base[7].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/m79-bullet.bmp");
    weapon_base[8].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/barretm82-bullet.png");
    weapon_base[9].textureAmmo  = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/spas12-bullet.png");
    weapon_base[10].textureAmmo = SOIL_LoadTextureEx(SOL_PATH + "Weapons-gfx/spas12-bullet.png");

    std::cout << "   loading grenade's textures ... " << std::endl;
    text_grenade[0] = text_nade;
    std::ostringstream oss;
    for (int i = 1; i < 16; ++i)
    {
        oss << i+1;
        text_grenade[i] = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/explosion/explode" + oss.str() + ".bmp");
        oss.str("");
    }

    std::cout << "   loading weather textures ... " << std::endl;
    if (map->weather != map->wtNONE)
    {
        if (map->weather == map->wtRAIN)
            text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/rain.bmp");
        else if (map->weather == map->wtSANDSTORM)
            text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/sand.bmp");
        else if (map->weather == map->wtSNOW)
            text_weath = SOIL_LoadTextureEx(SOL_PATH + "Sparks-gfx/snow.bmp");
    }

    std::cout << "   loading gostek textures ... " << std::endl;
    gostek[STOPA][0]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/stopa.bmp");
    gostek[STOPA][1]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/stopa2.bmp");
    gostek[KLATA][0]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/klata.bmp");
    gostek[KLATA][1]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/klata2.bmp");
    gostek[RAMIE][0]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/ramie.bmp");
    gostek[RAMIE][1]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/ramie2.bmp");
    gostek[MORDA][0]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/morda.bmp");
    gostek[MORDA][1]  = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/morda2.bmp");
    gostek[REKA][0]   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/reka.bmp");
    gostek[REKA][1]   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/reka2.bmp");
    gostek[DLON][0]   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/dlon.bmp");
    gostek[DLON][1]   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/dlon2.bmp");
    gostek[BIODRO][0] = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/biodro.bmp");
    gostek[BIODRO][1] = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/biodro2.bmp");
    gostek[UDO][0]    = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/udo.bmp");
    gostek[UDO][1]    = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/udo2.bmp");
    gostek[NOGA][0]   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/noga.bmp");
    gostek[NOGA][1]   = SOIL_LoadTextureEx(SOL_PATH + "Gostek-gfx/noga2.bmp");

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

    font1_16.init((SOL_PATH + Font1).c_str(), 14);
    font2_12.init((SOL_PATH + Font2).c_str(), 10);
    font2_28.init((SOL_PATH + Font2).c_str(), 28);

//    font1_16 = new OGLFT::Monochrome((SOL_PATH + Font1).c_str(), 14);
//    font2_12 = new OGLFT::Monochrome((SOL_PATH + Font2).c_str(), 10);
//    font2_28 = new OGLFT::Monochrome((SOL_PATH + Font2).c_str(), 28);

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

