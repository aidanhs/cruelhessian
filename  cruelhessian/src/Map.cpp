/*   Map.cpp
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
 * aint with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


// based on mar77a code



#include <fstream>
#include <vector>
#include <iostream>
#include "boost/filesystem/fstream.hpp"
#include "Map.h"
#include "TVector2D.h"
#include "Body.h"
#include "ParserManager.h"
#include "TexturesLoader.h"
#include "PhysicsManager.h"
#include "physics/ContactListener.h"


const float _180overpi = 57.29f;

template<class T> T read_bin(std::istream& is)
{
    T tmp;
    is.read(reinterpret_cast<char*>(&tmp), sizeof(T));
    return tmp;
}

static std::string read_string(std::istream& is, int full_length)
{
    int length = read_bin<char>(is);
    std::vector<char> tmp(full_length);
    is.read(&tmp[0], tmp.size());
    return std::string(tmp.begin(), tmp.begin() + length);
}


Map::Map(const std::string& mname)
{

    std::cout << "Loading map ... " << std::endl;


    if (!boost::filesystem::exists(mname))
    {
        std::cerr << "Cannot find map file : " << mname << std::endl;
        exit(1);
    }

    std::cout << "   loading map file ... " << std::endl;

    std::ifstream is(mname.c_str(), std::ios::binary);
    version = read_bin<int>(is);
    name = read_string(is, 38);
    texture = read_string(is, 24);
    bgColorTop = read_bin<PMS_COLOR>(is);
    bgColorBottom = read_bin<PMS_COLOR>(is);
    jetAmount = read_bin<int>(is);
    grenades = read_bin<ubyte>(is);
    medikits = read_bin<ubyte>(is);
    weather = PMS_WEATHERTYPE(read_bin<ubyte>(is));
    steps = PMS_STEPSTYPE(read_bin<ubyte>(is));
    randID = read_bin<int>(is);
    polygonCount = read_bin<int>(is);

    for (int i = 0; i < polygonCount; ++i)
    {
        PMS_POLYGON temp;
        temp.vertex[0] = read_bin<PMS_VERTEX>(is);
        temp.vertex[1] = read_bin<PMS_VERTEX>(is);
        temp.vertex[2] = read_bin<PMS_VERTEX>(is);
        temp.perpendicular[0] = read_bin<PMS_VECTOR>(is);
        temp.perpendicular[1] = read_bin<PMS_VECTOR>(is);
        temp.perpendicular[2] = read_bin<PMS_VECTOR>(is);
        temp.polyType = PMS_POLYTYPE(read_bin<ubyte>(is));

        //if (temp.polyType != ptNO_COLLIDE)
        {
            //TVector2D* axVertices = new TVector2D[3];
            std::vector<TVector2D> axVertices;
            axVertices.resize(3);

            axVertices[0] = TVector2D(0, 0);
            axVertices[1] = TVector2D(temp.vertex[1].x - temp.vertex[0].x, temp.vertex[1].y - temp.vertex[0].y);
            axVertices[2] = TVector2D(temp.vertex[2].x - temp.vertex[0].x, temp.vertex[2].y - temp.vertex[0].y);

            Body *body = new Body();
            body->Set(TVector2D(temp.vertex[0].x, temp.vertex[0].y), TVector2D(0,0), axVertices, 0.0);
            body->SetCollisionCallback(HandleContact);
            body->type = TYPE_POLYGON;
       //     body->number = i;
            //bod.push_back(bodd);
            Physics.m_staticObj.push_back(body);
            //world.addBody(bodd);
        }

        polygon.push_back(temp);

    }

    sectorDivisions = read_bin<int>(is);
    numSectors = read_bin<int>(is);
    //std::cout << "SECT " << sectorDivisions << " num " << numSectors << std::endl;

    topoffs = static_cast<float>(sectorDivisions * -numSectors);
    bottomoffs = static_cast<float>(sectorDivisions * numSectors);
    leftoffs = static_cast<float>(sectorDivisions * -numSectors);
    rightoffs = static_cast<float>(sectorDivisions * numSectors);

    for (int i = 0; i < ((numSectors*2)+1)*((numSectors*2)+1); ++i)
    {
        PMS_SECTOR temp;
        temp.polyCount = read_bin<word>(is);
        for (int m = 0; m < temp.polyCount; ++m)
        {
            temp.polys.push_back(read_bin<word>(is));
        }
        sector.push_back(temp);
    }
    propCount = read_bin<int>(is);
    for (int i = 0; i < propCount; ++i)
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
        prop.push_back(temp);
    }
    sceneryCount = read_bin<int>(is);
    for (int i = 0; i < sceneryCount; ++i)
    {
        PMS_SCENERY temp;
        //temp.nameLen = read_bin<ubyte>( is );
        temp.name = read_string(is, 50);
        temp.timestamp = read_bin<PMS_TIMESTAMP>(is);


       // std::cout << "DATE " << (temp.timestamp.date.year - 1980) << 9 | temp.timestamp.date.month << 5 | temp.timestamp.date.day << std::endl;;
       // std::cout << " TIME " << temp.timestamp.time.hour << 11 | temp.timestamp.time.minute << 5 | temp.timestamp.time.second / 2 << std::endl;
/*
       int wYear = (temp.timestamp.date.year >> 9) + 1980;
       int wMonth = (temp.timestamp.date.month >> 5) & 0xf;
       int wDay = temp.timestamp.date.day & 0x1f;
       std::cout << "YERDS " << wYear << " MON " << wMonth << " day " << wDay << std::endl;

       int wHour = temp.timestamp.time.hour >> 11;
       int wMinute = (temp.timestamp.time.minute >> 5) & 0x3f;
       int wSecond = (temp.timestamp.time.second & 0x1f) << 1;

       std::cout << "hour " << wHour << " Min " << wMinute << " sec " << wSecond << std::endl;
*/
        scenery.push_back(temp);
    }
    colliderCount = read_bin<int>(is);
    for (int i = 0; i < colliderCount; ++i)
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
        collider.push_back(temp);
    }
    spawnpointCount = read_bin<int>(is);
    //std::cout << "SPAWMsd " << p.spawnpointCount << std::endl;
    for (int i = 0; i < spawnpointCount; ++i)
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
        spawnpoint.push_back(temp);
    }
    waypointCount = read_bin<int>(is);
    for (int i = 0; i < waypointCount; ++i)
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

        for (int c = 0; c < 20; ++c)
            temp.connections[c] = read_bin<int>(is);

        waypoint.push_back(temp);
    }
    is.close();


    std::cout << "   loading scenery textures ... " << std::flush;

    text_scen = new GLuint[sceneryCount];

    for (unsigned int i = 0; i < scenery.size(); ++i)
    {
        text_scen[i] = Texture::Load(Parser.SOL_PATH + "Scenery-gfx/", scenery[i].name);
    }

    std::cout << "   DONE ... " << std::endl;

    std::cout << "   loading triangles texture ... " << std::endl;
    text_poly = Texture::Load(Parser.SOL_PATH + "Textures/", texture);


    std::cout << "   loading weather textures ... " << std::endl;
    if (weather != wtNONE)
    {
        if (weather == wtRAIN)
            text_weath = Texture::LoadExt(Parser.SOL_PATH + "Sparks-gfx/", "rain");
        else if (weather == wtSANDSTORM)
            text_weath = Texture::LoadExt(Parser.SOL_PATH + "Sparks-gfx/", "sand");
        else if (weather == wtSNOW)
            text_weath = Texture::LoadExt(Parser.SOL_PATH + "Sparks-gfx/", "snow");
    }


}


