#ifndef PMSMAP_H
#define PMSMAP_H

/*   Map.h
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



#include <vector>
#include <string>

#include "TexturesLoader.h"
#include "Drawable.h"
#include "Body.h"
#include "MapElement.h"
#include "MapElementPolygon.h"
#include "MapElementProp.h"
#ifdef _WIN32
#include "CompatibleWindows.h"
#else
#include <GL/gl.h>
#endif


//typedef unsigned char ubyte;
//typedef unsigned short word;


//class Body;

class Map : public Body
{
//protected:


public:
    GLuint* text_scen;
    GLuint text_poly;
    Tex text_weath;

    Map(const std::string& mname);

    ~Map();


//private:
    typedef enum
    {
        saNONE = 0,
        saSTOP_AND_CAMP,
        saWAIT_1_SECOND,
        saWAIT_5_SECONDS,
        saWAIT_10_SECONDS,
        saWAIT_15_SECONDS,
        saWAIT_20_SECONDS
    } PMS_SPECIALACTIONS;

    typedef enum
    {
        wtNONE = 0,
        wtRAIN,
        wtSANDSTORM,
        wtSNOW
    } PMS_WEATHERTYPE;

    typedef enum
    {
        stHARD_GROUND = 0,
        stSOFT_GROUND,
        stNONE
    } PMS_STEPSTYPE;

    typedef enum
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





    typedef struct
    {
        word polyCount;
        std::vector<word> polys;
    } PMS_SECTOR;



    typedef struct
    {
word second :
        5;
word minute :
        6;
word hour :
        5;
    } DOSTIME;

    typedef struct
    {
word day :
        5;
word month :
        4;
word year :
        7;
    } DOSDATE;

    typedef struct
    {
        DOSTIME time;
        DOSDATE date;
    } PMS_TIMESTAMP;

    typedef struct
    {
        std::string name;
        PMS_TIMESTAMP timestamp;
    } PMS_SCENERY;



    typedef struct
    {
        bool active;
        ubyte filler[3];
        long x;
        long y;
        PMS_SPAWNTEAM team;
    } PMS_SPAWNPOINT;

    typedef struct
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


public:
    std::vector<Body *>bod;
    long version;
    std::string name;
    std::string texture;

    long jetAmount;
    ubyte grenades;
    ubyte medikits;
    PMS_WEATHERTYPE weather;
    PMS_STEPSTYPE steps;
    long randID;
    long polygonCount;
    MapElementBackground background;
    std::vector<MapElementPolygon> polygon;
    long sectorDivisions;
    long numSectors;
    std::vector<PMS_SECTOR> sector;
    long propCount;
    std::vector<MapElementProp> prop;
    long sceneryCount;
    std::vector<PMS_SCENERY> scenery;
    long colliderCount;
//    std::vector<MapElementCollider> collider;
    long spawnpointCount;
    std::vector<PMS_SPAWNPOINT> spawnpoint;
    long waypointCount;
    std::vector<PMS_WAYPOINT> waypoint;
    float leftoffs;
    float rightoffs;
    float topoffs;
    float bottomoffs;

};


class MapBack : public Drawable
{
    const Map& map;
public:
    MapBack(const Map& _map) : map(_map) {};
	MapBack& operator=(const MapBack&) {}

    void Draw() const;
};


class MapFront : public Drawable
{
    const Map& map;
public:
    MapFront(const Map& _map) : map(_map) {};
	MapFront& operator=(const MapFront&) {}

    void Draw() const;
};


#endif

