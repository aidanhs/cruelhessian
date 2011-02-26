/*   GUIWindowUpdater.cpp
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


#include "GUIWindowUpdater.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Network.hpp>
#include "boost/filesystem/operations.hpp"

#include "../Game.h"
#include "../ParserManager.h"
#include "../decompress/UnLZMA.h"
#include "../decompress/UnTar.h"
#include "../parser/SimpleIni.h"



//const std::string SITE = "http://cdimage.ubuntu.com/daily/current/";
//const std::string SITE = "http://sourceforge.net/";
//const std::string SITE_PRE = "projects/cruelhessian/files/cruelhessian-data/";
//const std::string SITE_POST = "/download/";
CEGUI::ProgressBar* prog_bar;
CEGUI::Editbox* mess;
double progress;// = 0.0;
CEGUI::PushButton* cancel_butt;
bool DOWN_ON, NO_CONNECTION;

//void func_down(void *args);
//void func_progress(void *args);
//void func_down_list(void *args);
//void func_extract_delete(void *args);

//int sDownload(const std::string& filein, const std::string& placeout, const std::string& message);
//int sExtractDelete(const std::string& filein, const std::string& placeout, const std::string& message);

/*
typedef struct
{
    std::string site1;
    std::string site2;
    std::string file_in;
    std::string place_out;
} thread_data;
*/
/*
typedef struct
{
    std::string file_in;
    std::string place_out;
} thread_data_extr;
*/


GUIWindowUpdater::GUIWindowUpdater()
{
    frame = NULL;
}


GUIWindowUpdater::~GUIWindowUpdater()
{
    hide();
}
/*
static int progress_callback(double *client, double t, double d, double utotal, double ulnow)
{

    if (t != 0.0)
        progress = d / t;
    else
        progress = 0.0;

    return 0;
}


*/

void sDownload(const std::string& site_pre, const std::string& site_post, const std::string& place_out, int mode)
{
// tu ssprawdz modyfikacje mod..txt

    std::cout << " W " << site_pre << " " << site_post << " " << place_out << "\n";
    sf::Http Http;
    Http.SetHost(site_pre);

    sf::Http::Request Request;
    Request.SetMethod(sf::Http::Request::Get);
    Request.SetURI(site_post);

    sf::Http::Response Page = Http.SendRequest(Request);

    if (Page.GetStatus() == sf::Http::Response::Ok)
    {
//        std::cout<< "OK\n";
        std::ofstream outFile;

        if (mode == 1)
            outFile.open(place_out.c_str(), std::ofstream::out | std::ofstream::binary);
        else if (mode == 0)
            outFile.open(place_out.c_str(), std::ofstream::out);
        outFile.write((char *)Page.GetBody().c_str(), Page.GetBody().size());
        outFile.close();
    }
    else
        std::cout << "FAIL\n";
}

/*
void func_extract_delete(void *args)
{

    thread_data *pom = (thread_data *)args;
    char out[10000], out2[10000];
    std::string dst_file(pom->file_in.begin(), pom->file_in.end()-5);
    UnLZMA un1;
    UnTar un2;

    std::cout << " Extracting " << pom->file_in << " (1st stage)" << std::endl;

    un1.Extract(pom->file_in.c_str(), dst_file.c_str());

    remove(pom->file_in.c_str());


    std::cout << " Extracting " << dst_file << " (2nd stage)" << std::endl;

    strcpy(out, dst_file.c_str());
    strcpy(out2, pom->place_out.c_str());

    un2.Extract(out, out2);

    remove(dst_file.c_str());

    std::cout << "Extracted into: " << pom->place_out << std::endl << std::endl;

    //pthread_exit((void *)args);

    //  return 0;
}
*/

/*
int sDownload(const std::string& filein, const std::string& placeout, const std::string& message)
{

    if (DOWN_ON)
    {
        mess->setText(message);

        //pthread_t thread_down;
        thread_data str;

        str.file_in = filein;
        str.place_out = placeout;

//        pthread_create(&thread_down, NULL, func_down, &str);
        sf::Thread thread_down(&func_down, &str);
        thread_down.Launch();
        //pthread_join(thread_down, NULL);

        //pthread_exit(NULL);
    }
    return 0;
}
*/
/*
int sDownloadLinks(const std::string& sit1, const std::string& sit2, const std::string& message)
{

    //if (DOWN_ON)
    {
        mess->setText(message);

        //pthread_t thread_down;
        //thread_data str;

        //str.file_in = filein;
        //str.place_out = placeout;

//        pthread_create(&thread_down, NULL, func_down, &str);
        sf::Thread thread_down(&func_down, &str);
        thread_down.Launch();
        //pthread_join(thread_down, NULL);

        //pthread_exit(NULL);
    }
    return 0;
}
*/

