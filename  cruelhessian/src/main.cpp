/*   main.cpp
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

#include <libintl.h>
#ifndef WIN32
	#include <dirent.h>
	#include <sys/stat.h>
#endif

#include "globals.h"
#include "gui.h"



int main(int argc, char *argv[])
{

    CH_GUI ch;

    TVector2D a(3,4);
    a.normalize();
    std::cout << " QWE " << a.x << " "<< a.y;

#ifndef WIN32
    CH_DATA_DIRECTORY = "/usr/share/cruelhessian/data/";
    //CH_DATA_DIRECTORY = "../data/";
    CH_HOME_DIRECTORY = getenv("HOME");
    CH_HOME_DIRECTORY += "/.config/cruelhessian/";
    mkdir(CH_HOME_DIRECTORY.c_str(), 0755);
#endif

    CH_CONFIG_FILE = CH_HOME_DIRECTORY + "options.ini";
    setlocale(LC_ALL, "");
    bindtextdomain("pl", "locale");
    textdomain("pl");

    ch.init();
    ch.run();
    ch.quit();

    return 0;
}
