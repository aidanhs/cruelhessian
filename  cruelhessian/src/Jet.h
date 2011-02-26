#ifndef JET_H
#define JET_H

/*   Jet.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2011 by Paweł Konieczny <konp84 at gmail.com>
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

//#include "../Drawable.h"
//#include "../TVector2D.h"
//#include "TexturesLoader.h"
//#include "../Singleton.h"


class Tex;


//class Arrow : public Singleton<Arrow>, public Drawable
class Jet //: public InterfaceElement
{

    //const Tex& m_xTexture;
    //TVector2D m_xPosition;
    bool m_bDecrease;
    float m_fAmount;
    int m_iOriginalWidth;
	Jet& operator=(const Jet&) {return *this;}
    float JET_CHANGE_DEC;
    float JET_CHANGE_INC;

public:

    Jet();
    ~Jet() {}

    void Set(int orig_width, const Tex& text);
    float GetAmount() const;
    void Update();
    void Draw() const;
    void StartDecreasing();
    void StopDecreasing();
};


#endif
