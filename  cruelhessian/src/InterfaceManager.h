#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H


/*   InterfaceManager.h
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



#include "Singleton.h"
#include "TexturesLoader.h"
#include "interface/InterfaceElement.h"
#include "interface/JetBar.h"


class InterfaceManager : public Singleton<InterfaceManager>
{

public:

    InterfaceManager();
    virtual ~InterfaceManager();

    void Draw();
    void ReloadBar();
    JetBar jetBar;

private:

    unsigned char m_cInterfaceAlpha;

    // original
    InterfaceElement m_xHealthIcon;
    InterfaceElement m_xHealthBar;
    InterfaceElement m_xAmmoIcon;
    InterfaceElement m_xAmmoBar;
    InterfaceElement m_xJetIcon;
    InterfaceElement m_xJetBar;
    InterfaceElement m_xFireIcon;
    InterfaceElement m_xFireBar;
    InterfaceElement m_xWeaponName;
    InterfaceElement m_xStatus;
    InterfaceElement m_xTeamBox;
    InterfaceElement m_xVestBar;
    InterfaceElement m_xNade;
	InterfaceElement m_xClusterNade;
    InterfaceElement m_xPingDot;
    InterfaceElement m_xAmmoCount;

    // depending on resolution
    InterfaceElement HealthIcon;
    InterfaceElement HealthBar;
    InterfaceElement AmmoIcon;
    InterfaceElement AmmoBar;
    InterfaceElement JetIcon;

    //InterfaceElement jetBar;
    InterfaceElement FireIcon;
    InterfaceElement FireBar;
    InterfaceElement WeaponName;
    InterfaceElement Status;
    InterfaceElement TeamBox;
    InterfaceElement VestBar;
    InterfaceElement Nade;
	InterfaceElement ClusterNade;
    InterfaceElement PingDot;
    InterfaceElement AmmoCount;

    void Parse();

};

#define Interface InterfaceManager::GetSingleton()

#endif
