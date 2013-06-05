#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

/*   FontManager.h
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include <iostream>

#include "Singleton.h"
#include <SFML/Graphics.hpp>


typedef struct
{
    float size;
    bool bold;
} FontStruct;


class FontManager : public Singleton<FontManager>
{
    std::string m_xFont[2];

public:

    FontManager();
    virtual ~FontManager();

    sf::Font font[2];

    std::vector<unsigned char> textColorGunOnTouch;
    std::vector<unsigned char> textColorGunNormal;
    std::vector<unsigned char> textGunColor;
    std::vector<unsigned char> textColor;
    std::vector<std::vector<unsigned char> > textCol;

    FontStruct FontMenu;
    FontStruct FontConsole;
    FontStruct FontConsoleSmall;
    FontStruct FontBig;
    FontStruct FontWeaponMenu;

   /* int FontMenuSize;
    int FontConsoleSize;
    int FontBigSize;
    int FontWeaponMenuSize;
    int FontConsoleSmallSize;
    int FontHeightScale;
    int FontMenuBold;
    int FontConsoleBold;
    int FontBigBold;
    int FontWeaponMenuBold;
    int FontConsoleSmallBold;*/

    float FontHeightScale;
    int KillConsoleNameSpace;

    void printText(const sf::Font& font, const FontStruct& font_info, const std::string& text, const std::vector<unsigned char>& color, const float x, const float y) const;
    void printTextMiddle(const sf::Font& font, const FontStruct& font_info, const std::string& text, const std::vector<unsigned char>& color, const float y) const;

};

#define Fonts FontManager::GetSingleton()

#endif

