/*   Bot.cpp
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


#include "Bot.h"
#include "WorldMap.h"
#include "BotManager.h"
#include "PhysicsManager.h"
#include "Background.h"
#include "Body.h"
#include "ParserManager.h"
#include "InterfaceManager.h"
#include "AudioManager.h"
#include "WeaponManager.h"
#include "Map.h"
#include "WorldMap.h"
#include "Mouse.h"
#include "Weapon.h"
#include "Jet.h"
#include "Grenade.h"
#include "Cluster.h"
#include "ContactListener.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


//const float _180overpi = 57.29f;


Bot::Bot(const std::string& _name, const TVector2D& spawn, int gunmodel, TEAM _team, unsigned int bot_nr, int dest=0) :
    name(_name),
    team(_team),
    number(bot_nr),
    MODE_BERSERKER(false),
    MODE_PREDATOR(false),
    MODE_FLAMEGOD(false),
    movementType(STOI),
    movementDirection(LEFT),
    isKilled(false),
    isReloading(false),
    isShooting(false),
    wantToFly(false),
//    fly(false),
 //   jump(false),
    collisionWithPoly(false),
    shotPoint(TVector2D(0,0)),
    gunModel(gunmodel),
//    procJet(1.0f),
    procVest(0.0f),
    actLife(Bots.fullLife),
    respawnPeriod(0),
    lastShotTime(0),
    currentFrame(0),
    timerChangeFrame(0),
    timerRespawnTime(0),
    youKilledTime(0),
    destinationPoint(0),
    leftAmmos(1),
    numGrenades(Parser.MAX_GRENADES/2),
    points(0),
    numClusters(0),
    deaths(0),
    killedNr(0),
    ping(0),
    moveLeft(false),
    moveRight(false),
    moveUp(false),
    moveDown(false),
    changeMove(true),
    chatKill(""),
    chatDead(""),
    chatLowhealth(""),
    chatSeeEnemy(""),
    chatWinning(""),
    wayNumber(0),
    killer(0),
    killed(0),
    startReloadingTime(0),
    timeGetSuperbonus(0)
{

    weapon[0] = new Weapon(spawn, gunModel, number);
    weapon[1] = new Weapon(spawn, 0, number);

    usedWeaponNr = 0;

    jet = new Jet();

    color.resize(4);
    for (int i = 0; i < 4; ++i)
        color[i].resize(3);


    Set(spawn, TVector2D(0,0), 8, 6, 8, 80);
    //body->SetDensity(0.9);
    SetCollisionCallback(HandleContact);
    GetInvInertia() = 0.0f;
    GetInertia() = 0.0f;
    GetAngVelocity() = 0.0f;
    SetOrientation(0.0f);
    type = TYPE_PLAYER;
    // Physics.m_movingObj.push_back(this);

}



bool Bot::is_inside(int x=0, int y=0)
{
//    return ((x > position.x - r) && (x < position.x + r) &&
//            (y > position.y - r) && (y < position.y + r));
    return false;
}



void Bot::draw_gostek_help(int part, BODY_COLOR xbody) const
{

    BODY xname = Bots.indices[part];
    int dir = movementDirection;

    if (xbody != NONE)
        glColor3ub(color[xbody][0], color[xbody][1], color[xbody][2]);

    glPushMatrix();

//   glTranslatef(position.x + Bots.frame[movementType][part][currentFrame][dir].x + Bots.renderInfo[part].x,
    //              position.y + Bots.frame[movementType][part][currentFrame][dir].y + Bots.renderInfo[part].y, 0.0f);

    glTranslatef(GetPosition().x + Bots.frame[movementType][part][currentFrame][dir].x + Bots.renderInfo[part].x,
                 GetPosition().y + Bots.frame[movementType][part][currentFrame][dir].y + Bots.renderInfo[part].y, 0.0f);

    glRotatef(Bots.frame[movementType][part][currentFrame][dir].r, 0.0f, 0.0f, 1.0f);

    glTranslatef(-Bots.renderInfo[part].x,
                 -Bots.renderInfo[part].y, 0.0f);


    glBindTexture(GL_TEXTURE_2D, Bots.gostek[xname][dir].tex);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1);
    glVertex2i(0, 0);
    glTexCoord2i(1, 1);
    glVertex2i(Bots.gostek[xname][dir].w, 0);
    glTexCoord2i(1, 0);
    glVertex2i(Bots.gostek[xname][dir].w, Bots.gostek[xname][dir].h);
    glTexCoord2i(0, 0);
    glVertex2i(0, Bots.gostek[xname][dir].h);
    glEnd();

    glPopMatrix();

}



void Bot::Draw() const
{
//std::cout << "FKDFKDFK" << std::endl;
    if (!isKilled)
    {

        // torso (biodro)
        draw_gostek_help(9, PANTS);

        // prawa stopa
        draw_gostek_help(0, NONE);

        // lewa stopa
        draw_gostek_help(3, NONE);

        // prawa noga
        draw_gostek_help(2, PANTS);

        // lewa noga
        draw_gostek_help(5, PANTS);

        // prawe udo
        draw_gostek_help(1, PANTS);

        // lewe udo
        draw_gostek_help(4, PANTS);

        // prawe ramie
        draw_gostek_help(6, SHIRT);

        // prawa reka
        draw_gostek_help(7, SHIRT);

        // prawa dlon
        draw_gostek_help(8, SKIN);

        // (chest) klata
        draw_gostek_help(10, SHIRT);

        // (head) morda
        draw_gostek_help(11, SKIN);

        // lewe ramie
        draw_gostek_help(12, SHIRT);

        // lewa reka
        draw_gostek_help(13, SHIRT);

        // lewa dlon
        draw_gostek_help(14, SKIN);

        //weapon->Draw();

    }

}


void Bot::ChangeMovement(MT move)
{
    movementType = SKOK_W_BOK;
}



void Bot::Update()
{

    TVector2D xPlayerImpulse(0.0f, 0.0f);


    if (moveRight)
    {

        // podskakuje
        if (moveUp && collisionWithPoly)
        {
            xPlayerImpulse.x += 200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            xPlayerImpulse.y -= 1200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = SKOK_W_BOK;
        }
        // biega po powierzchni
        else //if (!moveUp && collisionWithPoly)
        {
            //std::cout << "PO\n";
            xPlayerImpulse.x += 200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = (movementDirection == RIGHT) ? BIEGA : BIEGA_TYL;
        }
        // rusza nogami w powietrzu
        /*else if (!moveUp && !collisionWithPoly)
        {
            std::cout << "RUSA\n";
            xPlayerImpulse.x += 20.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = (movementDirection == RIGHT) ? BIEGA : BIEGA_TYL;
        }
*/
    }

    else if (moveLeft)
    {

        // podskakuje
        if (moveUp && collisionWithPoly)
        {
            xPlayerImpulse.x -= 200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            xPlayerImpulse.y -= 1200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = SKOK_W_BOK;
        }
        // biega po powierzchni
        else //if (collisionWithPoly)
        {
            xPlayerImpulse.x -= 200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = (movementDirection == LEFT) ? BIEGA : BIEGA_TYL;
        }
        // rusza nogami w powietrzu
        /*else
        {
            xPlayerImpulse.x -= 20.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = (movementDirection == LEFT) ? BIEGA : BIEGA_TYL;
        }
*/
    }

    else if (moveUp)
    {
        if (collisionWithPoly)
        {
            xPlayerImpulse.y -= 2200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);
            movementType = SKOK;
            //jump = true;
        }
    }
    else if (moveDown)
    {
        movementType = KUCA;
    }
    else if (wantToFly)
    {

        if (jet->GetAmount() >= 0.01)
        {

            xPlayerImpulse.y += -200.0f * GetMass() * (Physics.fPlayerAirbornTimer * 0.4f + 0.6f);

            //if (SOUNDS_VOL > 0) Mix_PlayChannel(-1, Mix_LoadWAV("Sfx/jump.wav"), 0);
/*            if (procJet >= 2*world.JET_CHANGE)
            {
                procJet -= world.JET_CHANGE;
//            bot[MY_BOT_NR]->movementType = GORA;
            }
*/
        }

    }
    else
    {
        movementType = STOI;
    }

    AddForce(xPlayerImpulse);



    if (!isKilled && (world.getCurrentTime - timerChangeFrame >= 30))
    {
        timerChangeFrame = world.getCurrentTime;
        if (currentFrame >= Bots.FRAMES_MAX[movementType]-1)
        {
            if (movementType != KUCA)
                currentFrame = 0;
            changeMove = true;
        }
        else
        {
            currentFrame++;
            changeMove = false;
        }

    }

    else if (isKilled)
    {
        //Uint32 new_time = world.getCurrentTime - world.bot[i]->timerRespawnTime;
        if (world.getCurrentTime - timerRespawnTime > respawnPeriod)
        {
            isKilled = false;
            procVest = 0.0f;
//            procJet = 1.0f;

            // set start position
            int point = static_cast<int>(rand()%world.spawnpoint[team].size());
            TVector2D pos(static_cast<float>(world.map->spawnpoint[world.spawnpoint[team][point]].x),
                          static_cast<float>(world.map->spawnpoint[world.spawnpoint[team][point]].y));

            GetPosition() = pos;

            if (number == world.MY_BOT_NR)
            {
                world.backg->SetPosition(world.OLD_POS.x - pos.x, world.OLD_POS.y - pos.y);
                if (Parser.SOUNDS_VOL > 0)
                    Audio.sound_new_life.Play();
            }

        }

    }

    if (isReloading)
        ReloadGun();

    // flamegod - 10sec
    if (MODE_FLAMEGOD && (world.getCurrentTime - timeGetSuperbonus >= 10000))
    {
        MODE_FLAMEGOD = false;
    }

    // berserker - 15sec
    else if (MODE_BERSERKER && (world.getCurrentTime - timeGetSuperbonus >= 15000))
    {
        MODE_BERSERKER = false;
    }

    // predator - 25sec
    else if (MODE_PREDATOR && (world.getCurrentTime - timeGetSuperbonus >= 25000))
    {
        MODE_PREDATOR = false;
    }

    weapon[usedWeaponNr]->Update(GetPosition(), world.mouse->getGlobalPosition());
    jet->Update();

    collisionWithPoly = false;

    // speed limit