int sExtractDelete(const std::string& filein, const std::string& placeout)
{

    if (DOWN_ON)
    {
        //mess->setText(message);
        progress += 1.0/static_cast<double>(game.SOLDAT_FOLDER.size());



        char out[10000], out2[10000];
        std::string dst_file(filein.begin(), filein.end()-5);
        UnLZMA un1;
        UnTar un2;

        std::cout << " Extracting " << filein << " (1st stage)" << std::endl;

        un1.Extract(filein.c_str(), dst_file.c_str());

        remove(filein.c_str());


        std::cout << " Extracting " << dst_file << " (2nd stage)" << std::endl;

        strcpy(out, dst_file.c_str());
        strcpy(out2, placeout.c_str());

        un2.Extract(out, out2);

        remove(dst_file.c_str());

        std::cout << "Extracted into: " << placeout << std::endl << std::endl;

    }

    return 0;
}




void func_down_list(void *args)
{

    progress = 0.0;
    // siz << game.SOLDAT_FOLDER.size();
    DOWN_ON = true;

/*
    // download links
    sDownload("cruelhessian.konp.eu", "cruelhessian-links.txt", Parser.GAME_PATH + "cruelhessian-links.txt", 0);

    CSimpleIni ini(false, false, false);

    if (ini.LoadFile((Parser.GAME_PATH + "cruelhessian-links.txt").c_str()) < 0)
    {
        std::cout << "cannot read 'links.txt' file" << std::endl;
        return;
    }

    std::vector<std::string> Link(game.SOLDAT_FOLDER.size());

    std::string Server = ini.GetValue("main", "Server");


    for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); ++i)
    {
        Link[i] = ini.GetValue(game.SOLDAT_FOLDER[i].c_str(), "Path");
    }

    for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); ++i)
    {
        sDownload(Server, Link[i], Parser.GAME_PATH + game.SOLDAT_FOLDER[i] + ".tar.lzma", 1);
    }

    // progress = 0.0;
*/
    for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); ++i)
    {
        sExtractDelete(Parser.GAME_PATH + game.SOLDAT_FOLDER[i] + ".tar.lzma", Parser.GAME_PATH);
    }


    if (!NO_CONNECTION)
    {
        cancel_butt->setText("Done");
        mess->setText("Click Done button to continue");
    }

    DOWN_ON = false;

}


bool GUIWindowUpdater::handleClick(const CEGUI::EventArgs &e)
{

    CEGUI::String buttonName = static_cast<const CEGUI::WindowEventArgs&>(e).window->getName();

    if (buttonName == "WindowUpdater/Frame/Start")
    {
        NO_CONNECTION = false;
        start_butt->setEnabled(false);

        sf::Thread thread_down_list(&func_down_list);
        thread_down_list.Launch();
        std::cout << "KAKA\n";

    }
    else if (buttonName == "WindowUpdater/Frame/Cancel")
    {
        DOWN_ON = false;
        hide();

        //pthread_exit(NULL);
    }
    return false;
}


void GUIWindowUpdater::wireEvents()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    winMgr.getWindow("WindowUpdater/Frame/Start")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIWindowUpdater::handleClick, this));
    winMgr.getWindow("WindowUpdater/Frame/Cancel")->
    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIWindowUpdater::handleClick, this));
}


void GUIWindowUpdater::show(CEGUI::Window* root)
{

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    frame = winMgr.loadWindowLayout("wup.layout");

    prog_bar = static_cast<CEGUI::ProgressBar*>(winMgr.getWindow("WindowUpdater/Frame/ProgressBar"));
    start_butt = static_cast<CEGUI::PushButton*>(winMgr.getWindow("WindowUpdater/Frame/Start"));
    cancel_butt = static_cast<CEGUI::PushButton*>(winMgr.getWindow("WindowUpdater/Frame/Cancel"));
    mess = static_cast<CEGUI::Editbox*>(winMgr.getWindow("WindowUpdater/Frame/Message"));

    root->addChildWindow(frame);
    wireEvents();

}


void GUIWindowUpdater::hide()
{
    if (frame)
        frame->destroy();
    frame = NULL;
}
