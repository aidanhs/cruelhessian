/*   controls.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84@gmail.com>
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

#include "SDL_mixer.h"
#include "controls.h"
#include "draw_extra.h"
#include "gun.h"
#include "bot.h"
#include "worldmap.h"



static Uint8* keys;// = NULL;


static void Input_User()
{

    SDL_Event event;

    while ( SDL_PollEvent(&event) )
    {
        if ( event.type == SDL_QUIT )
        {
            world->CHOICE_EXIT = 1;
            //done = 1;
        }
        CURRENT_MOUSE_POS = GetOGLPos();
        if ( event.type == SDL_KEYDOWN )
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                world->CHOICE_ESC = 1-world->CHOICE_ESC;
            }
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                if ((world->CHOICE_GUN == 0) && (bot[world->MY_BOT_NR]->numGrenades >= 1))
                {
                    bot[world->MY_BOT_NR]->movementType = RZUCA;
                    if (SOUNDS_VOL > 0)
                        Mix_PlayChannel(-1, Mix_LoadWAV((SOL_PATH+"Sfx/grenade-throw.wav").c_str()), 0);

                    world->add_grenade(world->MY_BOT_NR, CURRENT_MOUSE_POS.x, CURRENT_MOUSE_POS.y, 20.0);
                }

            }
            if (event.key.keysym.sym == SDLK_s)
            {
                bot[world->MY_BOT_NR]->movementType = KUCA;
//           read_poa(bot[MY_BOT_NR].movementType, bot[MY_BOT_NR].movementDirection);
            }
        }

        //else read_poa("Anims/stoi.poa", MOVEMENT_DIRECTION);
        //event.button.button



//        if (move_jump == 0) read_poa("Anims/stoi.poa", MOVEMENT_DIRECTION);
        if ((float)CURRENT_MOUSE_POS.x <= bot[world->MY_BOT_NR]->position.x)
        {
            bot[world->MY_BOT_NR]->movementDirection = LEFT;
            bot[world->MY_BOT_NR]->movementType = STOI;
//       read_poa(bot[MY_BOT_NR].movementType, bot[MY_BOT_NR].movementDirection);
        }
        else if ((float)CURRENT_MOUSE_POS.x > bot[world->MY_BOT_NR]->position.x)
        {
            bot[world->MY_BOT_NR]->movementDirection = RIGHT;
            bot[world->MY_BOT_NR]->movementType = STOI;
//       read_poa(bot[MY_BOT_NR].movementType, bot[MY_BOT_NR].movementDirection);
        }
        // pojedyncze nacisniecie
        // tylko USSCOM, Dessert Eagles, M79
        //if (bot[MY_BOT_NR]->gunModel == 0 || bot[MY_BOT_NR]->gunModel == 1)
        if ( event.type == SDL_MOUSEBUTTONDOWN )
        {
            if ( event.button.button == SDL_BUTTON_LEFT )
            {
                //if (!click_g)
                /*if (bot[MY_BOT_NR]->isReloading == 0)
                {
                    // dla kazdego wystrzelonego naboju stworz strukture

                    shot(MY_BOT_NR, CURRENT_MOUSE_POS.x, CURRENT_MOUSE_POS.y);
                    //SDL_Delay(gun_interval_time(GUN_MODEL));

                    bot[MY_BOT_NR]->leftAmmos--;

                }*/
                if (SOUNDS_VOL > 0)
                    Mix_PlayChannel(-1, Mix_LoadWAV(gun_fire(bot[world->MY_BOT_NR]->gunModel)), 0);
                world->add_bullet(world->MY_BOT_NR, (float)CURRENT_MOUSE_POS.x, (float)CURRENT_MOUSE_POS.y);

                //sun.yadd(-1);
            }
            /*else
            {
                //gun_shot = 0;
                bot[MY_BOT_NR]->isShooting = 0;
            }*/
        }
    }

    //if (GUN_MODEL == 2 || GUN_MODEL == 3)
    // if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON_LMASK) printf("HHhh\n");
    //    if (!click_g) gunn = SDL_CreateThread(gun_shot, NULL);

    // prawy przycisk myszy

    if ((SDL_GetMouseState(NULL, NULL)&SDL_BUTTON_RMASK) && (bot[world->MY_BOT_NR]->procJet >= 0.004))
    {
        //if (!collision_det(0,-1, &bot[0]))
        {
            bot[world->MY_BOT_NR]->isFlying = 1;
            bot[world->MY_BOT_NR]->movementType = GORA;
            //bot[0].velocity.y += 5;
            //read_poa("Anims/gora.poa", MOVEMENT_DIRECTION);
        }
        if (SOUNDS_VOL > 0) Mix_PlayChannel(-1, Mix_LoadWAV("Sfx/jump.wav"), 0);
        if (bot[world->MY_BOT_NR]->procJet >= 0.005)
            bot[world->MY_BOT_NR]->procJet -= 0.004;
    }
    else if (bot[world->MY_BOT_NR]->procJet >= 0.000)
    {
        if (bot[world->MY_BOT_NR]->procJet <= 0.998)
        {
            bot[world->MY_BOT_NR]->procJet += 0.002;
        }
        bot[world->MY_BOT_NR]->isFlying = 0;

        /* if (!collision_det(0,1, &bot[0]))
         {
             bot[0].y += 1.5;                             // caly czas lec w dol
             read_poa("Anims/spada.poa", MOVEMENT_DIRECTION);
         }*/
    }



    keys = SDL_GetKeyState(NULL);
    //int LeftRightMove = 0;

    /*if ( keys[SDLK_g] == 1)
    {
        CHOICE_GUN = 1-CHOICE_GUN;
    }*/

    /* if ( keys[SDLK_SPACE] == 1)
         if ((CHOICE_GUN == 0) && (bot[MY_BOT_NR]->numGrenades >= 1))
         {
    //                gren = SDL_CreateThread(throw_grenade, NULL);
         }
    */

    if ( keys[SDLK_s] == 1)
    {
        //read_poa("Anims/kuca.poa", MOVEMENT_DIRECTION);
        //glTranslatef(0, -2.0f, 0);
        //ddx = 0;
        //ddy -= 1;
        bot[world->MY_BOT_NR]->movementType = KUCA;
        //if (!collision_det(0,1, ddx, ddy, SOL_WIDTH, SOL_HEIGHT)) ddy -= 1;
        //if (!collision_det(0,1, sun.mX, sun.mY, SOL_WIDTH, SOL_HEIGHT)) sun.add(0,1);
    }



    if ( keys[SDLK_a] == 1)// && !collision_det(-1, 0, &bot[0]))
        //    if (event.key.keysym.sym == SDLK_a)
    {
        bot[world->MY_BOT_NR]->move_left();
        //bot[MY_BOT_NR]->movementType = BIEGA;
        //bot[MY_BOT_NR].h = SOL_HEIGHT[bot[MY_BOT_NR].movementType];
        //bot[MY_BOT_NR].w = SOL_WIDTH[bot[MY_BOT_NR].movementType];
        //read_poa("Anims/biega.poa", MOVEMENT_DIRECTION);


        // sprawdz wysokosc przeszkody

        /* for (i = 0; i <= SOL_HEIGHT/3; i++)
             if (!collision_det(-1, -i, &bot[0]))
             {
                 //bot[0].position.x -= 1;
                 //bot[0].y -= i;
                 break;
             }
        */
        //if (!collision_det(-1, 1, &bot[0]))
        //LeftRightMove--;
        //bot[MY_BOT_NR]->velocity.x += sWalking;
        // sprawdz nachylenie przeszkody
        // fixme !!!!!!!!!

        // przesun ekran
        //glTranslatef(Walking_Coelff, 0, 0);
    }

    if (keys[SDLK_d] == 1)// && !collision_det(1, 0, &bot[0]))
    {
        bot[world->MY_BOT_NR]->move_right();
        //bot[MY_BOT_NR]->movementType = BIEGA;
        //bot[MY_BOT_NR].h = SOL_HEIGHT[bot[MY_BOT_NR].movementType];
        //bot[MY_BOT_NR].w = SOL_WIDTH[bot[MY_BOT_NR].movementType];
        //read_poa("Anims/biega.poa", MOVEMENT_DIRECTION);

        // sprawdz wysokosc ewentualnej przeszkody

        /* for (i = 0; i <= SOL_HEIGHT/3; i++)
             if (!collision_det(1, -i, &bot[0]))
             {
                 //bot[0].position.x += 1;
                 bot[0].position.y -= i;
                 break;
             }*/
        //if (!collision_det(1, -1, &bot[0]))
        //LeftRightMove++;
        //bot[MY_BOT_NR]->velocity.x -= sWalking;

        //if (bot[0].position.x < bgX+MAX_WIDTH)
        //glTranslatef(-Walking_Coelff, 0, 0);
    }

    //bot[0].vVelocity.x += LeftRightMove * Walking_Coelff;

    // skok mozna wykonac tylko wtedy gdy soldier stoi na powierzchni

    if (keys[SDLK_w] && bot[0]->collision_det_with_wall(0, 1))
    {
        //read_poa("Anims/kuca.poa", MOVEMENT_DIRECTION);
        //glTranslatef(0, -2.0f, 0);
        //ddx = 0;
        //ddy -= 1;
        bot[world->MY_BOT_NR]->movementType = SKOK;
        bot[world->MY_BOT_NR]->velocity.y += 3*world->sJumping;
        //if (!collision_det(0,1, ddx, ddy, SOL_WIDTH, SOL_HEIGHT)) ddy -= 1;
        //if (!collision_det(0,1, sun.mX, sun.mY, SOL_WIDTH, SOL_HEIGHT)) sun.add(0,1);
    }

    if (keys[SDLK_a] && keys[SDLK_w] && bot[0]->collision_det_with_wall(0, 1))
    {
        //bot[MY_BOT_NR].movementDirection = 0;
        //read_poa("Anims/skok.poa", MOVEMENT_DIRECTION);
//printf("HOP\n");
        //move_jump = -1;
        bot[world->MY_BOT_NR]->velocity.y += 2*world->sJumping;

        // Skok zgodny z kierunkiem ruchu (sila o polowe mniejsza niz wybicie w gore)
        bot[world->MY_BOT_NR]->velocity.x += world->sJumping * 0.5;
        //starty = bot[0].y;
    }


    if (keys[SDLK_d] == 1 && keys[SDLK_w] == 1 && bot[0]->collision_det_with_wall(0, 1))
    {
        //bot[MY_BOT_NR].movementDirection = 1;
        //read_poa("Anims/skok.poa", MOVEMENT_DIRECTION);

        //move_jump = 1;
        bot[world->MY_BOT_NR]->velocity.y += 2*world->sJumping;

        // Skok zgodny z kierunkiem ruchu (sila o polowe mniejsza niz wybicie w gore)
        bot[world->MY_BOT_NR]->velocity.x -= world->sJumping * 0.5;
        //starty = bot[0].y;
    }
    /*
        if (move_jump == 1 || move_jump == -1)
        {
            //if (timer_jump - SDL_GetTicks() >= 1500)
            {
                //timer_jump = SDL_GetTicks();

                if (JUMP_HEIGHT <= 8 && !collision_det(move_jump, -1, &bot[0]))
                {
                    bot[0].x += 0.6*move_jump;
                    //bot[0].y = starty+JUMP_STEPS;
                    bot[0].y -= 1.8;
                    JUMP_HEIGHT += 0.3;
                }
                else
                {
                    JUMP_HEIGHT = 0;
                    move_jump = 0;                        // koniec skoku
                }
            }
        }
    */
    //if (event.key.keysym.sym == SDLK_r)
    if ( keys[SDLK_r] == 1)
    {
        bot[world->MY_BOT_NR]->isReloading = 1;
        SDL_CreateThread(reload_gun, NULL);
    }

    //ograniczenie predkosci
    if (bot[world->MY_BOT_NR]->velocity.x > bot[world->MY_BOT_NR]->maxV) bot[world->MY_BOT_NR]->velocity.x = bot[world->MY_BOT_NR]->maxV;
    if (bot[world->MY_BOT_NR]->velocity.x < -bot[world->MY_BOT_NR]->maxV) bot[world->MY_BOT_NR]->velocity.x = -bot[world->MY_BOT_NR]->maxV;



}




