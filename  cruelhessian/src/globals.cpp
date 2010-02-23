/*   globals.cpp
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

#include <iostream>
#include <fstream>

#include "globals.h"
#include "parser/SimpleIni.h"
#include "boost/filesystem/fstream.hpp"

SDL_Surface *screen;

int CONFIG_VERSION = 1;

GAME_MODE CURRENT_GAME_MODE;

    Uint8 textColorGunOnTouch[4];
    Uint8 textColorGunNormal[4];
    Uint8 textCol[5][4];
        Uint8 textGunColor[4];

float SOUNDS_VOL, MUSIC_VOL;
//std::string SONG_NAME;
int AUDIO_QUAL;
bool FULLSCREEN;
int MAX_BPP;
float MAX_WIDTH, MAX_HEIGHT;

unsigned int TIME_LIMIT;                                   // in seconds !!!!!!!!!!!!!!!
unsigned int FIRST_LIMIT;

freetype::font_data font1_16, font2_12, font2_28;

void printText(freetype::font_data& font, const std::string& text, Uint8* color, float x, float y)
{
    glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    glLoadIdentity();
    freetype::print(font, x, MAX_HEIGHT-y-12.0f, text.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}


void printText(freetype::font_data& font, const std::string& text, unsigned int* color, float x, float y)
{
    glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    glLoadIdentity();
    freetype::print(font, x, MAX_HEIGHT-y-12.0f, text.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}


float bgX, bgY;

std::string PLAYER_NAME;
std::string SOL_PATH;
std::string CH_HOME_DIRECTORY = "";
std::string CH_DATA_DIRECTORY = "data/";
std::string CH_CONFIG_FILE = "";
std::string INTERF_PATH = "";
//std::string INTERFACES_PATH = "";
//std::string INTERFACESX_PATH = "";
std::string CH_INTERFACE = "";

std::vector<std::string> gMusicList;
//std::vector<std::string> mapsList;
int CURRENT_SONG_NUMBER;

int KEY_LEFT;
int KEY_RIGHT;
int KEY_UP;
int KEY_DOWN;
int KEY_RELOAD;
int KEY_GRENADE;
int KEY_CHAT;
int KEY_TEAMCHAT;

Uint32 getCurrentTime;

float SOL_WIDTH[50], SOL_HEIGHT[50];
float fullLife = 495.0f;

float part_x[20][100][100][2], part_y[20][100][100][2], part_z[20][100][100][2];  // rodzaj ruchu - czesc_ciala - klatka (frame) - ulozenie (prawo, lewo)

const float _180overpi = 57.29f;
Mix_Chunk *grenade_throw, *grenade_bounce, *grenade_explosion, *grenade_pullout, *sound_new_life, *sound_heaven, *sound_death[3], *sound_kitfall[2], *sound_spawn, *menu_click;



unsigned int FRAMES_MAX[50], BODYPARTS_MAX[50];
//Tex gost_x;
//Tex gost_stopa[2], gost_klata[2], gost_ramie[2], gost_morda[2], gost_reka[2], gost_dlon[2], gost_udo[2], gost_biodro[2], gost_noga[2];
float sGravity; // Przyspieszenie ziemskie
float sDrag; // Wspólczynnik oporu
float sDragWalking; // Wspólczynnik oporu podczas chodzenia
float sDragFly; // Wspólczynnik oporu podczas latania (im mniejszy tym wyzej dolatuje)
//static float sElasticity = 0.05; // Elastycznosc odbicia
//float sFriction = 1.5; // Tarcie
//float sFriction = 1; // Tarcie

float sWalking; // Szybkosc chodzenia
//float getsWalking(){return sWalking;};
float sFlying; // Szybkosc latania
float sJumping; // Sila skoku

float fTimeStep; // Krok czasowy fizyki



// 'soldat style' interface - fix !!



bool checkSoldat()
{

    boost::filesystem::directory_iterator end;

    if (!boost::filesystem::exists(SOL_PATH))
    {
        return false;
    }

    for (boost::filesystem::directory_iterator itr(SOL_PATH); itr != end; ++itr)
    {
        if (itr->filename() == "Soldat.exe")
        {
            return true;
        }
    }

    return false;
}




int defaults()
{
    std::cout << "Restoring defaults ..." << std::endl;

    SOL_PATH = "";
    CH_INTERFACE = "Default";
    INTERF_PATH = "";
    FULLSCREEN = 0;
    MAX_WIDTH = 640.0f;
    MAX_HEIGHT = 480.0f;
    MAX_BPP = 16;
    SOUNDS_VOL = 20.0f;
    MUSIC_VOL = 0.0f;
    AUDIO_QUAL = 44100;

    PLAYER_NAME = "Player";

    KEY_LEFT = 97;
    KEY_RIGHT = 100;
    KEY_DOWN = 115;
    KEY_UP = 119;
    KEY_RELOAD = 114;
    KEY_GRENADE = 32;
    KEY_CHAT = 116;
    KEY_TEAMCHAT = 121;

    return 0;
}


int read_configs()
{

    CSimpleIni ini(false, false, false);

    if (ini.LoadFile(CH_CONFIG_FILE.c_str()) < 0)
    {
        defaults();
        return 1;
    }

    std::cout << "Loading configs from 'options.ini' ..." << std::endl;

    if (ini.GetLongValue("info", "Version") != CONFIG_VERSION)
    {
        std::cout << "New config file version" << std::endl;
        defaults();
        return 1;
    }

    SOL_PATH = ini.GetValue("global", "SoldatPath");
    CH_INTERFACE = ini.GetValue("global", "Interface");
    FULLSCREEN = ini.GetBoolValue("global", "Fullscreen");
    MAX_WIDTH = static_cast<float>(ini.GetLongValue("global", "MaxWidth"));
    MAX_HEIGHT = static_cast<float>(ini.GetLongValue("global", "MaxHeight"));
    MAX_BPP = ini.GetLongValue("global", "MaxDeep");
    SOUNDS_VOL = static_cast<float>(ini.GetLongValue("global", "SoundVolume"));
    MUSIC_VOL = static_cast<float>(ini.GetLongValue("global", "MusicVolume"));
    AUDIO_QUAL = ini.GetLongValue("global", "AudioQuality");

    PLAYER_NAME = ini.GetValue("player", "Name");

    KEY_LEFT = ini.GetLongValue("controls", "Left");
    KEY_RIGHT = ini.GetLongValue("controls", "Right");
    KEY_DOWN = ini.GetLongValue("controls", "Crouch");
    KEY_UP = ini.GetLongValue("controls", "Jump");
    KEY_RELOAD = ini.GetLongValue("controls", "Reload");
    KEY_GRENADE = ini.GetLongValue("controls", "Grenade");
    KEY_CHAT = ini.GetLongValue("controls", "Chat");
    KEY_TEAMCHAT = ini.GetLongValue("controls", "TeamChat");

	/*if (SOL_PATH[SOL_PATH.length()-1] == '\\')
    {
		std::cout << "JEA" << std::endl;
        SOL_PATH[SOL_PATH.length()-1] = '/';
    }*/
	if (!SOL_PATH.empty())
	{
		if (SOL_PATH[SOL_PATH.length()-1] == '\\')
		{
			//std::cout << "JEA" << std::endl;
			//SOL_PATH[SOL_PATH.length()-1] = '/';
		}
		else if (SOL_PATH[SOL_PATH.length()-1] != '/')
		{
			SOL_PATH += '/';
		}
		INTERF_PATH = SOL_PATH + "Interface-gfx/";
	}
