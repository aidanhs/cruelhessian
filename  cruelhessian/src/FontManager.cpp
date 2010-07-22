/*   FontManager.cpp
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include "FontManager.h"
#include "ParserManager.h"
#include "parser/SimpleIni.h"


FontManager::FontManager()
{
    std::cout << "Starting FontManager ... " << std::endl;

    std::cout << "   loading fonts ... " << std::endl;

    //textColor[] = { 255, 255, 255, 0 };
    textColor.resize(4);
    textColor[0] = 255;
    textColor[1] = 255;
    textColor[2] = 255;
    textColor[3] = 0;
    //textColorGunOnTouch[4] = { 255, 255, 255, 255 };
    textColorGunOnTouch.resize(4);
    textColorGunOnTouch[0] = 255;
    textColorGunOnTouch[1] = 255;
    textColorGunOnTouch[2] = 255;
    textColorGunOnTouch[3] = 255;
    //textColorGunNormal = { 200, 200, 200, 250 };
    textColorGunNormal.resize(4);
    textColorGunNormal[0] = 200;
    textColorGunNormal[1] = 200;
    textColorGunNormal[2] = 200;
    textColorGunNormal[3] = 250;
    //textGunColor = { 247, 247, 247, 250 };
    textGunColor.resize(4);
    textGunColor[0] = 247;
    textGunColor[1] = 247;
    textGunColor[2] = 247;
    textGunColor[3] = 250;


// white, red, blue, yellow, green
    /*textCol[0] = {255, 255, 255, 0};
      textCol[1] = {255, 0, 0, 0};
      textCol[2] = {0, 0, 255, 0};
      textCol[3] = {255, 255, 0, 0};
      textCol[4] = {0, 255, 0, 0};
    */
    textCol.resize(5);
    textCol[0].resize(4);
    textCol[0][0] = 255;
    textCol[0][1] = 255;
    textCol[0][2] = 255;
    textCol[0][3] = 0;

    textCol[1].resize(4);
    textCol[1][0] = 255;
    textCol[1][1] = 0;
    textCol[1][2] = 0;
    textCol[1][3] = 0;

    textCol[2].resize(4);
    textCol[2][0] = 0;
    textCol[2][1] = 0;
    textCol[2][2] = 255;
    textCol[2][3] = 0;

    textCol[3].resize(4);
    textCol[3][0] = 255;
    textCol[3][1] = 255;
    textCol[3][2] = 0;
    textCol[3][3] = 0;

    textCol[4].resize(4);
    textCol[4][0] = 0;
    textCol[4][1] = 255;
    textCol[4][2] = 0;
    textCol[4][3] = 0;

    std::string tmp_str;
    CSimpleIni ini(false, false, false);
    if (ini.LoadFile((Parser.SOL_PATH + "Txt/font.ini").c_str()) < 0)
    {
        std::cerr << "Cannot read 'Txt/font.ini' file" << std::endl;
        return;
    }

    tmp_str = ini.GetValue("FONTS", "Font1");

    if (tmp_str == "Verdana")
    {
        Font[0] = "lucon.ttf";
    }
    else if (tmp_str == "Lucida Console")
    {
        Font[0] = "lucon.ttf";
    }
    else if (tmp_str == "BankGothic Lt BT")
    {
        Font[0] = "bgothl.ttf";
    }
    else if (tmp_str == "Arial")
    {
        Font[0] = "arial.ttf";
    }
    else if (tmp_str == "FreeSans")
    {
        Font[0] = "FreeSans.ttf";
    }

    tmp_str = ini.GetValue("FONTS", "Font2");
    if (tmp_str == "Verdana")
    {
        Font[1] = "lucon.ttf";
    }
    else if (tmp_str == "Lucida Console")
    {
        Font[1] = "lucon.ttf";
    }
    else if (tmp_str == "BankGothic Lt BT")
    {
        Font[1] = "bgothl.ttf";
    }
    else if (tmp_str == "Arial")
    {
        Font[1] = "arial.ttf";
    }
    else if (tmp_str == "FreeSans")
    {
        Font[1] = "FreeSans.ttf";
    }

    FontMenuSize = ini.GetLongValue("FONTS", "FontMenuSize");
    FontConsoleSize = ini.GetLongValue("FONTS", "FontConsoleSize");
    FontBigSize = ini.GetLongValue("FONTS", "FontBigSize");
    FontWeaponMenuSize = ini.GetLongValue("FONTS", "FontWeaponMenuSize");
    FontConsoleSmallSize = ini.GetLongValue("FONTS", "FontConsoleSmallSize");
    FontHeightScale = ini.GetLongValue("FONTS", "FontHeightScale");
    FontMenuBold = ini.GetLongValue("FONTS", "FontMenuBold");
    FontConsoleBold = ini.GetLongValue("FONTS", "FontConsoleBold");
    FontBigBold = ini.GetLongValue("FONTS", "FontBigBold");
    FontWeaponMenuBold = ini.GetLongValue("FONTS", "FontWeaponMenuBold");
    FontConsoleSmallBold = ini.GetLongValue("FONTS", "FontConsoleSmallBold");
    KillConsoleNameSpace = ini.GetLongValue("FONTS", "KillConsoleNameSpace");

    for (int i = 0; i < 2; ++i)
    {
        font[i][FontMenuSize].init((Parser.SOL_PATH + Font[i]).c_str(), FontMenuSize);
        font[i][FontConsoleSize].init((Parser.SOL_PATH + Font[i]).c_str(), FontConsoleSize);
        font[i][FontBigSize].init((Parser.SOL_PATH + Font[i]).c_str(), FontBigSize);
        font[i][FontWeaponMenuSize].init((Parser.SOL_PATH + Font[i]).c_str(), FontWeaponMenuSize);
        font[i][FontConsoleSmallSize].init((Parser.SOL_PATH + Font[i]).c_str(), FontConsoleSmallSize);
    }


    // font1_16.init((Parser.SOL_PATH + Font1).c_str(), 12);
    // font2_12.init((Parser.SOL_PATH + Font2).c_str(), 10);
    // font2_28.init((Parser.SOL_PATH + Font2).c_str(), 26);