void WorldMap::keys_control()
{
    Uint32 LastTime = ElapsedTime;
    float dt;
    int stepcount; //Ilosc krokow czasowych do wykonania
    //float steprest; //Reszta z dzielenia
    float dtDiv; //Podzielony czas
    int fStep;

    ElapsedTime = SDL_GetTicks();// - DemoStart; // Calculate Elapsed Time
    this->dDeltaTime = (this->dDeltaTime + (float)(ElapsedTime - LastTime) / 1000) / 2;

    //Time_Physics(dDeltaTime * sBulletTime);
    dt = this->dDeltaTime * sBulletTime;


    //Dt musi byc wartoscia z przedzialu <TimeStepSize; 0.1>
    if (dt < TimeStepSize)
    {
        dt = TimeStepSize;
        //dtDiv = dt / TimeStepSize;
    }
    else if (dt > 0.1)
    {
        dt = 0.1;
        //dtDiv = 0.1 / TimeStepSize;
    } //else printf("OJC\n\n");

    dtDiv = dt / world->TimeStepSize;
    stepcount = (int)dtDiv; //calkowita czesc z dzielenia

    for (fStep = 0; fStep < stepcount; fStep++)
    {
        calculate_physics(world->TimeStepSize);
        Input_User();
    }

    //Wyrownanie kroku
    calculate_physics((dtDiv-stepcount)* TimeStepSize);
    Input_User();


    ElapsedTime = (LastTime + ElapsedTime) / 2; // Average it out for smoother movement

    Input_User();
}



