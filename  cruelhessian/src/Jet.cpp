/*   Jet.cpp
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


#include "Jet.h"
//#include <cstdio>
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


Jet::Jet() :
    m_bDecrease(false),
    m_fAmount(1.0f),
    JET_CHANGE_DEC(0.009f),
    JET_CHANGE_INC(0.005f)
{
}

float Jet::GetAmount() const
{
    return m_fAmount;
}



void Jet::Update()
{

    if (m_bDecrease)
    {
        m_fAmount -= JET_CHANGE_DEC;
        if (m_fAmount < 0.0)
            m_fAmount = 0.0;
    }
    else
    {
        m_fAmount += JET_CHANGE_INC;
        if (m_fAmount > 1.0)
            m_fAmount = 1.0;
    }

}


void Jet::StartDecreasing()
{
    m_bDecrease = true;
}


void Jet::StopDecreasing()
{
    m_bDecrease = false;
}
