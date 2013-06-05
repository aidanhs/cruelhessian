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
#include "../TMatrix2D.h"
#include "Polygon.h"
#include <iostream>




static inline void swapf(float& a, float& b)
{
    float c = a;
    a = b;
    b = c;
}


static inline float sign(float x)
{
    return (x < 0.0f)? -1.0f : 1.0f;
}



// taken from
// http://www.physicsforums.com/showthread.php?s=e251fddad79b926d003e2d4154799c14&t=25293&page=2&pp=15
float PolyColl::CalculateInertia(const std::vector<TVector2D>& A, float mass)
{

    if (A.size() == 1)	return 0.0f;

    float a, b, denom = 0.0f, numer = 0.0f;

    for (unsigned int j = A.size()-1, i = 0; i < A.size(); j = i, i++)
    {
        a = fabs(A[j] ^ A[i]);
        b = A[i]*A[i] + A[i]*A[j] + A[j]*A[j];

        denom += (a * b);
        numer += a;
    }

    return (mass / 6.0f) * (denom / numer);
}


std::vector<TVector2D> PolyColl::BuildBlob(int iNumVertices, float radiusx, float radiusy)
{
    if (iNumVertices == 0)
    {
        std::cout << "ERROR\n";
    }
        //return 0;

    std::vector<TVector2D> axVertices;
    axVertices.resize(iNumVertices);

    if (iNumVertices == 1)
    {
        axVertices[0] = TVector2D(0, 0);
        return axVertices;
    }

    float a = static_cast<float>(M_PI / iNumVertices);
    float da = 2.0f * static_cast<float>(M_PI / iNumVertices);

    for(int i = 0; i < iNumVertices; i ++)
    {
        a += da;

        axVertices[i] = TVector2D(cos(a) * radiusx, sin(a) * radiusy);
    }
    return axVertices;
}


bool PolyColl::Collide(const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA,
                       const std::vector<TVector2D>& B, const TVector2D& PB, const TVector2D& VB, const TMatrix2D& OB,
                       TVector2D& N, float& t)
{
//    if (!A || !B) return false;

    //int Anum = A.size();
    //int Bnum = B.size();

    if (A.size() < 2 && B.size() < 2)
        return false;

    TMatrix2D xOrient = OA ^ OB;
    TVector2D xOffset = (PA - PB) ^ OB;
    TVector2D xVel    = (VA - VB) ^ OB;

    // All the separation axes
    TVector2D xAxis[64]; // note : a maximum of 32 vertices per poly is supported
    float  taxis[64];
    int    iNumAxes=0;

    float fVel2 = xVel * xVel;

    if (fVel2 > 0.00001f)
    {
        xAxis[iNumAxes] = TVector2D(-xVel.y, xVel.x);

        if (!IntervalIntersect(	A,
                                B,
                                xAxis[iNumAxes],
                                xOffset, xVel, xOrient,
                                taxis[iNumAxes], t))
        {
            return false;
        }
        iNumAxes++;
    }

    if (A.size() > 1)
    {
        // test separation axes of A
        for (unsigned int j = A.size()-1, i = 0; i < A.size(); j = i, i++)
        {
            TVector2D E0 = A[j];
            TVector2D E1 = A[i];
            TVector2D E  = E1 - E0;
            xAxis[iNumAxes] = TVector2D(-E.y, E.x) * xOrient;

            if (!IntervalIntersect(	A,
                                    B,
                                    xAxis[iNumAxes],
                                    xOffset, xVel, xOrient,
                                    taxis[iNumAxes], t))
            {
                return false;
            }

            iNumAxes++;
        }
    }

    // test separation axes of B
    if (B.size() > 1)
    {
        for (unsigned int j = B.size()-1, i = 0; i < B.size(); j = i, i++)
        {
            TVector2D E0 = B[j];
            TVector2D E1 = B[i];
            TVector2D E  = E1 - E0;
            xAxis[iNumAxes] = TVector2D(-E.y, E.x);

            if (!IntervalIntersect(	A,
                                    B,
                                    xAxis[iNumAxes],
                                    xOffset, xVel, xOrient,
                                    taxis[iNumAxes], t))
            {
                return false;
            }

            iNumAxes++;
        }
    }

    // special case for segments
    if (B.size() == 2)
    {
        TVector2D E  = B[1] - B[0];
        xAxis[iNumAxes] = E;

        if (!IntervalIntersect(	A,
                                B,
                                xAxis[iNumAxes],
                                xOffset, xVel, xOrient,
                                taxis[iNumAxes], t))
        {
            return false;
        }

        iNumAxes++;
    }

    // special case for segments
    if (A.size() == 2)
    {
        TVector2D E  = A[1] - A[0];
        xAxis[iNumAxes] = E * xOrient;

        if (!IntervalIntersect(	A, B,
                                xAxis[iNumAxes],
                                xOffset, xVel, xOrient,
                                taxis[iNumAxes], t))
        {
            return false;
        }

        iNumAxes++;
    }

    if (!FindMTD(xAxis, taxis, iNumAxes, N, t))
        return false;

    // make sure the polygons gets pushed away from each other.
    if (N * xOffset < 0.0f)
        N = -N;

    N *= OB;

    return true;
}

