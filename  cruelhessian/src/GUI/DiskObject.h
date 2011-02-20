#ifndef DISK_OBJECT_H
#define DISK_OBJECT_H


/*   DiskObject.h
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



#include <string>
#include <vector>


class DiskObject
{
#ifdef _WIN32
    std::vector<std::string> WinDisks;
    void findWinMainDir();
#endif

public:
    DiskObject();
    ~DiskObject() {}

    void upDir();

#ifdef _WIN32
    void fillWinMainDir();
#endif

    void fillDir(const std::string& dir1, const std::string& dir2);

    std::vector<std::string> dirList;
    std::string mStartPath;
};


#endif
