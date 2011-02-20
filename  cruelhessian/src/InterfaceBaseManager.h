#ifndef INTERFACE_BASE_MANAGER_H
#define INTERFACE_BASE_MANAGER_H

/*   InterfaceBaseManager.h
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



#include "TexturesLoader.h"
#include "Singleton.h"



class InterfaceBaseManager : public Singleton<InterfaceBaseManager>
{

public:

    InterfaceBaseManager(void);
    virtual ~InterfaceBaseManager(void);

    void Draw();

    Tex text_arrow;
    Tex text_mouse;
    Tex text_deaddot;
    Tex text_smalldot;

};

#define InterfaceBase InterfaceBaseManager::GetSingleton()

#endif
