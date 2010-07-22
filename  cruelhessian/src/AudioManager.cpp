/*   AudioManager.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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


#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"

#include "AudioManager.h"
#include "WorldMap.h"
#include "ParserManager.h"


AudioManager::AudioManager()
{

    std::cout << "Starting AudioManager ... " << std::endl;

    CURRENT_SONG_NUMBER = 0;

    music = NULL;

    //if (Parser.SOUNDS_VOL + Parser.MUSIC_VOL > 0)
    // {
    std::cout << "   loading audio ... " << std::endl;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cout << "  Unable to init audio: " << SDL_GetError() << std::endl;
        //return 1;
    }

    if (Mix_OpenAudio(Parser.AUDIO_QUAL, AUDIO_S16SYS, 1, 2048) != 0)
    {
        std::cout << "  Unable to initialize audio: " << Mix_GetError() << std::endl;
        //return 1;
    }

    //if (Parser.SOUNDS_VOL > 0)
    //{
    //fireSound[0] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "colt1911-fire");
    //  if (fireSound[0] != NULL) Mix_VolumeChunk(fireSound[0], SOUNDS_VOL_INT);
    //
    fireSound[0] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "colt1911-fire");//) != NULL) Mix_VolumeChunk(fireSound[0], SOUNDS_VOL_INT);
    fireSound[1] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "deserteagle-fire");//) != NULL) Mix_VolumeChunk(fireSound[1], SOUNDS_VOL_INT);
    fireSound[2] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "mp5-fire");//) != NULL) Mix_VolumeChunk(fireSound[2], SOUNDS_VOL_INT);
    fireSound[3] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "ak74-fire");//) != NULL) Mix_VolumeChunk(fireSound[3], SOUNDS_VOL_INT);
    fireSound[4] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "steyraug-fire");//) != NULL) Mix_VolumeChunk(fireSound[4], SOUNDS_VOL_INT);
    fireSound[5] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "spas12-fire");//) != NULL) Mix_VolumeChunk(fireSound[5], SOUNDS_VOL_INT);
    fireSound[6] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "ruger77-fire");//) != NULL) Mix_VolumeChunk(fireSound[6], SOUNDS_VOL_INT);
    fireSound[7] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "m79-fire");//) != NULL) Mix_VolumeChunk(fireSound[7], SOUNDS_VOL_INT);
    fireSound[8] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "barretm82-fire");//) != NULL) Mix_VolumeChunk(fireSound[8], SOUNDS_VOL_INT);
    fireSound[9] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "m249-fire");//) != NULL) Mix_VolumeChunk(fireSound[9], SOUNDS_VOL_INT);
    fireSound[10] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "minigun-fire");//) != NULL) Mix_VolumeChunk(fireSound[10], SOUNDS_VOL_INT);
    fireSound[11] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "flamer");//) != NULL) Mix_VolumeChunk(fireSound[11], SOUNDS_VOL_INT);
    fireSound[12] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "chainsaw-r");//) != NULL) Mix_VolumeChunk(fireSound[12], SOUNDS_VOL_INT);

    reloadSound[0] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "colt1911-reload");//) != NULL) Mix_VolumeChunk(reloadSound[0], SOUNDS_VOL_INT);
    reloadSound[1] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "deserteagle-reload");//) != NULL) Mix_VolumeChunk(reloadSound[1], SOUNDS_VOL_INT);
    reloadSound[2] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "mp5-reload");//) != NULL) Mix_VolumeChunk(reloadSound[2], SOUNDS_VOL_INT);
    reloadSound[3] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "ak74-reload");//) != NULL) Mix_VolumeChunk(reloadSound[3], SOUNDS_VOL_INT);
    reloadSound[4] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "steyraug-reload");//) != NULL) Mix_VolumeChunk(reloadSound[4], SOUNDS_VOL_INT);
    reloadSound[5] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "spas12-reload");//) != NULL) Mix_VolumeChunk(reloadSound[5], SOUNDS_VOL_INT);
    reloadSound[6] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "ruger77-reload");//) != NULL) Mix_VolumeChunk(reloadSound[6], SOUNDS_VOL_INT);
    reloadSound[7] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "m79-reload");//) != NULL) Mix_VolumeChunk(reloadSound[7], SOUNDS_VOL_INT);
    reloadSound[8] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "barretm82-reload");//) != NULL) Mix_VolumeChunk(reloadSound[8], SOUNDS_VOL_INT);
    reloadSound[9] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "m249-reload");//) != NULL) Mix_VolumeChunk(reloadSound[9], SOUNDS_VOL_INT);
    reloadSound[10] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "minigun-reload");//) != NULL) Mix_VolumeChunk(reloadSound[10], SOUNDS_VOL_INT);

    grenade_pullout = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "grenade-pullout");//) != NULL)
    grenade_throw = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "grenade-throw");//) != NULL)
    grenade_bounce = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "grenade-bounce");//) != NULL)
    grenade_explosion = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "grenade-explosion");//) != NULL)
    sound_new_life = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "wermusic");//) != NULL)
    sound_death[0] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "death");//) != NULL)
    sound_death[1] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "death2");//) != NULL) Mix_VolumeChunk(sound_death[1], SOUNDS_VOL_INT);
    sound_death[2] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "death3");//) != NULL) Mix_VolumeChunk(sound_death[2], SOUNDS_VOL_INT);
    //sound_kitfall[0] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "kit-fall")) != NULL)
    //sound_kitfall[1] = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "kit-fall2")) != NULL) Mix_VolumeChunk(sound_kitfall[1], SOUNDS_VOL_INT);
    sound_heaven = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "playerdeath");//) != NULL)
    sound_spawn = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "spawn");//) != NULL)
    menu_click = loadSoundFile2(Parser.SOL_PATH + "Sfx/", "menuclick");//) != NULL)

    sound_kitfall[0] = NULL;
    sound_kitfall[1] = NULL;

    setVolume();
    //}
    //}
}


void AudioManager::setVolume()
{

    int SOUNDS_VOL_INT = static_cast<int>(Parser.SOUNDS_VOL*MIX_MAX_VOLUME/100);

    for (unsigned int i = 0; i < 13; ++i)
    {
        Mix_VolumeChunk(fireSound[i], SOUNDS_VOL_INT);
    }

    for (unsigned int i = 0; i < 11; ++i)
    {
        Mix_VolumeChunk(reloadSound[i], SOUNDS_VOL_INT);
    }

    for (int i = 0; i < 2; ++i)
    {
        //Mix_VolumeChunk(sound_kitfall[i], SOUNDS_VOL_INT);
    }

    for (int i = 0; i < 3; ++i)
    {
        Mix_VolumeChunk(sound_death[i], SOUNDS_VOL_INT);
    }

    Mix_VolumeChunk(grenade_pullout, SOUNDS_VOL_INT);
    Mix_VolumeChunk(grenade_throw, SOUNDS_VOL_INT);
    Mix_VolumeChunk(grenade_bounce, SOUNDS_VOL_INT);
    Mix_VolumeChunk(grenade_explosion, SOUNDS_VOL_INT);
    Mix_VolumeChunk(sound_new_life, SOUNDS_VOL_INT);
    Mix_VolumeChunk(sound_heaven, SOUNDS_VOL_INT);
    Mix_VolumeChunk(sound_spawn, SOUNDS_VOL_INT);
    Mix_VolumeChunk(menu_click, SOUNDS_VOL_INT);

}

AudioManager::~AudioManager()
{

    std::cout << "Removing AudioManager ..." << std::endl;

    //if (Parser.SOUNDS_VOL > 0)
    //{
    for (int i = 0; i < 13; ++i)
    {
        Mix_FreeChunk(fireSound[i]);
        fireSound[i] = NULL;
    }
    for (int i = 0; i < 11; ++i)
    {
        Mix_FreeChunk(reloadSound[i]);
        reloadSound[i] = NULL;
    }
    for (int i = 0; i < 2; ++i)
    {
        Mix_FreeChunk(sound_kitfall[i]);
        sound_kitfall[i] = NULL;
    }
    for (int i = 0; i < 3; ++i)
    {
        Mix_FreeChunk(sound_death[i]);
        sound_death[i] = NULL;
    }

    Mix_FreeChunk(grenade_explosion);
    grenade_explosion = NULL;
    Mix_FreeChunk(grenade_bounce);
    grenade_bounce = NULL;
    Mix_FreeChunk(grenade_throw);
    grenade_throw = NULL;
    Mix_FreeChunk(grenade_pullout);
    grenade_pullout = NULL;
    Mix_FreeChunk(sound_new_life);
    sound_new_life = NULL;
    Mix_FreeChunk(sound_heaven);
    sound_heaven = NULL;
    Mix_FreeChunk(sound_spawn);
    sound_spawn = NULL;
    Mix_FreeChunk(menu_click);
    menu_click = NULL;

    //}

    //if (Parser.SOUNDS_VOL + Parser.MUSIC_VOL > 0)
    Mix_CloseAudio();

}


Mix_Chunk* AudioManager::loadSoundFile(const std::string& file)
{
    Mix_Chunk *temp = NULL;
    if ((temp = Mix_LoadWAV(file.c_str())) == NULL)
    {
        std::cerr << "  Cannot load sound file: " << file << ", " << Mix_GetError() << std::endl;
    }
    return temp;
}



// urwana nazwa pliku, bez rozszerzenia (najpierw wav, potem ogg), odporna na wielkosc znakow (tylko w nazwach plikow)
Mix_Chunk* AudioManager::loadSoundFile2(const std::string& src_dir, const std::string& file)
{

    if (!boost::filesystem::exists(src_dir))
    {
        std::cerr << "  Cannot find source directory : " << src_dir << std::endl;
        return NULL;
    }

    boost::filesystem::directory_iterator end;

    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {
        if (boost::iequals(iter->leaf(), file + ".wav"))
        {
            // !!!!!!!!!fix
            return loadSoundFile(iter->path().string());
        }
    }

    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {
        if (boost::iequals(iter->leaf(), file + ".ogg"))
        {
            // !!!!!!!!!fix
            return loadSoundFile(iter->path().string());
        }
    }

    std::cerr << "  Cannot find : " << file << std::endl;
    return NULL;
}



int AudioManager::playMusic(int pos)
{

    if (Parser.MUSIC_VOL > 0 && !gMusicList.empty())
    {
        CURRENT_SONG_NUMBER += pos;
        if (CURRENT_SONG_NUMBER < 0)
        {
            CURRENT_SONG_NUMBER = static_cast<int>(gMusicList.size()-1);
        }
        else if (CURRENT_SONG_NUMBER > static_cast<int>(gMusicList.size()-1))
        {
            CURRENT_SONG_NUMBER = 0;
        }

        if (Mix_PlayingMusic())
        {
            Mix_HaltMusic();
            Mix_FreeMusic(music);
        }


        world.chat->addMessage("Playing : " + gMusicList[CURRENT_SONG_NUMBER]);

        if ((music = Mix_LoadMUS(gMusicList[CURRENT_SONG_NUMBER].c_str())) == NULL)
        {
            std::cerr << "Error Mixer: " << Mix_GetError() << std::endl;
            return -1;
        }
        if (Mix_PlayMusic(music, 0) == -1)
        {
            std::cerr << "Unable to play file: " << Mix_GetError() << std::endl;
            return -1;
        }
    }

    return 0;
}
