#ifndef WINDOW_GUNS_H
#define WINDOW_GUNS_H

/*   WindowGuns.h
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


#include "Singleton.h"
#include "TexturesLoader.h"
#include "TVector2D.h"


class WindowGuns : public Singleton<WindowGuns>
{

    TVector2D m_mouse_pos;
    float x, y, w, h;
    int m_number;
    void draw_help(const Tex& texture, float dx, float dy) const;
    //char *m_cChar;

public:

    WindowGuns();
    ~WindowGuns();

    void Draw();
    void Update(const TVector2D& pos);
    int select() const;
};


#endif

