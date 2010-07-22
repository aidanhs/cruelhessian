#ifndef BONUS_MANAGER_H
#define BONUS_MANAGER_H

/*   BonusManager.h
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
#include "Tex.h"


class BonusManager : public Singleton<BonusManager>
{
    public:
        BonusManager();
        ~BonusManager();

        Tex text_bonus[7];
};

#define Bonuses BonusManager::GetSingleton()

#endif // BONUS_MANAGER_H