static inline bool ccw(TVector2D A, PMS_VERTEX B, PMS_VERTEX C)
{
    if ((C.y-A.y)*(B.x-A.x) > (B.y-A.y)*(C.x-A.x)) return true;
    else return false;
}

static inline bool ccw(TVector2D A, TVector2D B, PMS_VERTEX C)
{
    if ((C.y-A.y)*(B.x-A.x) > (B.y-A.y)*(C.x-A.x)) return true;
    else return false;
}


// sprawdzanie, czy boty sie widza
// first - numer pierwszego bota
// second numer drugiego
bool
WorldMap::do_bots_see(int first, int second)
{

    // A - bot[first].position.x, bot[first].position.y
    // B - bot[second].position.x, bot[second].position.y
    // C - xpoly[i].vertex[k].x, xpoly[i].vertex[k].y
    // D - xpoly[i].vertex[k+1].x, xpoly[i].vertex[k+1].y

    for (int i = 0; i < (int)p.polygonCount; i++) // sprawdz kazdy trojkat, czy koliduje z linia
    {
        if (p.polygon[i].polyType != ptONLY_BULLETS_COLLIDE && p.polygon[i].polyType != ptNO_COLLIDE)
        {
            // 0, 1
            if ((ccw(bot[first]->position, p.polygon[i].vertex[0], p.polygon[i].vertex[1]) !=
                    ccw(bot[second]->position, p.polygon[i].vertex[0], p.polygon[i].vertex[1])) &&
                    (ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[0]) !=
                     ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[1])))
                return false;

            // 1, 2
            if ((ccw(bot[first]->position, p.polygon[i].vertex[1], p.polygon[i].vertex[2]) !=
                    ccw(bot[second]->position, p.polygon[i].vertex[1], p.polygon[i].vertex[2])) &&
                    (ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[1]) !=
                     ccw(bot[first]->position, bot[second]->position, p.polygon[i].vertex[2])))
                return false;

        }
    }
    return true;

}