void MapBack::Draw() const
{

    glEnable(GL_TEXTURE_2D);

    // draw scenery on the back
    for (unsigned int i = 0; i < map.prop.size(); ++i)
    {
        if (map.prop[i].level == map.dbBEHIND_ALL)
        {
            glPushMatrix();
            glTranslatef(map.prop[i].x, map.prop[i].y, 0.0);
            glRotatef(-map.prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(map.prop[i].scaleX, map.prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, map.text_scen[map.prop[i].style-1]);

            glColor4ub(map.prop[i].color.red, map.prop[i].color.green, map.prop[i].color.blue, map.prop[i].alpha);

            glBegin(GL_QUADS);
            glTexCoord2i(0, 1);
            glVertex2i(0, 0);
            glTexCoord2i(1, 1);
            glVertex2i(map.prop[i].width, 0);
            glTexCoord2i(1, 0);
            glVertex2i(map.prop[i].width, map.prop[i].height);
            glTexCoord2i(0, 0);
            glVertex2i(0, map.prop[i].height);

            glEnd();

            glPopMatrix();
        }
    }

	glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

}


void MapFront::Draw() const
{
    glEnable(GL_TEXTURE_2D);

    // draw scenery on the back
    for (unsigned int i = 0; i < map.prop.size(); ++i)
    {
        if (map.prop[i].level == map.dbBEHIND_MAP)
        {
            glPushMatrix();
            glTranslatef(map.prop[i].x, map.prop[i].y, 0.0);
            glRotatef(-map.prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(map.prop[i].scaleX, map.prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, map.text_scen[map.prop[i].style-1]);

            glColor4ub(map.prop[i].color.red, map.prop[i].color.green, map.prop[i].color.blue, map.prop[i].alpha);

            glBegin(GL_QUADS);
            glTexCoord2i(0, 1);
            glVertex2i(0, 0);
            glTexCoord2i(1, 1);
            glVertex2i(map.prop[i].width, 0);
            glTexCoord2i(1, 0);
            glVertex2i(map.prop[i].width, map.prop[i].height);
            glTexCoord2i(0, 0);
            glVertex2i(0, map.prop[i].height);

            glEnd();

            glPopMatrix();
        }
    }

    glBindTexture(GL_TEXTURE_2D, map.text_poly);
    glBegin(GL_TRIANGLES);
    for (long i = 0; i < map.polygonCount; ++i)
    {

        glColor4ub(map.polygon[i].vertex[0].color.red,
                   map.polygon[i].vertex[0].color.green,
                   map.polygon[i].vertex[0].color.blue,
                   map.polygon[i].vertex[0].color.alpha);
        glTexCoord3f(map.polygon[i].vertex[0].tu, -map.polygon[i].vertex[0].tv, 1.0f / map.polygon[i].vertex[0].rhw);
        glVertex3f(map.polygon[i].vertex[0].x, map.polygon[i].vertex[0].y, map.polygon[i].vertex[0].z);

        glColor4ub(map.polygon[i].vertex[1].color.red,
                   map.polygon[i].vertex[1].color.green,
                   map.polygon[i].vertex[1].color.blue,
                   map.polygon[i].vertex[1].color.alpha);
        glTexCoord3f(map.polygon[i].vertex[1].tu, -map.polygon[i].vertex[1].tv, 1.0f / map.polygon[i].vertex[1].rhw);
        glVertex3f(map.polygon[i].vertex[1].x, map.polygon[i].vertex[1].y, map.polygon[i].vertex[1].z);

        glColor4ub(map.polygon[i].vertex[2].color.red,
                   map.polygon[i].vertex[2].color.green,
                   map.polygon[i].vertex[2].color.blue,
                   map.polygon[i].vertex[2].color.alpha);
        glTexCoord3f(map.polygon[i].vertex[2].tu, -map.polygon[i].vertex[2].tv, 1.0f / map.polygon[i].vertex[2].rhw);
        glVertex3f(map.polygon[i].vertex[2].x, map.polygon[i].vertex[2].y, map.polygon[i].vertex[2].z);
    }
    glEnd();




    // draw scenery on the front
    for (unsigned int i = 0; i < map.prop.size(); ++i)
    {
        if (map.prop[i].level == map.dbBEHIND_NONE)
        {
            glPushMatrix();
            glTranslatef(map.prop[i].x, map.prop[i].y, 0.0);
            glRotatef(-map.prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(map.prop[i].scaleX, map.prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, map.text_scen[map.prop[i].style-1]);

            glColor4ub(map.prop[i].color.red, map.prop[i].color.green, map.prop[i].color.blue, map.prop[i].alpha);

            glBegin(GL_QUADS);

            glTexCoord2i(0, 1);
            glVertex2i(0, 0);
            glTexCoord2i(1, 1);
            glVertex2i(map.prop[i].width, 0);
            glTexCoord2i(1, 0);
            glVertex2i(map.prop[i].width, map.prop[i].height);
            glTexCoord2i(0, 0);
            glVertex2i(0, map.prop[i].height);
            glEnd();

            glPopMatrix();
        }
    }

	glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

}


Map::~Map()
{

    waypoint.clear();
    spawnpoint.clear();
    collider.clear();
    polygon.clear();
    sector.clear();
    prop.clear();
    scenery.clear();
    texture.clear();

    delete[] text_scen;

}
