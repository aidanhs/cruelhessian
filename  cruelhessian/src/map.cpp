/*   map.cpp
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
#include <iostream>


#include "map.h"
#include "parser/SimpleIni.h"



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
    is.read( &tmp[0], tmp.size() );
    return std::string(tmp.begin(), tmp.begin() + length);
}


std::string Map::getTextPoly() const
{
    return texture;
}


std::vector<std::string> Map::getTextScen() const
{
    std::vector<std::string> text_name(sceneryCount);

    for (int i = 0; i < sceneryCount; ++i)
        text_name[i] = scenery[i].name;
    return text_name;
}


void Map::setPoly(GLuint text)
{
    text_poly = text;
}


void Map::setScen(GLuint *text)
{
    text_scen = new GLuint[sceneryCount];

    for (int i = 0; i < sceneryCount; ++i)
        text_scen[i] = text[i];
}


Map::Map(const std::string& mname)
{

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
        polygon.push_back(temp);

    }
    sectorDivisions = read_bin<int>(is);
    numSectors = read_bin<int>(is);

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

}


void Map::drawBack()
{
    // glEnable(GL_TEXTURE_2D);

    // draw scenery on the back
    for (size_t i = 0; i < prop.size(); ++i)
    {
        if (prop[i].level == dbBEHIND_ALL)
        {
            glPushMatrix();
            glTranslatef(prop[i].x, prop[i].y, 0.0);
            glRotatef(-prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(prop[i].scaleX, prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, text_scen[prop[i].style-1]);

            glColor4ub(prop[i].color.red, prop[i].color.green, prop[i].color.blue, prop[i].alpha);

            glBegin(GL_QUADS);
            //glColor4ubv(reinterpret_cast<GLubyte*>(&prop[i].color));
            glTexCoord2i(0, 1);
            glVertex2f(0.0, 0.0);
            glTexCoord2i(1, 1);
            glVertex2f(static_cast<float>(prop[i].width), 0.0);
            glTexCoord2i(1, 0);
            glVertex2f(static_cast<float>(prop[i].width), static_cast<float>(prop[i].height));
            glTexCoord2i(0, 0);
            glVertex2f(0.0, static_cast<float>(prop[i].height));

            glEnd();

            glPopMatrix();
        }
    }

    glColor3f(1.0f, 1.0f, 1.0f);

}


void Map::drawFront()
{
    // glEnable(GL_TEXTURE_2D);

    // draw scenery on the back
    for (size_t i = 0; i < prop.size(); ++i)
    {
        if (prop[i].level == dbBEHIND_MAP)
        {
            glPushMatrix();
            glTranslatef(prop[i].x, prop[i].y, 0.0);
            glRotatef(-prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(prop[i].scaleX, prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, text_scen[prop[i].style-1]);

            glColor4ub(prop[i].color.red, prop[i].color.green, prop[i].color.blue, prop[i].alpha);

            glBegin(GL_QUADS);
            //glColor4ubv(reinterpret_cast<GLubyte*>(&prop[i].color));
            glTexCoord2i(0, 1);
            glVertex2f(0.0, 0.0);
            glTexCoord2i(1, 1);
            glVertex2f(static_cast<float>(prop[i].width), 0.0);
            glTexCoord2i(1, 0);
            glVertex2f(static_cast<float>(prop[i].width), static_cast<float>(prop[i].height));
            glTexCoord2i(0, 0);
            glVertex2f(0.0, static_cast<float>(prop[i].height));

            glEnd();

            glPopMatrix();
        }
    }

    glBindTexture(GL_TEXTURE_2D, text_poly);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < polygonCount; ++i)
    {
        //glColor4ubv(reinterpret_cast<GLubyte*>(&polygon[i].vertex[0].color));
        glColor4ub(polygon[i].vertex[0].color.red,
                   polygon[i].vertex[0].color.green,
                   polygon[i].vertex[0].color.blue,
                   polygon[i].vertex[0].color.alpha);
        glTexCoord3f(polygon[i].vertex[0].tu, -polygon[i].vertex[0].tv, 1.0f / polygon[i].vertex[0].rhw);
        glVertex3f(polygon[i].vertex[0].x, polygon[i].vertex[0].y, polygon[i].vertex[0].z);

        //glColor4ubv(reinterpret_cast<GLubyte*>(&polygon[i].vertex[1].color));
        glColor4ub(polygon[i].vertex[1].color.red,
                   polygon[i].vertex[1].color.green,
                   polygon[i].vertex[1].color.blue,
                   polygon[i].vertex[1].color.alpha);
        glTexCoord3f(polygon[i].vertex[1].tu, -polygon[i].vertex[1].tv, 1.0f / polygon[i].vertex[1].rhw);
        glVertex3f(polygon[i].vertex[1].x, polygon[i].vertex[1].y, polygon[i].vertex[1].z);

        //glColor4ubv(reinterpret_cast<GLubyte*>(&polygon[i].vertex[2].color));
        glColor4ub(polygon[i].vertex[2].color.red,
                   polygon[i].vertex[2].color.green,
                   polygon[i].vertex[2].color.blue,
                   polygon[i].vertex[2].color.alpha);
        glTexCoord3f(polygon[i].vertex[2].tu, -polygon[i].vertex[2].tv, 1.0f / polygon[i].vertex[2].rhw);
        glVertex3f(polygon[i].vertex[2].x, polygon[i].vertex[2].y, polygon[i].vertex[2].z);
    }
    glEnd();




    // draw scenery on the front
    for (size_t i = 0; i < prop.size(); ++i)
    {
        if (prop[i].level == dbBEHIND_NONE)
        {
            glPushMatrix();
            glTranslatef(prop[i].x, prop[i].y, 0.0);
            glRotatef(-prop[i].rotation * _180overpi, 0.0, 0.0, 1.0);
            glScalef(prop[i].scaleX, prop[i].scaleY, 0.0);
            glBindTexture(GL_TEXTURE_2D, text_scen[prop[i].style-1]);

            glColor4ub(prop[i].color.red, prop[i].color.green, prop[i].color.blue, prop[i].alpha);

            glBegin(GL_QUADS);

            //glColor4ubv(reinterpret_cast<GLubyte*>(&prop[i].color));
            glTexCoord2i(0, 1);
            glVertex2f(0.0, 0.0);
            glTexCoord2i(1, 1);
            glVertex2f(static_cast<float>(prop[i].width), 0.0);
            glTexCoord2i(1, 0);
            glVertex2f(static_cast<float>(prop[i].width), static_cast<float>(prop[i].height));
            glTexCoord2i(0, 0);
            glVertex2f(0.0, static_cast<float>(prop[i].height));
            glEnd();

            glPopMatrix();
        }
    }
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
}
