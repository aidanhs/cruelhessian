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
#include "boost/filesystem/operations.hpp"

#include "soil/SOIL.h"
#include "WorldMap.h"
#include "Game.h"
#include "Globals.h"



int WorldMap::takeScreenshot()
{
    std::string screens_dir = game.CH_HOME_DIRECTORY + "Screens/";
    std::ostringstream oss;
    boost::filesystem::create_directory(screens_dir);
    std::string filename = screens_dir + "screen", temp;

    oss << rand()%RAND_MAX;
    filename += oss.str();
    filename += ".bmp";

    if (SOIL_save_screenshot(filename.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, static_cast<int>(game.MAX_WIDTH), static_cast<int>(game.MAX_HEIGHT)) == 0)
    {
        chat->addMessage("Cannot save screenshot");
        return -1;
    }
    else
    {
        chat->addMessage("Screenshot saved as 'screen" + oss.str() + ".bmp'");
        return 0;
    }
}



int WorldMap::playMusic(int pos)
{

    if (game.MUSIC_VOL > 0 && !game.gMusicList.empty())
    {
        game.CURRENT_SONG_NUMBER += pos;
        if (game.CURRENT_SONG_NUMBER < 0)
        {
            game.CURRENT_SONG_NUMBER = static_cast<int>(game.gMusicList.size()-1);
        }
        else if (game.CURRENT_SONG_NUMBER > static_cast<int>(game.gMusicList.size()-1))
        {
            game.CURRENT_SONG_NUMBER = 0;
        }

        if (Mix_PlayingMusic())
        {
            Mix_HaltMusic();
            Mix_FreeMusic(music);
        }
//std::cout << gMusicList[CURRENT_SONG_NUMBER]<< std::endl;
        chat->addMessage("Playing : " + game.gMusicList[game.CURRENT_SONG_NUMBER]);

        if ((music = Mix_LoadMUS(game.gMusicList[game.CURRENT_SONG_NUMBER].c_str())) == NULL)
        {
            std::cout << "Error Mixer: " << Mix_GetError() << std::endl;
            return -1;
        }
        if (Mix_PlayMusic(music, 0) == -1)
        {
            std::cout << "Unable to play file: " << Mix_GetError() << std::endl;
            return -1;
        }
    }

    return 0;
}
