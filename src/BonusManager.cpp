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
#include "PhysicsManager.h"
#include "TexturesLoader.h"
#include "WorldMap.h"
#include "Map.h"
#include "Bonus.h"



BonusManager::BonusManager(void)
{

    std::cout << "Starting BonusManager ..." << std::endl;

    std::cout << "   loading bonuses' textures ... " << std::endl;

    text_bonus[BONUS_BERSERKER] = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "berserkerkit");
    text_bonus[BONUS_CLUSTERS]  = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "clusterkit");
    text_bonus[BONUS_FLAMER]    = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "flamerkit");
    text_bonus[BONUS_GRENADES]  = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "grenadekit");
    text_bonus[BONUS_MEDIKITS]  = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "medikit");
    text_bonus[BONUS_PREDATOR]  = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "predatorkit");
    text_bonus[BONUS_VESTKIT]   = Texture::LoadExt(Parser.GAME_PATH + "Textures/Objects/", "vestkit");

}

BonusManager::~BonusManager(void)
{

    std::cout << "Removing BonusManager ... DONE" << std::endl;

}


int BonusManager::LoadBonuses(void)
{

    std::cout << "   loading bonuses information ..." << std::endl;

    bonus.resize(7);

    for (int i = 0; i < world.map->spawnpointCount; ++i)
    {
        if (world.map->spawnpoint[i].active)
        {
            if (world.map->spawnpoint[i].team == world.map->stBERSERKER && Parser.BONUSES[BONUS_BERSERKER])
                bonus[BONUS_BERSERKER].push_back(i);
            else if (world.map->spawnpoint[i].team == world.map->stCLUSTERS && Parser.BONUSES[BONUS_CLUSTERS])
                bonus[BONUS_CLUSTERS].push_back(i);
            else if (world.map->spawnpoint[i].team == world.map->stFLAMER && Parser.BONUSES[BONUS_FLAMER])
                bonus[BONUS_FLAMER].push_back(i);
            else if (world.map->spawnpoint[i].team == world.map->stGRENADES)
                bonus[BONUS_GRENADES].push_back(i);
            else if (world.map->spawnpoint[i].team == world.map->stMEDKITS)
                bonus[BONUS_MEDIKITS].push_back(i);
            else if (world.map->spawnpoint[i].team == world.map->stPREDATOR && Parser.BONUSES[BONUS_PREDATOR])
                bonus[BONUS_PREDATOR].push_back(i);
            else if (world.map->spawnpoint[i].team == world.map->stVEST && Parser.BONUSES[BONUS_VESTKIT])
                bonus[BONUS_VESTKIT].push_back(i);
        }
    }

    // laduj wszystko co jest
    // fix it !!!!!!!!!!!!
    TVector2D p;
    int num;
    for (unsigned int i = 0; i < 7; ++i)
    {
        if (Parser.BONUSES[i])
            for (unsigned int j = 0; j < bonus[i].size(); ++j)
            {
                num = bonus[i][j];
                p.x = static_cast<float>(world.map->spawnpoint[num].x);
                p.y = static_cast<float>(world.map->spawnpoint[num].y);
                Bonus *newbon = new Bonus(p, i);

				Physics.m_movingObj.push_back(newbon);
            }
    }

    return 0;
}



int BonusManager::RemoveIfNeeded(void)
{
	return 0;
/*    BodyUserData* b;

    for (b2Body *body = world.spaceMap->GetBodyList(); body; body = body->GetNext())
    {
        b = static_cast<BodyUserData*>(body->GetUserData());

        if (b->type == TYPE_BONUS && static_cast<Bonus*>(b->pointer)->killMyself)
        {
            world.spaceMap->DestroyBody(body);
        }
    }
*/
}
