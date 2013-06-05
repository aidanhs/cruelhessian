/*   GUIWindowNoSoldat.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010 by Paweł Konieczny <konp84 at gmail.com>
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



#include "GUIWindowNoSoldat.h"


GUIWindowNoSoldat::GUIWindowNoSoldat()
{
    frame = NULL;
}


GUIWindowNoSoldat::~GUIWindowNoSoldat()
{
    hide();
}


bool GUIWindowNoSoldat::handleClick(const CEGUI::EventArgs &e)
{

    CEGUI::String buttonName = static_cast<const CEGUI::WindowEventArgs&>(e).window->getName();

    if (buttonName == "WindowNoSoldat/Frame/Ok")
    {
        hide();
    }
    return false;
}


void GUIWindowNoSoldat::wireEvents()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    winMgr.getWindow("WindowNoSoldat/Frame/Ok")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIWindowNoSoldat::handleClick, this));
}


void GUIWindowNoSoldat::show(CEGUI::Window* root)
{

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    frame = winMgr.loadWindowLayout("wns.layout");
    root->addChildWindow(frame);
    wireEvents();

}


void GUIWindowNoSoldat::hide()
{
    if (frame)
        frame->destroy();
    frame = NULL;
}
