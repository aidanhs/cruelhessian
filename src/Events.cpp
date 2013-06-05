/*   Events.cpp
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

#include <string>
#include "WorldMap.h"
#include "AudioManager.h"
#include "WeaponManager.h"
#include "ParserManager.h"
#include "Bonus.h"
#include "Body.h"
#include "Bot.h"
#include "textinout/TextOutput.h"
#include "ClusterGrenade.h"
#include "BotManager.h"


void WorldMap::onTouchBonus(Bot* xbot, Bonus* xbonus)
{

    switch (xbonus->m_iTypeBonus)
    {

    case BONUS_BERSERKER:
        if (!xbot->MODE_BERSERKER && !xbot->MODE_PREDATOR && !xbot->MODE_FLAMEGOD)
        {
            xbonus->killMyself = true;
            xbot->MODE_BERSERKER = true;
            xbot->timeGetSuperbonus = world.getCurrentTime;
            xbot->actLife = Bots.fullLife;
            if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                Audio.take_berserker.Play();
        }

        break;

    case BONUS_CLUSTERS:
        xbonus->killMyself = true;
        xbot->numGrenades = 0;
        xbot->numClusters = 3;
        if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
            Audio.take_grenades.Play();
        break;

    case BONUS_GRENADES:
        if (xbot->numClusters == 0)
        {
            xbonus->killMyself = true;
            xbot->numGrenades += 3;
            if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                Audio.take_grenades.Play();
        }

        break;

    case BONUS_FLAMER:
        if (!xbot->MODE_BERSERKER && !xbot->MODE_PREDATOR && !xbot->MODE_FLAMEGOD)
        {
            xbot->timeGetSuperbonus = world.getCurrentTime;
            xbot->MODE_FLAMEGOD = true;
            xbonus->killMyself = true;
            xbot->actLife = Bots.fullLife;
            if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                Audio.take_flamer.Play();

        }
        break;

    case BONUS_MEDIKITS:
        if (xbot->actLife < Bots.fullLife)
        {
            xbonus->killMyself = true;
            xbot->actLife = Bots.fullLife;
            if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                Audio.take_medikit.Play();
        }
        break;

    case BONUS_PREDATOR:
        if (!xbot->MODE_BERSERKER && !xbot->MODE_PREDATOR && !xbot->MODE_FLAMEGOD)
        {
            xbot->timeGetSuperbonus = world.getCurrentTime;
            xbot->MODE_PREDATOR = true;
            xbonus->killMyself = true;
            xbot->actLife = Bots.fullLife;
            if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
                Audio.take_predator.Play();
        }

        break;

    case BONUS_VESTKIT:
        xbonus->killMyself = true;
        xbot->procVest = 1.0f;
        if (xbot->number == MY_BOT_NR && Parser.SOUNDS_VOL > 0)
            Audio.take_vestkit.Play();
        break;

    default:
        break;

    }

}


void WorldMap::onBonusFall()
{
    if (Parser.SOUNDS_VOL > 0)
        Audio.sound_kitfall[rand()%2].Play();

}



// shooted - ustrzelony
// owner - sprawca

void WorldMap::onReceiveShot(unsigned int shooted, unsigned int owner, float damage)
{
    //if ((bot[shooted]->team == bot[owner]->team && FRIENDLY_FIRE) || bot[shooted]->team != bot[owner]->team)
    if (!bot[shooted]->MODE_FLAMEGOD && !bot[shooted]->MODE_BERSERKER && !bot[shooted]->MODE_PREDATOR && bot[shooted]->team != bot[owner]->team)
    {
        bot[shooted]->actLife -= damage;
        if (bot[shooted]->actLife <= 0)
        {
            if (shooted != owner) bot[owner]->points += 10;
            bot[owner]->killedNr++;
            bot[owner]->killed = shooted;
            bot[shooted]->deaths++;
            bot[shooted]->isKilled = true;
            //std::cout << "IMIERAM" << std::endl;
            bot[shooted]->killer = owner;
            bot[shooted]->actLife = Bots.fullLife;
            bot[shooted]->respawnPeriod = 1000.f*static_cast<float>((rand()%MAX_RESPAWN_TIME + 1));
            if (bot[owner]->chatWinning != "") text_output->AddMessage(bot[owner]->name, bot[owner]->chatWinning);
            if (bot[shooted]->chatDead != "") text_output->AddMessage(bot[shooted]->name, bot[shooted]->chatDead);

            // if I'm dead
            if (shooted == MY_BOT_NR)
            {
                if (Parser.SOUNDS_VOL > 0)
                {
                    Audio.sound_death[rand()%3].Play();
                    Audio.sound_heaven.Play();
                }

                OLD_POS = bot[MY_BOT_NR]->GetPosition();
            }
            // if I killed someone (not me)
            if (owner == MY_BOT_NR && owner != shooted)
            {
                YOU_KILLED = true;
                bot[MY_BOT_NR]->youKilledTime = getCurrentTime;
            }

            // set start values
            bot[shooted]->timerRespawnTime = getCurrentTime;
            bot[shooted]->currentFrame = 0;
            bot[shooted]->timerChangeFrame = 0;
            bot[shooted]->destinationPoint = 0;
            bot[shooted]->leftAmmos = 1;
            bot[shooted]->numGrenades = 5;
        }
    }
}
