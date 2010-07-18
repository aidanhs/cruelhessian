/*   GUIWindowUpdater.cpp
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


#include "GUIWindowUpdater.h"
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include "boost/filesystem/operations.hpp"

#include "../Game.h"
#include "../decompress/UnLZMA.h"
#include "../decompress/UnTar.h"



const std::string SITE = "http://temporary09.eu5.org/files/";
CEGUI::ProgressBar* prog_bar;
CEGUI::Editbox* mess;
double progress;// = 0.0;
CEGUI::PushButton* cancel_butt;
bool DOWN_ON, NO_CONNECTION;

void *func_down(void *args);
void *func_progress(void *args);
void *func_down_list(void *args);
void *func_extract_delete(void *args);

int sDownload(const std::string& filein, const std::string& placeout, const std::string& message);
int sExtractDelete(const std::string& filein, const std::string& placeout, const std::string& message);


typedef struct
{
    std::string file_in;
    std::string place_out;
} thread_data_down;


typedef struct
{
    std::string file_in;
    std::string place_out;
} thread_data_extr;



GUIWindowUpdater::GUIWindowUpdater()
{
    frame = NULL;
}


GUIWindowUpdater::~GUIWindowUpdater()
{
    hide();
}

static int progress_callback(double *client, double t, double d, double utotal, double ulnow)
{

    if (t != 0.0)
        progress = d / t;
    else
        progress = 0.0;

    return 0;
}




void *func_down(void *args)
{
// tu ssprawdz modyfikacje mod..txt
    thread_data_down *pom = (thread_data_down *)args;

    std::string fullPath = SITE + pom->file_in;
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *c = curl_easy_init();
    if (c)
    {
        FILE *plik = fopen(pom->place_out.c_str(), "wb");
        char errorBuffer[CURL_ERROR_SIZE];

        curl_easy_setopt(c, CURLOPT_ERRORBUFFER, errorBuffer);
        curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(c, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(c, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, plik);
        curl_easy_setopt(c, CURLOPT_URL, fullPath.c_str());

        progress = 0;
        CURLcode res = curl_easy_perform(c);

        curl_easy_cleanup(c);

        if (res != CURLE_OK)
        {
            mess->setText(errorBuffer);
            DOWN_ON = false;
            NO_CONNECTION = true;
        }

        fclose(plik);
    }

    pthread_exit((void *)args);

    return 0;
}


void *func_progress(void *args)
{

    while (DOWN_ON)
    {
        //std::cout << " P " << progress << std::endl;
        prog_bar->setProgress(static_cast<float>(progress));
    }

    pthread_exit((void *)args);

    return 0;
}


void *func_extract_delete(void *args)
{

    thread_data_extr *pom = (thread_data_extr *)args;
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

    pthread_exit((void *)args);

    return 0;
}



int sDownload(const std::string& filein, const std::string& placeout, const std::string& message)
{

    if (DOWN_ON)
    {
        mess->setText(message);

        pthread_t thread_down;
        thread_data_down str;

        str.file_in = filein;
        str.place_out = placeout;

        pthread_create(&thread_down, NULL, func_down, &str);

        pthread_join(thread_down, NULL);

        //pthread_exit(NULL);
    }
    return 0;
}


int sExtractDelete(const std::string& filein, const std::string& placeout, const std::string& message)
{

    if (DOWN_ON)
    {
        mess->setText(message);
        progress += 1.0/static_cast<double>(game.SOLDAT_FOLDER.size());

        thread_data_extr str;
        pthread_t thread_extract_delete;

        str.file_in = filein;
        str.place_out = placeout;

        pthread_create(&thread_extract_delete, NULL, func_extract_delete, &str);

        pthread_join(thread_extract_delete, NULL);

        //pthread_exit(NULL);
    }

    return 0;
}




void *func_down_list(void *args)
{
    std::ostringstream oss, siz;
    pthread_t thread_progress;

    progress = 0.0;
    siz << game.SOLDAT_FOLDER.size();
    DOWN_ON = true;

    pthread_create(&thread_progress, NULL, func_progress, NULL);

    sDownload("modifications_server.ini",
              game.SOL_PATH + "modifications_server.ini",
              "Downloading information file ...");


    /*
        int n = ParseModif(game.SOL_PATH + "modifications_server.ini");
        if (n == 1)
        {
            cancel_butt->setText("Done");
            mess->setText("Nothing to update");
            return 0;
        }
      */
   /* for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); i++)
    {
        oss << i+1;
        sDownload(game.SOLDAT_FOLDER[i] + ".tar.lzma",
                  game.SOL_PATH + game.SOLDAT_FOLDER[i] + ".tar.lzma",
                  "Downloading " + game.SOLDAT_FOLDER[i] + " ... (" + oss.str() + "/" + siz.str() + ")");
        oss.str("");
    }
*/

    progress = 0.0;

    for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); i++)
    {
        oss << i+1;
        sExtractDelete(game.SOL_PATH + game.SOLDAT_FOLDER[i] + ".tar.lzma",
                       game.SOL_PATH,
                       "Unpacking " + game.SOLDAT_FOLDER[i] + " ... (" + oss.str() + "/" + siz.str() + ")");
        oss.str("");
    }


    // creating Mp3 and Mods directories, if they were not existed
    boost::filesystem::create_directory(game.SOL_PATH + "Mods");
    boost::filesystem::create_directory(game.SOL_PATH + "Mp3");

    if (!NO_CONNECTION)
    {
        cancel_butt->setText("Done");
        mess->setText("Click Done button to continue");
    }

    DOWN_ON = false;

    pthread_exit((void *)args);

    return 0;
}


bool GUIWindowUpdater::handleClick(const CEGUI::EventArgs &e)
{

    CEGUI::String buttonName = static_cast<const CEGUI::WindowEventArgs&>(e).window->getName();

    pthread_t thread_down_list;

    if (buttonName == "WindowUpdater/Frame/Start")
    {
        NO_CONNECTION = false;
        start_butt->setEnabled(false);

        pthread_create(&thread_down_list, NULL, func_down_list, NULL);
        //pthread_join(thread_down_list, NULL);

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
