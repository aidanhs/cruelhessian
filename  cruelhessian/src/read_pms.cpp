/*   read_pms.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008 by Pawel Konieczny <konp84 at gmail.com>
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
 * aint with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


// based on mar77a code



#include <fstream>
#include <vector>
#include <string>

#include "read_pms.h"
#include "worldmap.h"
#include "parser/SimpleIni.h"


Map p;


template<class T> T read_bin(std::istream& is)
{
    T tmp;
    //float tmp;
    //if (T == int) return tmp;
    is.read(reinterpret_cast<char*>(&tmp), sizeof(T));

    //is.read(reinterpret_cast<char*>(&tmp), siz);
    return tmp;
}

static std::string read_string(std::istream& is, int full_length)
{
    int length = read_bin<char>(is);
    std::vector<char> tmp(full_length);
    is.read( &tmp[0], tmp.size() );
    return std::string(tmp.begin(), tmp.begin() + length);
}
/*
int read_messages(const char *name)
{
    FILE * pFile;
   char mystring[100];

   pFile = fopen (name , "r");

   //while (fgets (mystring , 100 , pFile)) {printf("%s\n",mystring);}
//   while (pFile.getline(MESSAGE[i++], '\r')){}
for (int i=0;i<700;i++)
{
pFile.get(mystring, sizeof(mystring));
cout << mystring;
}
    return 1;
}
*/
/*
int read_messages(const char *name)
{
    wifstream is;
    is.open(name);
    wcout.imbue( locale("") );
    //is.imbue( locale( "pl_PL.UTF-8" ) ) ;
    int i = 0;
    wstring test;
    //char bufor[256];

 //std::wstring resultstring;
    if (is == NULL)
    {
        cout << "Blad odczytu pliku\n";
        return 0;
    }
    //is.get();
    //is.get();
    while (getline(is, test)) {wcout << test;i++;}
    //while (getline(is, MESSAGE[i++], '\r'))
    {
        //MESSAGE.push_back(test);
        //is.get();
    }
cout << i;
    is.close();

    //for (int k = 0; k < i; k++) cout << MESSAGE[k] << "";
    //cout << i;

    return 1;
}
*/


/** @param  fsdfsd
    @return opis
*/


