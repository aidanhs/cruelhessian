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
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "boost/filesystem/operations.hpp"

#include "../Game.h"
#include "../ParserManager.h"
#include "../decompress/UnLZMA.h"
#include "../decompress/UnTar.h"



//const std::string SITE = "http://cdimage.ubuntu.com/daily/current/";
const std::string SITE = "http://sourceforge.net/";
const std::string SITE_PRE = "projects/cruelhessian/files/cruelhessian-data/";
const std::string SITE_POST = "/download/";
CEGUI::ProgressBar* prog_bar;
CEGUI::Editbox* mess;
double progress;// = 0.0;
CEGUI::PushButton* cancel_butt;
bool DOWN_ON, NO_CONNECTION;

void func_down(void *args);
void func_progress(void *args);
void func_down_list(void *args);
void func_extract_delete(void *args);

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




void func_down(void *args)
{
// tu ssprawdz modyfikacje mod..txt

    thread_data_down *pom = (thread_data_down *)args;

    sf::Http Http;
    Http.SetHost(SITE);

    sf::Http::Request Request;
    Request.SetMethod(sf::Http::Request::Get);
    Request.SetURI(SITE_PRE + pom->file_in + SITE_POST);

    sf::Http::Response Page = Http.SendRequest(Request);

    if (Page.GetStatus() == sf::Http::Response::Ok)
    {
        std::cout<< "OK\n";
        std::ofstream outFile(pom->place_out, std::ios::binary);
        outFile << Page.GetBody();
    }
    else
        std::cout << "FAIL\n";
}


void func_progress(void *args)
{
//std::cout << "START" << std::endl;
    //while (DOWN_ON)
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    CEGUI::ProgressBar* pb = static_cast<CEGUI::ProgressBar*>(winMgr.getWindow("WindowUpdater/Frame/ProgressBar"));
    while (DOWN_ON)
    {
        std::cout << "PROS " << progress << std::endl;
        //prog_bar->setProgress(static_cast<float>(progress));
        pb->setProgress(static_cast<float>(progress));
//        winMgr.getWindow("WindowUpdater/Frame/ProgressBar")->setProperty(
        //  "CurrentProgress", CEGUI::PropertyHelper::floatToString(progress));
    }

    //pthread_exit((void *)args);

    //  return 0;
}


void func_extract_delete(void *args)
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

    //pthread_exit((void *)args);

    //  return 0;
}



int sDownload(const std::string& filein, const std::string& placeout, const std::string& message)
{

    if (DOWN_ON)
    {
        mess->setText(message);

        //pthread_t thread_down;
        thread_data_down str;

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


int sExtractDelete(const std::string& filein, const std::string& placeout, const std::string& message)
{

    if (DOWN_ON)
    {
        mess->setText(message);
        progress += 1.0/static_cast<double>(game.SOLDAT_FOLDER.size());

        thread_data_extr str;
        //pthread_t thread_extract_delete;

        str.file_in = filein;
        str.place_out = placeout;

        //pthread_create(&thread_extract_delete, NULL, func_extract_delete, &str);

        //pthread_join(thread_extract_delete, NULL);

        //   sf::Thread thread_extract_delete(&func_extract_delete, &str);
        //   thread_extract_delete.Launch();

        //pthread_exit(NULL);
    }

    return 0;
}




void func_down_list(void *args)
{
    std::ostringstream oss, siz;

    //pthread_t thread_progress;

    progress = 0.0;
    siz << game.SOLDAT_FOLDER.size();
    DOWN_ON = true;




    //sDownload("natty-alternate-amd64+mac.iso",
   /* sDownload("linux-image-2.6.20-cdlinux_15_i386.deb",
              //Parser.SOL_PATH + "natty-alternate-amd64+mac.iso",
              Parser.SOL_PATH + "linux-image-2.6.20-cdlinux_15_i386.deb",
              "Downloading information file ...");
*/

    sDownload("modifications_server.ini",
              Parser.SOL_PATH + "modifications_server.ini",
              "Downloading information file ...");


    /*
        int n = ParseModif(Parser.SOL_PATH + "modifications_server.ini");
        if (n == 1)
        {
            cancel_butt->setText("Done");
            mess->setText("Nothing to update");
            return 0;
        }
      */
    // std::cout << "STARTwww" << std::endl;
    /*  for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); ++i)
      {

          oss << i+1;
          sDownload(game.SOLDAT_FOLDER[i] + ".tar.lzma",
                    Parser.SOL_PATH + game.SOLDAT_FOLDER[i] + ".tar.lzma",
                    "Downloading " + game.SOLDAT_FOLDER[i] + " ... (" + oss.str() + "/" + siz.str() + ")");
          oss.str("");
      }


      progress = 0.0;

      for (unsigned int i = 0; i < game.SOLDAT_FOLDER.size(); ++i)
      {
          oss << i+1;
          sExtractDelete(Parser.SOL_PATH + game.SOLDAT_FOLDER[i] + ".tar.lzma",
                         Parser.SOL_PATH,
                         "Unpacking " + game.SOLDAT_FOLDER[i] + " ... (" + oss.str() + "/" + siz.str() + ")");
          oss.str("");
      }


      // creating Mp3 and Mods directories, if they were not existed
      boost::filesystem::create_directory(Parser.SOL_PATH + "Mods");
      boost::filesystem::create_directory(Parser.SOL_PATH + "Mp3");
    */
    if (!NO_CONNECTION)
    {
        cancel_butt->setText("Done");
        mess->setText("Click Done button to continue");
    }

    DOWN_ON = false;

    //pthread_exit((void *)args);

//    return 0;
}


bool GUIWindowUpdater::handleClick(const CEGUI::EventArgs &e)
{

    CEGUI::String buttonName = static_cast<const CEGUI::WindowEventArgs&>(e).window->getName();

    //pthread_t thread_down_list;

    if (buttonName == "WindowUpdater/Frame/Start")
    {
        NO_CONNECTION = false;
        start_butt->setEnabled(false);

        //  pthread_create(&thread_down_list, NULL, func_down_list, NULL);
        sf::Thread thread_down_list(&func_down_list);
        thread_down_list.Launch();


        // DOWN_ON = true;
        //   sf::Thread thread_progress(&func_progress);
        //  thread_progress.Launch();

        /* while (DOWN_ON)
         {
             std::cout << "PROS " << progress << std::endl;
             //   prog_bar->setProgress(static_cast<float>(progress));
         }
        */
        //  func_down_list();
        // std::cout << "DALE " << std::endl;
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
