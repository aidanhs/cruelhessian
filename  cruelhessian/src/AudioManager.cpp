/*   AudioManager.cpp
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


#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"

#include "textinout/TextOutput.h"
#include "AudioManager.h"
#include "WorldMap.h"
#include "ParserManager.h"
#include "SoundBufferManager.h"





AudioManager::AudioManager()
{


    m_iCurrentSongNumber= 0;

    std::cout << "Starting AudioManager ... " << std::flush;

    music = new sf::Music();

    grenade_pullout.SetBuffer  (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "grenade-pullout"));
    grenade_throw.SetBuffer    (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "grenade-throw"));
    grenade_bounce.SetBuffer   (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "grenade-bounce"));
    grenade_explosion.SetBuffer(*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "grenade-explosion"));
    cluster_explosion.SetBuffer(*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "cluster-explosion"));
    cluster.SetBuffer          (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "clustergrenade"));
    sound_new_life.SetBuffer   (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "wermusic"));
    sound_heaven.SetBuffer     (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "playerdeath"));
    sound_spawn.SetBuffer      (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "spawn"));
    menu_click.SetBuffer       (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "menuclick"));
    take_medikit.SetBuffer     (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "takemedikit"));
    take_vestkit.SetBuffer     (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "vesttake"));
    take_flamer.SetBuffer      (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "godflame"));
    take_berserker.SetBuffer   (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "berserker"));
    take_predator.SetBuffer    (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "predator"));
    take_grenades.SetBuffer    (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "pickupgun"));

    fireSound[0].SetBuffer( *LoadSoundExt(Parser.SOL_PATH + "Sfx/", "colt1911-fire"));
    fireSound[1].SetBuffer( * LoadSoundExt(Parser.SOL_PATH + "Sfx/", "deserteagle-fire"));
    fireSound[2].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "mp5-fire"));
    fireSound[3].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "ak74-fire"));
    fireSound[4].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "steyraug-fire"));
    fireSound[5].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "spas12-fire"));
    fireSound[6].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "ruger77-fire"));
    fireSound[7].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "m79-fire"));
    fireSound[8].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "barretm82-fire"));
    fireSound[9].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "m249-fire"));
    fireSound[10].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "minigun-fire"));
    fireSound[11].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "flamer"));
    fireSound[12].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "chainsaw-r"));

    reloadSound[0].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "colt1911-reload"));
    reloadSound[1].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "deserteagle-reload"));
    reloadSound[2].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "mp5-reload"));
    reloadSound[3].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "ak74-reload"));
    reloadSound[4].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "steyraug-reload"));
    reloadSound[5].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "spas12-reload"));
    reloadSound[6].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "ruger77-reload"));
    reloadSound[7].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "m79-reload"));
    reloadSound[8].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "barretm82-reload"));
    reloadSound[9].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "m249-reload"));
    reloadSound[10].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "minigun-reload"));

    sound_death[0].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "death"));
    sound_death[1].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "death2"));
    sound_death[2].SetBuffer (*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "death3"));
    sound_kitfall[0].SetBuffer ( *LoadSoundExt(Parser.SOL_PATH + "Sfx/", "kit-fall"));
    sound_kitfall[1].SetBuffer(*LoadSoundExt(Parser.SOL_PATH + "Sfx/", "kit-fall2"));


    setVolume();

    std::cout << " DONE" << std::endl;
}


void AudioManager::setVolume()
{

    //sf::Listener::SetGlobalVolume(Parser.SOUNDS_VOL);
    for (unsigned int i = 0; i < 13; ++i)
    {
        fireSound[i].SetVolume(Parser.SOUNDS_VOL);
    }

    for (unsigned int i = 0; i < 11; ++i)
    {
        reloadSound[i].SetVolume(Parser.SOUNDS_VOL);
    }

    for (int i = 0; i < 2; ++i)
    {
         sound_kitfall[i].SetVolume(Parser.SOUNDS_VOL);
    }

    for (int i = 0; i < 3; ++i)
    {
        sound_death[i].SetVolume(Parser.SOUNDS_VOL);
    }

    grenade_pullout.SetVolume(Parser.SOUNDS_VOL);
    grenade_throw.SetVolume(Parser.SOUNDS_VOL);
    grenade_bounce.SetVolume(Parser.SOUNDS_VOL);
    grenade_explosion.SetVolume(Parser.SOUNDS_VOL);
    cluster_explosion.SetVolume(Parser.SOUNDS_VOL);
    cluster.SetVolume(Parser.SOUNDS_VOL);
    sound_new_life.SetVolume(Parser.SOUNDS_VOL);
    sound_heaven.SetVolume(Parser.SOUNDS_VOL);
    sound_spawn.SetVolume(Parser.SOUNDS_VOL);
    menu_click.SetVolume(Parser.SOUNDS_VOL);
    take_berserker.SetVolume(Parser.SOUNDS_VOL);
    take_medikit.SetVolume(Parser.SOUNDS_VOL);
    take_predator.SetVolume(Parser.SOUNDS_VOL);
    take_vestkit.SetVolume(Parser.SOUNDS_VOL);
    take_grenades.SetVolume(Parser.SOUNDS_VOL);
    take_flamer.SetVolume(Parser.SOUNDS_VOL);

}

AudioManager::~AudioManager()
{

    std::cout << "Removing AudioManager ..." << std::flush;

/*
    for (int i = 0; i < 13; ++i)
    {
        delete fireSound[i];
    }
    for (int i = 0; i < 11; ++i)
    {
        delete reloadSound[i];
    }
    for (int i = 0; i < 2; ++i)
    {
        delete sound_kitfall[i];
    }
    for (int i = 0; i < 3; ++i)
    {
        delete sound_death[i];
    }

    delete cluster;
    delete cluster_explosion;
    delete grenade_explosion;
    delete grenade_bounce;
    delete grenade_throw;
    delete grenade_pullout;
    delete sound_new_life;
    delete sound_heaven;
    delete sound_spawn;
    delete menu_click;

    delete take_berserker;
    delete take_grenades;
    delete take_medikit;
    delete take_predator;
    delete take_vestkit;
    delete take_flamer;
*/
    delete music;

    std::cout << " DONE" << std::endl;

}



