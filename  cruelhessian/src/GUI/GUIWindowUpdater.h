/*   GUIWindowUpdater.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Pawel Konieczny <konp84 at gmail.com>
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



#ifndef WUP_H
#define WUP_H

#include "CEGUI.h"
#include <iostream>

//class CEGUI;


class GUIWindowUpdater
{
public:
    GUIWindowUpdater();
    virtual ~GUIWindowUpdater();

    void show(CEGUI::Window* root);
    void hide();
    // double getResult();

private:
    virtual bool handleClick(const CEGUI::EventArgs& e);
    virtual void wireEvents();
    //int Download(const std::string& site, const std::string& file, const std::string& dst_place, const std::string& message);
    CEGUI::Window* frame;
    int Download(const std::string& filein, const std::string& placeout, const std::string& message);
    //void *show_progress(void *args);
//    CEGUI::ProgressBar* prog_bar;
    CEGUI::PushButton* start_butt;

};


#endif