//	std::cout << "JEA" << INTERF_PATH << std::endl;
    /*
    //std::locale loc("");
        std::cout << "Loading maps list from 'mapslist.txt' ..." << std::endl;
        //std::wifstream file((SOL_PATH + "mapslist.txt").c_str(), std::ios_base::binary);
        std::ifstream file((SOL_PATH + "mapslist.txt").c_str());
        //std::locale loc(file.getloc(), new boost::archive::codecvt_null<wchar_t>);

    //    file.imbue(loc);

        if (file.is_open())
        {
            std::string newbuffer;
            std::string buffer;
    int k = 0;
            file.seekg(2, std::ios::beg);
            while (getline(file, buffer))
            //while (file >> buffer)
            {
                //std::cout << buffer[0]<<std::endl;
                //if (buffer[0] != '\n' && buffer[0] != '\r' && buffer != "")
                //if (buffer[0] >= 'a' && buffer[0] <= 'Z')
                {
                              //  newbuffer = buffer;
                //std::cout << "ONE" << std::endl;
                //newbuffer.assign(buffer.begin(), buffer.end());
              //  newbuffer += ".PMS";
                //if (boost::filesystem::exists(newbuffer))
                    mapsList.push_back(buffer);
                //std::cout << buffer << std::endl;
                }

            }
            for (unsigned int i = 0; i < mapsList.size(); ++i)
            {
                std::cout << mapsList[i] << std::endl;
                //if (mapsList[i][0] == ' ') std::cout << "KURE\n";
            }

            file.close();
        }
        else
        {
            std::cout << "ERROR opening file 'mapslist.txt'" << std::endl;
            return -1;
        }
    */
    return 0;
}



