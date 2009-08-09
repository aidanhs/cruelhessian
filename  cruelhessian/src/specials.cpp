/*   specials.cpp
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
#include <fstream>
#include "regex.h"
#ifndef WIN32
#include <dirent.h>
#include <sys/stat.h>
#else
#include <direct.h>
#endif
#include "soil/SOIL.h"
#include "worldmap.h"
#include "globals.h"



int WorldMap::takeScreenshot()
{
    std::string screens_dir = CH_HOME_DIRECTORY + "Screens/";
#ifdef WIN32
    _mkdir(screens_dir.c_str());
#else
    mkdir(screens_dir.c_str(), 0755);
#endif
    std::ostringstream oss;
    std::string filename = screens_dir + "screen";

    int part2 = rand()%RAND_MAX;
    oss << part2;
    filename += oss.str();
    filename += ".bmp";

    if (SOIL_save_screenshot(filename.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, MAX_WIDTH, MAX_HEIGHT) == 0)
    {
        std::cout << "Cannot save screenshot" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Screenshot saved as 'screen" << part2 << ".bmp" << std::endl;
        return 0;
    }
}



int WorldMap::playMusic(int pos)
{
    if (MUSIC_VOL > 0 && !gMusicList.empty())
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

        std::cout << "Loading track : " << gMusicList[CURRENT_SONG_NUMBER] << std::endl;

        if (Mix_PlayingMusic())
        {
            Mix_HaltMusic();
            Mix_FreeMusic(music);
        }

        if ((music = Mix_LoadMUS(gMusicList[CURRENT_SONG_NUMBER].c_str())) == NULL)
        {
            std::cout << "Error Mixer: " << Mix_GetError() << std::endl;
            return 1;
        }
        if (Mix_PlayMusic(music, 0) == -1)
        {
            std::cout << "Unable to play file: " << Mix_GetError() << std::endl;
            return 1;
        }
    }
    return 0;
}
