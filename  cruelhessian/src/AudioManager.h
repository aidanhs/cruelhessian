#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

/*   AudioManager.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawe³ Konieczny <konp84 at gmail.com>
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



#include <vector>
#include <iostream>

#include "Singleton.h"
#include "SDL_mixer.h"


class AudioManager : public Singleton<AudioManager>
{

private:

    Mix_Chunk* loadSoundFile(const std::string& file);
    Mix_Chunk* loadSoundFile2(const std::string& src_dir, const std::string& file);

public:

    AudioManager();
    virtual ~AudioManager();
    void setVolume();

    int CURRENT_SONG_NUMBER;
    std::vector<std::string> gMusicList;

    Mix_Music *music;
    Mix_Chunk *grenade_throw, *grenade_bounce, *grenade_explosion, *grenade_pullout, *sound_new_life, *sound_heaven, *sound_death[3], *sound_kitfall[2], *sound_spawn, *menu_click;
    Mix_Chunk *fireSound[20], *reloadSound[20];

    int playMusic(int pos);
};

#define Audio AudioManager::GetSingleton()

#endif

