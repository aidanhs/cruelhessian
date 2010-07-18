/*   gui_interface.cpp
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
//#include <locale.h>
//#include <libintl.h>
#include "boost/filesystem/fstream.hpp"

#include "../GUI.h"
//#include "../globals.h"
#include "../Game.h"

//#define _(string) gettext(string)



void GUI::showInterfaces(const std::string& name)
{

    std::string str;
    std::string fold_in = name + "Interface-gfx/";
    std::string fold_cus = name + "Custom-Interfaces/";
    boost::filesystem::directory_iterator end;

    if (!boost::filesystem::exists(fold_in))
    {
        std::cout << "'Interface-gfx' directory doesn't exist !" << std::endl;
        exit(EXIT_FAILURE);
    }

    mInterfaces->addItem(new MyListItem("Default"));

    if (!boost::filesystem::exists(fold_cus))
    {
        std::cout << "'Custom-Interfaces' directory doesn't exist !" << std::endl;
    }
    else
    {
        for (boost::filesystem::directory_iterator iter(fold_cus); iter != end; ++iter)
        {
            str.assign(iter->path().string().begin() + fold_cus.length(), iter->path().string().end());
            mInterfaces->addItem(new MyListItem(str));
            //  str.assign(dp->d_name);
            // if ((str != ".") && (str != ".."))
            //   mInterfaces->addItem(new MyListItem(str));
        }
    }
    //mInterfaces->setSortingEnabled(true);
    mInterfaces->setItemSelectState(size_t(0), true);

}