//    if (velocity.x > maxSpeed.x) velocity.x = maxSpeed.x;
//    else if (velocity.x < -maxSpeed.x) velocity.x = -maxSpeed.x;
//    if (velocity.y > maxSpeed.y) velocity.y = maxSpeed.y;
//    else if (velocity.y < -maxSpeed.y) velocity.y = -maxSpeed.y;

}


void Bot::ChangeWeapon()
{

    // pobierz bron, ktora masz na plecach
	
    Weapon* temp = weapon[0];
    weapon[0] = weapon[1];
    weapon[1] = temp;
	std::cout << "MAM TERAZ " << weapon[0]->m_iModel << std::endl;
    ChangeMovement(ZMIEN_BRON);

}


void Bot::DropWeapon()
{
	ChangeMovement(WYRZUCA);
    delete weapon[0];
    weapon[0] = NULL;

}


void Bot::Shot(const TVector2D& aim)
{

    if (world.getCurrentTime - lastShotTime >= Weapons[gunModel].fireInterval && !isReloading && leftAmmos > 0)
    {

        movementType = CELUJE;

        if (Parser.SOUNDS_VOL > 0)
        {
            // make noise

            TVector2D vec = GetPosition() - world.bot[world.MY_BOT_NR]->GetPosition();
            vec.normalize();

//            std::cout << "KAT " << vec.x << " " << vec.y << std::endl;

            Audio.fireSound[gunModel].SetPosition(-vec.x, -vec.y, 0.f);
            Audio.Play(Audio.fireSound[gunModel]);

        }
        //std::cout << "KAT " << std::endl;
        //Bullet *sbullet = new Bullet(bot[bot_nr]->shotPoint, dest, bot[bot_nr]->gunModel, bot_nr);
        weapon[usedWeaponNr]->Shot(aim);
        leftAmmos--;
//        bullet_list.push_back(sbullet);
        // m_objects.push_back(sbullet);
        lastShotTime = world.getCurrentTime;

        //bot.resize(bot.size()+1);
        //bot[bot.size()-1] = newbot;
    }

    if (leftAmmos == 0) ReloadGun();
}



