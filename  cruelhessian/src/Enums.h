#ifndef ENUMS_H
#define ENUMS_H

/*   Enums.h
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




typedef enum BONUS
{
    BONUS_BERSERKER = 0,
    BONUS_CLUSTERS,
    BONUS_GRENADES,
    BONUS_FLAMER,
    BONUS_MEDIKITS,
    BONUS_PREDATOR,
    BONUS_VESTKIT
} BONUS;


typedef enum GAME_MODE
{
    DM = 0,
    PM,
    TM,
    CTF,
    RM,
    INF,
    HTF
} GAME_MODE;

typedef enum OBJECT_TYPE
{
    GOSTEK = 0,
    BULLET,
    GRENADE,
    PARA,
    FLAG,
    KARABIN,
    KIT,
    STAT
} OBJECT_TYPE;


typedef enum TEAM
{
    TEAM_GENERAL = 0,
    TEAM_ALPHA,
    TEAM_BRAVO,
    TEAM_CHARLIE,
    TEAM_DELTA
} TEAM;

typedef enum OBJECT_SHAPE
{
    XPOINT = 0,
    CIRCLE,
    SQUARE
} OBJECT_SHAPE;


typedef enum MT
{
    BIEGA = 0,
    BIEGA_TYL,
    CELUJE,
    CIESZY,
    GORA,
    KUCA,
    RZUCA,
    SKOK,
    SKOK_DOL_OBROT,
    SKOK_DOL_OBROT_TYL,
    SKOK_W_BOK,
    SPADA,
    STOI,
    ZMIEN_BRON
} MT;

typedef enum MD
{
    LEFT = 0,
    RIGHT
} MD;



typedef enum BODY
{
    STOPA = 0,
    KLATA,
    RAMIE,
    MORDA,
    REKA,
    DLON,
    UDO,
    BIODRO,
    NOGA
} BODY;


typedef enum
{
    SHIRT = 0,
    HAIR,
    SKIN,
    PANTS
} BODY_COLOR;


#endif

