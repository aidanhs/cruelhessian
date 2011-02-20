#ifndef POLYGON_H
#define POLYGON_H

/*   Polygon.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
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


// based on Olivier renault code - Polycolly



#include "../TVector2D.h"
#include <vector>



class TVector2D;

class PolyColl
{
    static bool ProjectPointOnSegment(const TVector2D& V, const TVector2D& A, const TVector2D& B, TVector2D& W);
    static void GetInterval(const std::vector<TVector2D>& axVertices,
                            const TVector2D& xAxis, float& min, float& max);

    static bool FindMTD(TVector2D* xAxis, float* taxis, int iNumAxes, TVector2D& N, float& t);

    static bool IntervalIntersect(const std::vector<TVector2D>& A,
                                  const std::vector<TVector2D>& B,
                                  const TVector2D& xAxis,
                                  const TVector2D& xOffset, const TVector2D& xVel, const TMatrix2D& xOrient,
                                  float& taxis, float tmax);

    static int FindSupportPoints(const TVector2D& N, float t, const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA, TVector2D* S);

    static bool ConvertSupportPointsToContacts(const TVector2D& N,
            TVector2D* S0, int S0num,
            TVector2D* S1, int S1num,
            TVector2D* C0,
            TVector2D* C1,
            int& Cnum);
    static TVector2D Transform(const TVector2D& Vertex, const TVector2D& P, const TVector2D& V, const TMatrix2D& xOrient, float t);

public:

    static float    CalculateInertia(const std::vector<TVector2D>& A, float mass);

  //  static TVector2D*	BuildBox	(int& iNumVertices, float width, float height);
    static std::vector<TVector2D>	BuildBlob	(int iNumVertices, float radiusx, float radiusy);

    //static void		Render		(const TVector2D& xOffset, float angle, uint32_t ARGBfill, uint32_t ARGBline, const TVector2D* axVertices, int iNumVertices, const Tex& m_currentTexture);
    static bool		Collide		(const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA,
                                 const std::vector<TVector2D>& B, const TVector2D& PB, const TVector2D& VB, const TMatrix2D& OB,
                                 TVector2D& N, float& t);

    // calculate contact points of two colliding polygons
    static bool FindContacts(const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA,
                             const std::vector<TVector2D>& B, const TVector2D& PB, const TVector2D& VB, const TMatrix2D& OB,
                             const TVector2D& N, float t,
                             TVector2D* CA,
                             TVector2D* CB,
                             int& Cnum);

    static bool		ClipSegment	(const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA,
                                 const TVector2D& xStart, const TVector2D& xEnd,
                                 float& tnear, float& tfar, TVector2D& Nnear, TVector2D& Nfar);
};



#endif
