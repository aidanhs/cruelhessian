#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

/*   FontManager.h
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


#include <iostream>

#include "Singleton.h"
#include "SDL.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include "Fonts.h"


class FontManager : public Singleton<FontManager>
{
public:

    FontManager();
    virtual ~FontManager();

    std::string Font[2];
    freetype::font_data font[2][50];

    std::vector<unsigned char> textColorGunOnTouch;
    //unsigned char textColorGunOnTouch[4];
    std::vector<unsigned char> textColorGunNormal;
    //unsigned char textColorGunNormal[4];
    std::vector<std::vector<unsigned char> > textCol;
    //unsigned char textCol[5][4];
    std::vector<unsigned char> textGunColor;
    //unsigned char textGunColor[4];
    std::vector<unsigned char> textColor;

    int FontMenuSize;
    int FontConsoleSize;
    int FontBigSize;
    int FontWeaponMenuSize;
    int FontConsoleSmallSize;
    int FontHeightScale;
    int FontMenuBold;
    int FontConsoleBold;
    int FontBigBold;
    int FontWeaponMenuBold;
    int FontConsoleSmallBold;
    int KillConsoleNameSpace;

    //void printText(freetype::font_data& font, const std::string& text, unsigned int* color, float x, float y);
    //void printText(freetype::font_data& font, const std::string& text, unsigned char* color, float x, float y);
    void printText(freetype::font_data& font, const std::string& text, std::vector<unsigned char>& color, float x, float y);
    //void printTextMiddle(freetype::font_data& font, const std::string& text, unsigned int* color, float y);
    //void printTextMiddle(freetype::font_data& font, const std::string& text, unsigned char* color, float y);
    void printTextMiddle(freetype::font_data& font, const std::string& text, std::vector<unsigned char>& color, float y);

};

#define Fonts FontManager::GetSingleton()

#endif

