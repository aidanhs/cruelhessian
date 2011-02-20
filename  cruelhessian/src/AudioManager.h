#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

/*   AudioManager.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
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
#include "SoundBufferManager.h"
#include <SFML/Audio.hpp>




//#define g SoundBufferManager
//extern SoundBufferManager g;

class AudioManager : public Singleton<AudioManager>
{

private:
    //SoundBufferManager g;
    //sf::SoundBuffer* loadSoundFile(const std::string& file) const;
    sf::SoundBuffer* LoadSoundExt(const std::string& src_dir, const std::string& file) const;
    int m_iCurrentSongNumber;
    std::vector<std::string> m_musicList;
    AudioManager(const AudioManager&) {}
    AudioManager& operator=(const AudioManager&) {return *this;}

public:

    AudioManager();
    virtual ~AudioManager();

    void AddToPlaylist(const std::string& buffer);
    void ClearPlaylist();
    void Play(sf::Sound &snd);
    int playMusic(const int pos);
    void setVolume();

    sf::Music *music;
    sf::Sound grenade_throw, grenade_bounce, grenade_explosion, grenade_pullout, cluster, cluster_explosion, sound_new_life, sound_heaven, sound_death[3], sound_kitfall[2], sound_spawn, menu_click;
    sf::Sound take_medikit, take_vestkit, take_berserker, take_predator, take_grenades, take_flamer;
    sf::Sound fireSound[20], reloadSound[20];


};

#define Audio AudioManager::GetSingleton()

#endif