void Bot::ThrowGrenade(const TVector2D& aim, float push_time)
{
    movementType = RZUCA;

    if (Parser.SOUNDS_VOL > 0)
    {
        Audio.Play(Audio.grenade_pullout);
        Audio.Play(Audio.grenade_throw);
    }

    float divider = (aim.y - m_xPosition.y) / (aim.x - m_xPosition.x);
    float inverted = (aim.x < m_xPosition.x) ? 180.0f : 0.0f;    // if mouse on left add 180 degrees
    float m_fAngle = atan(divider) * RAD_TO_DEG + inverted;

    const float cosa = cos(m_fAngle * DEG_TO_RAD);
    const float sina = sin(m_fAngle * DEG_TO_RAD);

    TVector2D p = TVector2D(cosa, sina);
    TVector2D v = p*push_time;
    //v *= Weapons[m_iModel].speed;
    p += m_xPosition;

    Grenade *grenade = new Grenade(p, v, number);

    Physics.m_movingObj.push_back(grenade);

    numGrenades--;

}



void Bot::ThrowCluster(const TVector2D& aim, float push_time)
{
    movementType = RZUCA;

    if (Parser.SOUNDS_VOL > 0)
    {
        Audio.Play(Audio.grenade_pullout);
        Audio.Play(Audio.grenade_throw);
    }

    float divider = (aim.y - m_xPosition.y) / (aim.x - m_xPosition.x);
    float inverted = (aim.x < m_xPosition.x) ? 180.0f : 0.0f;    // if mouse on left add 180 degrees
    float m_fAngle = atan(divider) * RAD_TO_DEG + inverted;

    const float cosa = cos(m_fAngle * DEG_TO_RAD);
    const float sina = sin(m_fAngle * DEG_TO_RAD);

    TVector2D p = TVector2D(cosa, sina);
    TVector2D v = p*push_time;
    //v *= Weapons[m_iModel].speed;
    p += m_xPosition;

    Cluster *cluster = new Cluster(p, v, number);

    Physics.m_movingObj.push_back(cluster);

    numClusters--;

}

void Bot::ReloadGun()
{

    if (!isReloading)
    {
        if (Parser.SOUNDS_VOL > 0)
            Audio.Play(Audio.reloadSound[gunModel]);
        startReloadingTime = world.getCurrentTime;
        isReloading = true;
        leftAmmos = 0;
        movementType = CHANGE;
    }
    if ((world.getCurrentTime - startReloadingTime) >= Weapons[gunModel].reloadTime)
    {
        leftAmmos = Weapons[gunModel].ammo;
        isReloading = false;
    }
    else if (number == world.MY_BOT_NR)
    {
        if (InterfaceManager::GetSingletonPtr() != NULL)
            Interface.ReloadBar();
    }

}
