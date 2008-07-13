/*   read_pms.cpp
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


// based on mar77a code



#include <fstream>
#include <vector>
#include <string>
#include "globals.h"
#include "read_pms.h"
#include "worldmap.h"

char Font1[100];
char Font2[100];
int FontMenuSize;
int FontConsoleSize;
int FontBigSize;
int FontWeaponMenuSize;
int FontConsoleSmallSize;
int FontHeightScale;
int FontMenuBold;
int FontConsoleBold;
int FontBigBold;
int FontWeaponMenuBold;
int FontConsoleSmallBold;
int KillConsoleNameSpace;
pms p;

template<class T> T read_bin(istream& is)
{
    T tmp;
    is.read(reinterpret_cast<char*>(&tmp), sizeof(T));
    return tmp;
}

string read_string(istream& is, int full_length)
{
    int length = read_bin<char>(is);
    vector<char> tmp(full_length);
    is.read( &tmp[0], tmp.size() );
    return string(tmp.begin(), tmp.begin() + length);
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

//int WorldMap::read_map(const char *name)
/** @param  fsdfsd
    @return opis
*/
int WorldMap::read_map(std::string name)
{

    ifstream is(name.c_str(), ios::binary);

    p.version = read_bin<int>(is);
    p.name = read_string(is, 38);
    p.texture = read_string(is, 24);
    p.bgColorTop = read_bin<PMS_COLOR>(is);
    p.bgColorBottom = read_bin<PMS_COLOR>(is);
    p.jetAmount = read_bin<long>(is);
    p.grenades = read_bin<ubyte>(is);
    p.medikits = read_bin<ubyte>(is);
    p.weather = PMS_WEATHERTYPE(read_bin<ubyte>(is));
    p.steps = PMS_STEPSTYPE(read_bin<ubyte>(is));
    p.randID = read_bin<long>(is);
    p.polygonCount = read_bin<long>(is);

    for (int i = 0; i < p.polygonCount; i++)
    {
        PMS_POLYGON temp; // cheap solution
        temp.vertex[0] = read_bin<PMS_VERTEX>(is);
        temp.vertex[1] = read_bin<PMS_VERTEX>(is);
        temp.vertex[2] = read_bin<PMS_VERTEX>(is);
        temp.perpendicular[0] = read_bin<PMS_VECTOR>(is);
        temp.perpendicular[1] = read_bin<PMS_VECTOR>(is);
        temp.perpendicular[2] = read_bin<PMS_VECTOR>(is);
        temp.polyType = PMS_POLYTYPE(read_bin<ubyte>(is));
        p.polygon.push_back(temp);
    }
    p.sectorDivisions = read_bin<long>(is);
    p.numSectors = read_bin<long>(is);

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
    p.propCount = read_bin<long>(is);
    for (int i = 0; i < p.propCount; i++)
    {
        PMS_PROP temp;
        temp.active = bool(read_bin<ubyte>(is));
        temp.filler1 = read_bin<ubyte>(is);
        temp.style = read_bin<word>(is);
        temp.width = read_bin<long>(is);
        temp.height = read_bin<long>(is);
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
    p.sceneryCount = read_bin<long>(is);
    for (int i = 0; i < p.sceneryCount; i++)
    {
        PMS_SCENERY temp;
        //temp.nameLen = read_bin<ubyte>( is );
        temp.name = read_string(is, 50);
        temp.timestamp = read_bin<PMS_TIMESTAMP>(is);
        p.scenery.push_back(temp);
    }
    p.colliderCount = read_bin<long>(is);
    for (int i = 0; i < p.colliderCount; i++)
    {
        PMS_COLLIDER temp;
        temp.active = bool(read_bin<ubyte>(is));
        temp.filler[0] = read_bin<ubyte>(is);
        temp.filler[1] = read_bin<ubyte>(is);
        temp.filler[2] = read_bin<ubyte>(is);
        temp.x = read_bin<float>(is);
        temp.y = read_bin<float>(is);
        temp.radius = read_bin<float>(is);
        p.collider.push_back(temp);
    }
    p.spawnpointCount = read_bin<long>(is);
    for (int i = 0; i < p.spawnpointCount; i++)
    {
        PMS_SPAWNPOINT temp;
        temp.active = bool(read_bin<ubyte>(is));
        temp.filler[0] = read_bin<ubyte>(is);
        temp.filler[1] = read_bin<ubyte>(is);
        temp.filler[2] = read_bin<ubyte>(is);
        temp.x = read_bin<long>(is);
        temp.y = read_bin<long>(is);
        temp.team = PMS_SPAWNTEAM(read_bin<unsigned long>(is));
        p.spawnpoint.push_back(temp);
    }
    p.waypointCount = read_bin<long>(is);
    for (int i = 0; i < p.waypointCount; i++)
    {
        PMS_WAYPOINT temp;
        temp.active = bool(read_bin<ubyte>(is));
        temp.filler1[0] = read_bin<ubyte>(is);
        temp.filler1[1] = read_bin<ubyte>(is);
        temp.filler1[2] = read_bin<ubyte>(is);
        temp.id = read_bin<long>(is);
        temp.x = read_bin<long>(is);
        temp.y = read_bin<long>(is);
        temp.left = bool(read_bin<ubyte>(is));
        temp.right = bool(read_bin<ubyte>(is));
        temp.up = bool(read_bin<ubyte>(is));
        temp.down = bool(read_bin<ubyte>(is));
        temp.jet = bool(read_bin<ubyte>(is));
        temp.path = read_bin<ubyte>(is);
        temp.specialAction = PMS_SPECIALACTIONS(read_bin<ubyte>(is));
        temp.c2 = read_bin<ubyte>(is);
        temp.c3 = read_bin<ubyte>(is);
        temp.filler2[0] = read_bin<ubyte>(is);
        temp.filler2[1] = read_bin<ubyte>(is);
        temp.filler2[2] = read_bin<ubyte>(is);
        temp.numConnections = read_bin<long>(is);
        for (int c=0; c<20; c++)
            temp.connections[c] = read_bin<long>(is);
        p.waypoint.push_back(temp);
    }
    is.close();
    return 0;
}




static char *font_read_str(char *tag, int i, char *font)
{
    int k = 0;

    while (tag[i] != '\r' && tag[i] != '\n')
    {
        font[k++] = tag[i++];
    }
    font[k] = '\0';
    return font;
}


static int font_read_int(char *tag, int i)
{
    int var = 0;

    while (isdigit(tag[i]))
    {
        var *= 10;
        var += (int)(tag[i++]-48);
    }
    return var;
}


int read_fonts()
{
    char tag[1024], temp_font[100];
    int i;
    ifstream cfg((SOL_PATH+"Txt/font.ini").c_str());
    if (cfg == NULL)
    {
        cout << "Error while loading fonts";
        exit(1);
    }
    char var_name[100];

    while (!cfg.eof())
    {
        cfg.getline(tag, 1024);
        if (tag[0] != '#' && tag[0] != '\n' && tag[0] != '\r' && tag[0] != '[')
        {
            i = 0;

            // read variable's name
            while (tag[i] != '=')
            {
                var_name[i] = tag[i];
                i++;
            }
            var_name[i] = '\0';

            // read variable
            i++;
            if (!strcmp(var_name, "Font1"))
            {
                font_read_str(tag, i, temp_font);
                if (!strcmp(temp_font, "BankGothic Lt BT"))
                {
                    strcpy(Font1, "bgothl.ttf");
                }
                else if (!strcmp(temp_font, "Lucida Console"))
                {
                    strcpy(Font1, "lucon.ttf");
                }
                else exit(EXIT_FAILURE);
            }
            else if (!strcmp(var_name, "Font2"))
            {
                font_read_str(tag, i, temp_font);
                if (!strcmp(temp_font, "BankGothic Lt BT"))
                {
                    strcpy(Font2, "bgothl.ttf");
                }
                else if (!strcmp(temp_font, "Lucida Console"))
                {
                    strcpy(Font2, "lucon.ttf");
                }
                else exit(EXIT_FAILURE);
            }
            else if (!strcmp(var_name, "FontMenuSize")) FontMenuSize = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontConsoleSize")) FontConsoleSize = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontBigSize")) FontBigSize = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontWeaponMenuSize")) FontWeaponMenuSize = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontConsoleSmallSize")) FontConsoleSmallSize = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontHeightScale")) FontHeightScale = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontMenuBold")) FontMenuBold = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontConsoleBold")) FontConsoleBold = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontBigBold")) FontBigBold = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontWeaponMenuBold")) FontWeaponMenuBold = font_read_int(tag, i);
            else if (!strcmp(var_name, "FontConsoleSmallBold")) FontConsoleSmallBold = font_read_int(tag, i);
            else if (!strcmp(var_name, "KillConsoleNameSpace")) KillConsoleNameSpace = font_read_int(tag, i);

        }
    }
    cfg.close();
    return 0;

}