void AudioManager::Play(sf::Sound &snd)
{
    snd.Play();
}


// urwana nazwa pliku, bez rozszerzenia (najpierw wav, potem ogg), odporna na wielkosc znakow (tylko w nazwach plikow)
sf::SoundBuffer* AudioManager::LoadSoundExt(const std::string& src_dir, const std::string& file) const
{

    if (!boost::filesystem::exists(src_dir))
    {
        std::cerr << "  Cannot find source directory : " << src_dir << std::endl;
        return NULL;
    }

    boost::filesystem::directory_iterator end;

    for (boost::filesystem::directory_iterator iter(src_dir); iter != end; ++iter)
    {
        if (boost::iequals(iter->leaf(), file + ".wav") || boost::iequals(iter->leaf(), file + ".ogg"))
        {
            //return g.Load(iter->path().string());
            return g.getResource(iter->path().string());
        }
    }

    std::cerr << "  Cannot find : " << file << std::endl;
    return NULL;
}



int AudioManager::playMusic(const int pos)
{

    if (Parser.MUSIC_VOL > 0 && !m_musicList.empty())
    {
        m_iCurrentSongNumber += pos;
        if (m_iCurrentSongNumber < 0)
        {
            m_iCurrentSongNumber = static_cast<int>(m_musicList.size()-1);
        }
        else if (m_iCurrentSongNumber > static_cast<int>(m_musicList.size()-1))
        {
            m_iCurrentSongNumber = 0;
        }

        if (music->GetStatus() == sf::Music::Playing)
        {
            music->Stop();
            //Mix_FreeMusic(music);
        }

        if (!music->OpenFromFile(m_musicList[m_iCurrentSongNumber]))
        {
           // std::cerr << "Error Mixer: " << std::endl;
            return -1;
        }
        else
        {
            world.text_output->AddMessage("Playing : " + m_musicList[m_iCurrentSongNumber]);
            music->Play();
        }

    }

    return 0;
}


void AudioManager::AddToPlaylist(const std::string& buffer)
{
    m_musicList.push_back(buffer);
}


void AudioManager::ClearPlaylist()
{
    m_musicList.clear();
}
