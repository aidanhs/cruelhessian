/*   GUIWindowFolderSelector.cpp
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

// based on http://www.cegui.org.uk/wiki/index.php/Folder_Selector


#include "GUIWindowFolderSelector.h"
#include "DiskObject.h"


GUIWindowFolderSelector::GUIWindowFolderSelector()
{
    frame = NULL;
    disk = new DiskObject();
}


GUIWindowFolderSelector::~GUIWindowFolderSelector()
{
    delete disk;
    disk = NULL;
    hide();
}


bool GUIWindowFolderSelector::handleClick(const CEGUI::EventArgs &e)
{

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    CEGUI::String buttonName = static_cast<const CEGUI::WindowEventArgs&>(e).window->getName();

    if (buttonName == "FolderSelector/Frame/FolderList")
    {
        CEGUI::Listbox* list = static_cast<CEGUI::Listbox*>(winMgr.getWindow("FolderSelector/Frame/FolderList"));
        CEGUI::ListboxTextItem* item = static_cast<CEGUI::ListboxTextItem*>(list->getFirstSelectedItem());

        if (item == NULL)
            return 0;

        std::string text = disk->dirList[item->getID()];

        // gdy kliknelismy na root
        if (text == "     MAIN")
        {
            // show main directory
#ifdef _WIN32
            disk->fillWinMainDir();
#else
            disk->fillDir("/", "");
#endif
        }
        // gdy kliknelismy na dwukropek
        else if (text == "..")
        {
            disk->upDir();
        }
        // gdy kliknelismy na nazwe katalogu
        else
        {
            disk->fillDir(disk->mStartPath, text);
        }
        updateFolderList();
    }
    if (buttonName == "FolderSelector/Frame/Cancel")
    {
        hide();
    }
    if (buttonName == "FolderSelector/Frame/Ok")
    {
        hide();
    }
    return false;
}



const std::string GUIWindowFolderSelector::getResult()
{
    return disk->mStartPath;
}


void GUIWindowFolderSelector::wireEvents()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    winMgr.getWindow("FolderSelector/Frame/Ok")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIWindowFolderSelector::handleClick, this));
    winMgr.getWindow("FolderSelector/Frame/Cancel")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIWindowFolderSelector::handleClick, this));
    winMgr.getWindow("FolderSelector/Frame/FolderList")->
    subscribeEvent(CEGUI::Listbox::EventMouseDoubleClick, CEGUI::Event::Subscriber(&GUIWindowFolderSelector::handleClick, this));
}


void GUIWindowFolderSelector::show(CEGUI::Window* root)
{

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    frame = winMgr.loadWindowLayout("fs.layout");
    root->addChildWindow(frame);
    wireEvents();
    disk->fillDir(".", "");
    updateFolderList();

}


void GUIWindowFolderSelector::hide()
{
    if (frame)
        frame->destroy();
    frame = NULL;
}


void GUIWindowFolderSelector::updateFolderList()
{

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    CEGUI::Listbox* list = static_cast<CEGUI::Listbox*>(winMgr.getWindow("FolderSelector/Frame/FolderList"));
    list->resetList();

    for (unsigned int n = 0; n < disk->dirList.size(); n++)
    {
        CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(disk->dirList[n]);
        item->setID(n);
        list->addItem(item);
    }

    CEGUI::Editbox* box = static_cast<CEGUI::Editbox*>(winMgr.getWindow("FolderSelector/Frame/Address"));
    box->setText(disk->mStartPath);

}