//    font1_16 = new OGLFT::Monochrome((Parser.SOL_PATH + Font1).c_str(), 14);
//    font2_12 = new OGLFT::Monochrome((Parser.SOL_PATH + Font2).c_str(), 10);
//    font2_28 = new OGLFT::Monochrome((Parser.SOL_PATH + Font2).c_str(), 28);
}

FontManager::~FontManager()
{

    std::cout << "Removing FontManager ..." << std::endl;

    for (int i = 0; i < 2; ++i)
    {
        font[i][FontMenuSize].clean();
        font[i][FontConsoleSize].clean();
        font[i][FontBigSize].clean();
        font[i][FontWeaponMenuSize].clean();
        font[i][FontConsoleSmallSize].clean();
    }

}

//void FontManager::printTextMiddle(freetype::font_data& fontx, const std::string& text, unsigned int* color, float y)
//void FontManager::printTextMiddle(freetype::font_data& fontx, const std::string& text, unsigned char* color, float y)
void FontManager::printTextMiddle(freetype::font_data& fontx, const std::string& text, std::vector<unsigned char>& color, float y)
{
    printText(fontx, text, color, Parser.MAX_WIDTH/2 - text.length() / 2 - 50, y);
}


//void FontManager::printText(freetype::font_data& fontx, const std::string& text, unsigned int* color, float x, float y)
//void FontManager::printText(freetype::font_data& fontx, const std::string& text, unsigned char* color, float x, float y)
void FontManager::printText(freetype::font_data& fontx, const std::string& text, std::vector<unsigned char>& color, float x, float y)
{
    glPushMatrix();
    glColor3ub(color[0], color[1], color[2]);
    glLoadIdentity();
    freetype::print(fontx, x, Parser.MAX_HEIGHT-y-12.0f, text.c_str());
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}