void WorldMap::bots_control()
{
    int dest, j;
    vector<int> ostrz_list;

    // shooting (strzelajacy)
    for (int i = 1; i < world->BOTS_NUM; i++)
    {
        // optimize it !!!!!!!!!!!!!!!!!!!!!!!!1
        for (j = 0; j < world->BOTS_NUM; j++)
        {
            // stworz liste mozliwych ostrzeliwanych
            if (!bot[i]->isKilled && !bot[j]->isKilled && i != j && do_bots_see(i, j))
            {
                ostrz_list.push_back(j);
            }
        }
        if (ostrz_list.size() > 0)
        {
            // j - wylosuj ostrzeliwanego
            j = ostrz_list[rand() % ostrz_list.size()];
            if (bot[j]->position.x > bot[i]->position.x)
            {
                if (bot[i]->movementDirection == LEFT)
                    bot[i]->movementDirection = RIGHT;
            }
            else
            {
                if (bot[i]->movementDirection == RIGHT)
                    bot[i]->movementDirection = LEFT;
            }
            world->add_bullet(i, bot[j]->position.x, bot[j]->position.y);
            ostrz_list.clear();
        }
    }

    for (int i = 1; i < world->BOTS_NUM; i++)
    {
        // waypoints
//cout <<"test";
        dest = bot[i]->destinationPoint;
        //cout << dest;

        if (p.waypoint[dest].right) bot[i]->move_right();
        if (p.waypoint[dest].left) bot[i]->move_left();
        if (p.waypoint[dest].up) bot[i]->move_up();
        if (p.waypoint[dest].down) bot[i]->move_down();
        if (p.waypoint[dest].jet) bot[i]->move_jet();
        /*
                switch (p.waypoint[dest].specialAction)
                {
                    case saNONE:cout<<"zero\n";break;
                    case saSTOP_AND_CAMP:cout<<"st\n";break;
                    case saWAIT_1_SECOND:cout<<"111111\n";break;
                    case saWAIT_5_SECONDS:cout<<"55555\n";break;
                    case saWAIT_10_SECONDS:cout<<"hhhhhhhh\n";break;
                    case saWAIT_15_SECONDS:cout<<"nnnnnnnnnn\n";break;
                    case saWAIT_20_SECONDS:cout<<"vvvvvvv\n";break;
                }
        */

        //if (doszedl(i))
        if (bot[i]->is_inside(p.waypoint[bot[i]->destinationPoint].x, p.waypoint[bot[i]->destinationPoint].y))
        {
            //cout << "Doszedl";
            if (p.waypoint[dest].numConnections == 0)
                bot[i]->destinationPoint = p.waypoint[dest].connections[0]-1;
            else
                //{
                //int j = (int)(rand() % p.waypoint[dest].numConnections);
                bot[i]->destinationPoint = p.waypoint[dest].connections[(int)(rand() % p.waypoint[dest].numConnections)]-1;
            //}
        }
        /*
                glColor4ub(180,190,190,100);
                glBegin(GL_LINES);
                 glVertex2f((float)bot[i]->position.x, (float)bot[i]->position.y);
                 glVertex2f((float)p.waypoint[dest].x, (float)p.waypoint[dest].y);
                 //glVertex2f((float)p.waypoint[p.waypoint[dest].connections[j]-1].x, (float)p.waypoint[p.waypoint[dest].connections[j]-1].y);
                 glEnd();
        */
        //cout << " dest X Y "<< p.waypoint[dest].x << " " << p.waypoint[dest].y << endl;
        //cout << " Xx xY "<< bot[i]->position.x << " " << bot[i]->position.y << endl;
    }

}

