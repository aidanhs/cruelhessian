/*   BonusManager.cpp
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


#include "BonusManager.h"
#include "Game.h"
#include "ParserManager.h"
#include "TexturesLoader.h"



BonusManager::BonusManager()
{

    std::cout << "Starting BonusManager ..." << std::endl;

    std::cout << "   loading bonuses' textures ... " << std::endl;

    text_bonus[BONUS_BERSERKER] = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "berserkerkit");
    text_bonus[BONUS_CLUSTERS]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "clusterkit");
    text_bonus[BONUS_FLAMER]    = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "flamerkit");
    text_bonus[BONUS_GRENADES]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "grenadekit");
    text_bonus[BONUS_MEDIKITS]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "medikit");
    text_bonus[BONUS_PREDATOR]  = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "predatorkit");
    text_bonus[BONUS_VESTKIT]   = SOIL_LoadTextureEx2(Parser.SOL_PATH + "Textures/Objects/", "vestkit");


}

BonusManager::~BonusManager()
{

    std::cout << "Removing BonusManager ..." << std::endl;

}