int save_configs()
{
    std::cout << "Saving configs to 'options.ini' ..." << std::endl;

    CSimpleIni ini(false, false, false);

    if (!std::ifstream(CH_CONFIG_FILE.c_str()))
    {
        std::ifstream o_file(CH_CONFIG_FILE.c_str());
        o_file.close();
    }

    ini.LoadFile(CH_CONFIG_FILE.c_str());

    ini.SetLongValue("info", "Version", CONFIG_VERSION, NULL);

    ini.SetValue("global", "SoldatPath", SOL_PATH.c_str(), NULL);
    ini.SetValue("global", "Interface", CH_INTERFACE.c_str(), NULL);
    ini.SetBoolValue("global", "Fullscreen", FULLSCREEN, NULL);
    ini.SetLongValue("global", "MaxWidth", static_cast<int>(MAX_WIDTH), NULL);
    ini.SetLongValue("global", "MaxHeight", static_cast<int>(MAX_HEIGHT), NULL);
    ini.SetLongValue("global", "MaxDeep", MAX_BPP, NULL);
    ini.SetLongValue("global", "SoundVolume", static_cast<int>(SOUNDS_VOL), NULL);
    ini.SetLongValue("global", "MusicVolume", static_cast<int>(MUSIC_VOL), NULL);
    ini.SetLongValue("global", "AudioQuality", AUDIO_QUAL, NULL);

    ini.SetValue("player", "Name", PLAYER_NAME.c_str(), NULL);

    ini.SetLongValue("controls", "Left", KEY_LEFT, NULL);
    ini.SetLongValue("controls", "Right", KEY_RIGHT, NULL);
    ini.SetLongValue("controls", "Jump", KEY_UP, NULL);
    ini.SetLongValue("controls", "Crouch", KEY_DOWN, NULL);
    ini.SetLongValue("controls", "Reload", KEY_RELOAD, NULL);
    ini.SetLongValue("controls", "Grenade", KEY_GRENADE, NULL);
    ini.SetLongValue("controls", "Chat", KEY_CHAT, NULL);
    ini.SetLongValue("controls", "TeamChat", KEY_TEAMCHAT, NULL);

    ini.SaveFile(CH_CONFIG_FILE.c_str());

    return 0;
}


int setSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
    {
        std::cerr << "Unable to set attributes SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (FULLSCREEN)
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_FULLSCREEN);
    else
        screen = SDL_SetVideoMode(static_cast<int>(MAX_WIDTH), static_cast<int>(MAX_HEIGHT), MAX_BPP, SDL_OPENGL|SDL_RESIZABLE);

    if (!screen)
    {
        std::cerr << "Unable to set video mode: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Cruel Hessian", "");

    SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    return 0;
}
