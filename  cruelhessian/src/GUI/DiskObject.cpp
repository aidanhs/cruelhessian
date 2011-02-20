/*   DiskObject.cpp
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
 * aint with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include "DiskObject.h"
#include "boost/filesystem/fstream.hpp"
#include <iostream>



DiskObject::DiskObject() :
    mStartPath("")
{
#ifdef _WIN32
    findWinMainDir();
#endif
}

void DiskObject::upDir()
{

    mStartPath = boost::filesystem::path(mStartPath).parent_path().file_string();
    fillDir(mStartPath, "");

}


void DiskObject::fillDir(const std::string& dir1, const std::string& dir2)
{

    if (dir1 == ".")
    {
        mStartPath = boost::filesystem::current_path().file_string();
    }
    /*else if (dir1 == "..")
    {
        mStartPath = boost::filesystem::path(mStartPath).parent_path().file_string();
    }*/
    else
    {
        boost::filesystem::path p(dir1);
        p /= dir2;
        mStartPath = p.string();
    }

    try
    {
        boost::filesystem::directory_iterator dir_iter(mStartPath);
    }
    catch (boost::filesystem::basic_filesystem_error<boost::filesystem::path> e )
    {
        mStartPath = boost::filesystem::path(mStartPath).parent_path().file_string();
        return;
    }

    boost::filesystem::directory_iterator dir_iter(mStartPath), dir_end;

    dirList.clear();

    for (; dir_iter != dir_end; ++dir_iter)
    {
        if (boost::filesystem::is_directory(dir_iter->status()))
            dirList.push_back((*dir_iter).filename());
    }

    std::sort(dirList.begin(), dirList.end());

#ifdef _WIN32

    if (mStartPath != "")
    {
        if (std::find(WinDisks.begin(), WinDisks.end(), mStartPath) == WinDisks.end())
            dirList.insert(dirList.begin(), std::string(".."));
        dirList.insert(dirList.begin(), std::string("     MAIN"));
    }

#else

    if (mStartPath != "/")
    {
        dirList.insert(dirList.begin(), std::string(".."));
        dirList.insert(dirList.begin(), std::string("     MAIN"));
    }

#endif

}


#ifdef _WIN32
void DiskObject::findWinMainDir()
{

    char disk[4];
    disk[1] = ':';
    disk[2] = '/';
    disk[3] = '\0';
    bool fileExists;

    for (char k = 'C'; k <= 'Z'; k++)
    {
        disk[0] = k;

        fileExists = false;

        try
        {
            fileExists = boost::filesystem::exists(disk);
        }
        catch (boost::filesystem::basic_filesystem_error<boost::filesystem::path> e )
        {
            fileExists = false;
        }

        if (fileExists)
        {
            WinDisks.push_back(std::string(disk));
        }

    }

}


void DiskObject::fillWinMainDir()
{
    dirList.clear();
    dirList = WinDisks;

    mStartPath = "";
}

#endif