int Map::read(const std::string& mname)
{

    std::ifstream is(mname.c_str(), std::ios::binary);
//cout << "test " << sizeof(int) << " " << sizeof(int) << endl;
    p.version = read_bin<int>(is);
    p.name = read_string(is, 38);
    p.texture = read_string(is, 24);
    p.bgColorTop = read_bin<PMS_COLOR>(is);
    p.bgColorBottom = read_bin<PMS_COLOR>(is);
    p.jetAmount = read_bin<int>(is);
    p.grenades = read_bin<ubyte>(is);
    p.medikits = read_bin<ubyte>(is);
    p.weather = PMS_WEATHERTYPE(read_bin<ubyte>(is));
    p.steps = PMS_STEPSTYPE(read_bin<ubyte>(is));
    p.randID = read_bin<int>(is);
    p.polygonCount = read_bin<int>(is);


    /*
        p.version = read_bin<4>(is);
        p.name = read_string(is, 38);
        p.texture = read_string(is, 24);
        p.bgColorTop = read_bin<PMS_COLOR>(is);
        p.bgColorBottom = read_bin<PMS_COLOR>(is);
        p.jetAmount = read_bin<8>(is);
        p.grenades = read_bin<1>(is);
        p.medikits = read_bin<1>(is);
        p.weather = PMS_WEATHERTYPE(read_bin<1>(is));
        p.steps = PMS_STEPSTYPE(read_bin<1>(is));
        p.randID = read_bin<8>(is);
        p.polygonCount = read_bin<8>(is);
    */
//std::cout << " P " << p.polygonCount << "\n";

    for (int i = 0; i < p.polygonCount; i++)
    {
        PMS_POLYGON temp;
        temp.vertex[0] = read_bin<PMS_VERTEX>(is);
        temp.vertex[1] = read_bin<PMS_VERTEX>(is);
        temp.vertex[2] = read_bin<PMS_VERTEX>(is);
        temp.perpendicular[0] = read_bin<PMS_VECTOR>(is);
        temp.perpendicular[1] = read_bin<PMS_VECTOR>(is);
        temp.perpendicular[2] = read_bin<PMS_VECTOR>(is);
        temp.polyType = PMS_POLYTYPE(read_bin<ubyte>(is));
        p.polygon.push_back(temp);

       // aa[i][0] = p.polygon[i].perpendicular[0].y / p.polygon[i].perpendicular[0].x;
       // aa[i][1] = p.polygon[i].perpendicular[1].y / p.polygon[i].perpendicular[1].x;
       // aa[i][2] = p.polygon[i].perpendicular[2].y / p.polygon[i].perpendicular[2].x;

/*        if (temp.polyType != ptNO_COLLIDE && temp.polyType != ptONLY_BULLETS_COLLIDE)
        {
            cpVect tr_temp[] =
            {
                // clockwise direction
                cpv(temp.vertex[2].x, temp.vertex[2].y),
                cpv(temp.vertex[1].x, temp.vertex[1].y),
                cpv(temp.vertex[0].x, temp.vertex[0].y),
            };
            cpSpaceAddStaticShape(spaceMap, cpPolyShapeNew(staticBodyMap, 3, tr_temp, cpvzero));
        }*/
    }
    p.sectorDivisions = read_bin<int>(is);
    p.numSectors = read_bin<int>(is);

    p.topoffs = (float)(p.sectorDivisions * -p.numSectors);
    p.bottomoffs = (float)(p.sectorDivisions * p.numSectors);
    p.leftoffs = (float)(p.sectorDivisions * -p.numSectors);
    p.rightoffs = (float)(p.sectorDivisions * p.numSectors);

    for (int i = 0; i < ((p.numSectors*2)+1)*((p.numSectors*2)+1); i++)
    {
        PMS_SECTOR temp;
        temp.polyCount = read_bin<word>(is);
        for (int m = 0; m < temp.polyCount; m++)
        {
            temp.polys.push_back(read_bin<word>(is));
        }
        p.sector.push_back(temp);
    }
    p.propCount = read_bin<int>(is);
    for (int i = 0; i < p.propCount; i++)
    {
        PMS_PROP temp;
        //temp.active = bool(read_bin<ubyte>(is));
        temp.active = bool(read_bin<bool>(is));
        temp.filler1 = read_bin<ubyte>(is);
        temp.style = read_bin<word>(is);
        temp.width = read_bin<int>(is);
        temp.height = read_bin<int>(is);
        temp.x = read_bin<float>(is);
        temp.y = read_bin<float>(is);
        temp.rotation = read_bin<float>(is);
        temp.scaleX = read_bin<float>(is);
        temp.scaleY = read_bin<float>(is);
        temp.alpha = read_bin<ubyte>(is);
        temp.filler2[0] = read_bin<ubyte>(is);
        temp.filler2[1] = read_bin<ubyte>(is);
        temp.filler2[2] = read_bin<ubyte>(is);
        temp.color = read_bin<PMS_COLOR>(is);
        temp.level = PMS_DRAWBEHIND(read_bin<ubyte>(is));
        temp.filler3[0] = read_bin<ubyte>(is);
        temp.filler3[1] = read_bin<ubyte>(is);
        temp.filler3[2] = read_bin<ubyte>(is);
        p.prop.push_back(temp);
    }
    p.sceneryCount = read_bin<int>(is);
    for (int i = 0; i < p.sceneryCount; i++)
    {
        PMS_SCENERY temp;
        //temp.nameLen = read_bin<ubyte>( is );
        temp.name = read_string(is, 50);
        temp.timestamp = read_bin<PMS_TIMESTAMP>(is);
        p.scenery.push_back(temp);
    }
    p.colliderCount = read_bin<int>(is);
    for (int i = 0; i < p.colliderCount; i++)
    {
        PMS_COLLIDER temp;
        //temp.active = bool(read_bin<ubyte>(is));
        temp.active = bool(read_bin<bool>(is));
        temp.filler[0] = read_bin<ubyte>(is);
        temp.filler[1] = read_bin<ubyte>(is);
        temp.filler[2] = read_bin<ubyte>(is);
        temp.x = read_bin<float>(is);
        temp.y = read_bin<float>(is);
        temp.radius = read_bin<float>(is);
        p.collider.push_back(temp);
    }
    p.spawnpointCount = read_bin<int>(is);
    //std::cout << "SPAWMsd " << p.spawnpointCount << std::endl;
    for (int i = 0; i < p.spawnpointCount; i++)
    {
        PMS_SPAWNPOINT temp;
        //temp.active = bool(read_bin<ubyte>(is));
        temp.active = bool(read_bin<bool>(is));
        temp.filler[0] = read_bin<ubyte>(is);
        temp.filler[1] = read_bin<ubyte>(is);
        temp.filler[2] = read_bin<ubyte>(is);
        temp.x = read_bin<int>(is);
        temp.y = read_bin<int>(is);
        temp.team = PMS_SPAWNTEAM(read_bin<unsigned int>(is));
        p.spawnpoint.push_back(temp);
    }
    p.waypointCount = read_bin<int>(is);
    for (int i = 0; i < p.waypointCount; i++)
    {
        PMS_WAYPOINT temp;
        //temp.active = bool(read_bin<ubyte>(is));
        temp.active = bool(read_bin<bool>(is));
        temp.filler1[0] = read_bin<ubyte>(is);
        temp.filler1[1] = read_bin<ubyte>(is);
        temp.filler1[2] = read_bin<ubyte>(is);
        temp.id = read_bin<int>(is);
        temp.x = read_bin<int>(is);
        temp.y = read_bin<int>(is);
        //temp.left = bool(read_bin<ubyte>(is));
        temp.left = bool(read_bin<bool>(is));
        //temp.right = bool(read_bin<ubyte>(is));
        temp.right = bool(read_bin<bool>(is));
        //temp.up = bool(read_bin<ubyte>(is));
        temp.up = bool(read_bin<bool>(is));
        //temp.down = bool(read_bin<ubyte>(is));
        temp.down = bool(read_bin<bool>(is));
        //temp.jet = bool(read_bin<ubyte>(is));
        temp.jet = bool(read_bin<bool>(is));
        temp.path = read_bin<ubyte>(is);
        temp.specialAction = PMS_SPECIALACTIONS(read_bin<ubyte>(is));
        temp.c2 = read_bin<ubyte>(is);
        temp.c3 = read_bin<ubyte>(is);
        temp.filler2[0] = read_bin<ubyte>(is);
        temp.filler2[1] = read_bin<ubyte>(is);
        temp.filler2[2] = read_bin<ubyte>(is);
        temp.numConnections = read_bin<int>(is);
        for (int c = 0; c < 20; c++)
            temp.connections[c] = read_bin<int>(is);
        p.waypoint.push_back(temp);
    }
    is.close();

    return 0;
}