// calculate the projection range of a polygon along an axis
void PolyColl::GetInterval(const std::vector<TVector2D>& axVertices, const TVector2D& xAxis, float& min, float& max)
{
    min = max = (axVertices[0] * xAxis);
    float d;

    for (unsigned int i = 1; i < axVertices.size(); ++i)
    {
        d = axVertices[i] * xAxis;
        if (d < min)
            min = d;
        else if (d > max)
            max = d;
    }
}

bool PolyColl::IntervalIntersect(const std::vector<TVector2D>& A,
                                 const std::vector<TVector2D>& B,
                                 const TVector2D& xAxis,
                                 const TVector2D& xOffset, const TVector2D& xVel, const TMatrix2D& xOrient,
                                 float& taxis, float tmax)
{
    float min0, max0;
    float min1, max1;
    GetInterval(A, xAxis ^ xOrient, min0, max0);
    GetInterval(B, xAxis, min1, max1);

    float h = xOffset * xAxis;
    min0 += h;
    max0 += h;

    float d0 = min0 - max1; // if overlapped, do < 0
    float d1 = min1 - max0; // if overlapped, d1 > 0

    // separated, test dynamic intervals
    if (d0 > 0.0f || d1 > 0.0f)
    {
        float v = xVel * xAxis;

        // small velocity, so only the overlap test will be relevant.
        if (fabs(v) < 0.0000001f)
            return false;

        float t0 =-d0 / v; // time of impact to d0 reaches 0
        float t1 = d1 / v; // time of impact to d0 reaches 1

        if (t0 > t1)
        {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        taxis  = (t0 > 0.0f)? t0 : t1;

        if (taxis < 0.0f || taxis > tmax)
            return false;

        return true;
    }
    else
    {
        // overlap. get the interval, as a the smallest of |d0| and |d1|
        // return negative number to mark it as an overlap
        taxis = (d0 > d1)? d0 : d1;
        return true;
    }
}


bool PolyColl::FindMTD(TVector2D* xAxis, float* taxis, int iNumAxes, TVector2D& N, float& t)
{
    // find collision first
    int mini = -1;
    t = 0.0f;
    for (int i = 0; i < iNumAxes; ++i)
    {
        if (taxis[i] > 0)
        {
            if (taxis[i] > t)
            {
                mini = i;
                t = taxis[i];
                N = xAxis[i];
                N.normalize();
            }
        }
    }

    // found one
    if (mini != -1)
        return true;

    // nope, find overlaps
    mini = -1;
    for (int i = 0; i < iNumAxes; ++i)
    {
        float n = xAxis[i].normalize();
        taxis[i] /= n;

        if (taxis[i] > t || mini == -1)
        {
            mini = i;
            t = taxis[i];
            N = xAxis[i];
        }
    }

    if (mini == -1)
        std::cout << "Error\n";

    return (mini != -1);
}


//-----------------------------------------------------------------------------------------------
// Find closest point on a segment to a vertex
//-----------------------------------------------------------------------------------------------
bool PolyColl::ProjectPointOnSegment(const TVector2D& V, const TVector2D& A, const TVector2D& B, TVector2D& W)
{

    TVector2D AV = V - A;
    TVector2D AB = B - A;
    float t = (AV * AB) / (AB * AB);

    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;

    W = A + t * AB;

    return true;
}

//-----------------------------------------------------------------------------------------------
// Tranform a contact point into world collision space
//-----------------------------------------------------------------------------------------------
TVector2D PolyColl::Transform(const TVector2D& Vertex, const TVector2D& P, const TVector2D& V, const TMatrix2D& xOrient, float t)
{
    TVector2D T = P + (Vertex * xOrient);

    if(t > 0.0f)
        T += V * t;

    return T;
}

//-----------------------------------------------------------------------------------------------
// find the support points of a convex shape along a direction
//-----------------------------------------------------------------------------------------------
int PolyColl::FindSupportPoints(const TVector2D& N, float t, const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA, TVector2D* S)
{
    TVector2D Norm = N ^ OA;

    float d[32];
    float dmin;
    dmin = d[0] = A[0] * Norm;

    for(unsigned int i = 1; i < A.size(); i ++)
    {
        d[i] = A[i] * Norm;

        if (d[i] < dmin)
        {
            dmin = d[i];
        }
    }

    // we will limit the number of support points to only 2.
    // if we have more than 2 support points, we take the extremums.
    int Snum = 0;
    const float threshold = 1.0E-3f;
    float s[2];
    float sign = false;

    TVector2D Perp(-Norm.y, Norm.x);

    for(unsigned int i = 0; i < A.size(); i ++)
    {
        if (d[i] < dmin + threshold)
        {
            TVector2D Contact = Transform(A[i], PA, VA, OA, t);

            float c = Contact * Perp;

            if (Snum < 2)
            {
                s[Snum] = c;
                S[Snum] = Contact;
                Snum++;

                if (Snum > 1)
                {
                    sign = (s[1] > s[0]);
                }
            }
            else
            {
                float&  min = (sign)? s[0] : s[1];
                float&  max = (sign)? s[1] : s[0];
                TVector2D& Min = (sign)? S[0] : S[1];
                TVector2D& Max = (sign)? S[1] : S[0];

                if (c < min)
                {
                    min = c;
                    Min = Contact;
                }
                else if (c > max)
                {
                    max = c;
                    Max = Contact;
                }
            }
        }
    }
    return Snum;
}

// convert sets of support points to contact points.
bool PolyColl::ConvertSupportPointsToContacts(const TVector2D& N,
        TVector2D* S0, int S0num,
        TVector2D* S1, int S1num,
        TVector2D* C0,
        TVector2D* C1,
        int& Cnum)
{
    Cnum = 0;

    if (S0num == 0 || S1num == 0)
        return false;

    if (S0num == 1 && S1num == 1)
    {
        C0[Cnum] = S0[0];
        C1[Cnum] = S1[0];
        Cnum++;
        return true;
    }

    TVector2D xPerp(-N.y, N.x);

    float min0 = S0[0] * xPerp;
    float max0 = min0;
    float min1 =  S1[0] * xPerp;
    float max1 = min1;

    if (S0num == 2)
    {
        max0 = S0[1] * xPerp;

        if (max0 < min0)
        {
            swapf(min0, max0);

            TVector2D T = S0[0];
            S0[0] = S0[1];
            S0[1] = T;
        }
    }

    if (S1num == 2)
    {
        max1 = S1[1] * xPerp;

        if (max1 < min1)
        {
            swapf(min1, max1);

            TVector2D T = S1[0];
            S1[0] = S1[1];
            S1[1] = T;
        }
    }

    if (min0 > max1 || min1 > max0)
        return false;

    if (min0 > min1)
    {
        TVector2D Pseg;
        if (ProjectPointOnSegment(S0[0], S1[0], S1[1], Pseg))
        {
            C0[Cnum] = S0[0];
            C1[Cnum] = Pseg;
            Cnum++;
        }
    }
    else
    {
        TVector2D Pseg;
        if (ProjectPointOnSegment(S1[0], S0[0], S0[1], Pseg))
        {
            C0[Cnum] = Pseg;
            C1[Cnum] = S1[0];
            Cnum++;
        }
    }

    if (max0 != min0 && max1 != min1)
    {
        if (max0 < max1)
        {
            TVector2D Pseg;
            if (ProjectPointOnSegment(S0[1], S1[0], S1[1], Pseg))
            {
                C0[Cnum] = S0[1];
                C1[Cnum] = Pseg;
                Cnum++;
            }
        }
        else
        {
            TVector2D Pseg;
            if (ProjectPointOnSegment(S1[1], S0[0], S0[1], Pseg))
            {
                C0[Cnum] = Pseg;
                C1[Cnum] = S1[1];
                Cnum++;
            }
        }
    }
    return true;
}

bool PolyColl::FindContacts(const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA,
                            const std::vector<TVector2D>& B, const TVector2D& PB, const TVector2D& VB, const TMatrix2D& OB,
                            const TVector2D& N, float t,
                            TVector2D* CA,
                            TVector2D* CB,
                            int& Cnum)
{
    TVector2D S0[4];
    TVector2D S1[4];
    int S0num = FindSupportPoints( N, t, A, PA, VA, OA, S0);
    int S1num = FindSupportPoints(-N, t, B, PB, VB, OB, S1);

    if (!ConvertSupportPointsToContacts(N, S0, S0num, S1, S1num, CA, CB, Cnum))
        return false;

    return true;
}

// clip a ray to a polygon
bool PolyColl::ClipSegment(const std::vector<TVector2D>& A, const TVector2D& PA, const TVector2D& VA, const TMatrix2D& OA,
                           const TVector2D& xStart, const TVector2D& xEnd,
                           float& tnear, float& tfar, TVector2D& Nnear, TVector2D& Nfar)
{
//    if (!A) return false;

//    int Anum = A.size();
    TVector2D xPos = xStart - PA;
    TVector2D xDir = xEnd - xStart;
    xDir ^= OA;
    xPos ^= OA;

    // test separation axes of A
    for(unsigned int j = A.size()-1, i = 0; i < A.size(); j = i, i ++)
    {

        TVector2D E  = A[i] - A[j];
        TVector2D En(E.y, -E.x);
        TVector2D D = A[j] - xPos;

        float denom = D * En;
        float numer = xDir * En;

        // ray parallel to plane
        if (fabs(numer) < 1.0E-8f)
        {
            // origin outside the plane, no intersection
            if (denom < 0.0f)
                return false;
        }
        else
        {
            float tclip = denom / numer;

            // near intersection
            if (numer < 0.0f)
            {
                if (tclip > tfar)
                    return false;

                if (tclip > tnear)
                {
                    tnear = tclip;
                    Nnear = En;
                    Nnear.normalize();
                    Nnear *= OA;
                }
            }
            // far intersection
            else
            {
                if (tclip < tnear)
                    return false;

                if (tclip < tfar)
                {
                    tfar = tclip;
                    Nfar = En;
                    Nfar.normalize();
                    Nfar *= OA;
                }
            }
        }
    }

    return true;
}
