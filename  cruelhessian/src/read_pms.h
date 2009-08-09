/*   read_pms.h
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawe³ Konieczny <konp84@gmail.com>
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

#ifndef _READPMS_
#define _READPMS_

#include <vector>
#include <string>


typedef unsigned char ubyte;
typedef unsigned short word;


class Map
{
    public:

    int read(const std::string& name);

    typedef enum
    {
        ptNORMAL = 0,
        ptONLY_BULLETS_COLLIDE,
        ptONLY_PLAYERS_COLLIDE,
        ptNO_COLLIDE,
        ptICE,
        ptDEADLY,
        ptBLOODY_DEADLY,
        ptHURTS,
        ptREGENERATES,
        ptLAVA
    } PMS_POLYTYPE;

    typedef enum
    {
        dbBEHIND_ALL = 0,
        dbBEHIND_MAP,
        dbBEHIND_NONE
    } PMS_DRAWBEHIND;

    typedef enum SPECIALACTIONS
    {
        saNONE = 0,
        saSTOP_AND_CAMP,
        saWAIT_1_SECOND,
        saWAIT_5_SECONDS,
        saWAIT_10_SECONDS,
        saWAIT_15_SECONDS,
        saWAIT_20_SECONDS
    } PMS_SPECIALACTIONS;

    typedef enum WEATHERTYPE
    {
        wtNONE = 0,
        wtRAIN,
        wtSANDSTORM,
        wtSNOW
    } PMS_WEATHERTYPE;

    typedef enum STEPSTYPE
    {
        stHARD_GROUND = 0,
        stSOFT_GROUND,
        stNONE
    } PMS_STEPSTYPE;

    typedef enum SPAWNTEAM
    {
        stGENERAL = 0,
        stALPHA,
        stBRAVO,
        stCHARLIE,
        stDELTA,
        stALPHA_FLAG,
        stBRAVO_FLAG,
        stGRENADES,
        stMEDKITS,
        stCLUSTERS,
        stVEST,
        stFLAMER,
        stBERSERKER,
        stPREDATOR,
        stYELLOW_FLAG,
        stRAMBO_BOW,
        stSTAT_GUN
    } PMS_SPAWNTEAM;

    typedef struct tagPMS_VECTOR
    {
        float x;
        float y;
        float z;
    } PMS_VECTOR;

    typedef struct tagPMS_COLOR
    {
        ubyte blue;
        ubyte green;
        ubyte red;
        ubyte alpha;
    } PMS_COLOR;

// more stuff

    typedef struct tagPMS_VERTEX
    {
        float x;
        float y;
        float z;
        float rhw;
        PMS_COLOR color;
        float tu;
        float tv;
    } PMS_VERTEX;

    typedef struct tagPMS_POLYGON
    {
        PMS_VERTEX vertex[3];
        PMS_VECTOR perpendicular[3];
        PMS_POLYTYPE polyType;
    } PMS_POLYGON;

    typedef struct tagPMS_SECTOR
    {
        word polyCount;
        std::vector<word> polys;
    } PMS_SECTOR;

    typedef struct tagPMS_PROP
    {
        bool active;
        ubyte filler1;
        word style;
        long width;
        long height;
        float x;
        float y;
        float rotation;
        float scaleX;
        float scaleY;
        ubyte alpha;
        ubyte filler2[3];
        PMS_COLOR color;
        PMS_DRAWBEHIND level;
        ubyte filler3[3];
    } PMS_PROP;

    typedef struct tagDOSTIME
    {
word second :
        5;
word minute :
        6;
word hour :
        5;
    } DOSTIME;

    typedef struct tagDOSDATE
    {
word day :
        5;
word month :
        4;
word year :
        7;
    } DOSDATE;

    typedef struct tagPMS_TIMESTAMP
    {
        DOSTIME time;
        DOSDATE date;
    } PMS_TIMESTAMP;

    typedef struct tagPMS_SCENERY
    {
        std::string name;
        PMS_TIMESTAMP timestamp;
    } PMS_SCENERY;

    typedef struct tagPMS_COLLIDER
    {
        bool active;
        ubyte filler[3];
        float x;
        float y;
        float radius;
    } PMS_COLLIDER;

    typedef struct tagPMS_SPAWNPOINT
    {
        bool active;
        ubyte filler[3];
        long x;
        long y;
        PMS_SPAWNTEAM team;
    } PMS_SPAWNPOINT;

    typedef struct tagPMS_WAYPOINT
    {
        bool active;
        ubyte filler1[3];
        long id;
        long x;
        long y;
        bool left;
        bool right;
        bool up;
        bool down;
        bool jet;
        ubyte path;
        PMS_SPECIALACTIONS specialAction;
        ubyte c2;
        ubyte c3;
        ubyte filler2[3];
        long numConnections;
        long connections[20];
    } PMS_WAYPOINT;

// Pms structure

    //struct pms
//    {
        long version;
        std::string name;
        std::string texture;
        PMS_COLOR bgColorTop;
        PMS_COLOR bgColorBottom;
        long jetAmount;
        ubyte grenades;
        ubyte medikits;
        PMS_WEATHERTYPE weather;
        PMS_STEPSTYPE steps;
        long randID;
        long polygonCount;
        std::vector<PMS_POLYGON> polygon;
        long sectorDivisions;
        long numSectors;
        std::vector<PMS_SECTOR> sector;
        long propCount;
        std::vector<PMS_PROP> prop;
        long sceneryCount;
        std::vector<PMS_SCENERY> scenery;
        long colliderCount;
        std::vector<PMS_COLLIDER> collider;
        long spawnpointCount;
        std::vector<PMS_SPAWNPOINT> spawnpoint;
        long waypointCount;
        std::vector<PMS_WAYPOINT> waypoint;
        float leftoffs;
        float rightoffs;
        float topoffs;
        float bottomoffs;
    //};
};

extern Map p;

#endif
