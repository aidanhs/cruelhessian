//------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Created by    : OR - 07/02/2004 12:42:04
//
// Copyright (C) : 2004 Rebellion, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description   :
// ---------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------



#ifndef __CMATERIAL_H__
#define __CMATERIAL_H__




class CMaterial
{
private:
    float m_fCoF, m_fCoR, m_fCoS, m_fSep;

public:
    CMaterial(float fCoF = 0.8f, float fCoR = 0.3f, float fCoS = 0.8f, float fSep=0.5f)
        : m_fCoF(fCoF)
        , m_fCoR(fCoR)
        , m_fCoS(fCoS)
        , m_fSep(fSep)
    {}

    void SetSeparation		(float fSep)
    {
        m_fSep = fSep;
    }
    void SetFriction		(float fCoF)
    {
        m_fCoF = fCoF;
    }
    void SetStaticFriction	(float fCoS)
    {
        m_fCoS = fCoS;
    }
    void SetRestitution		(float fCoR)
    {
        m_fCoR = fCoR;
    }

    float GetSeparation		() const
    {
        return m_fSep;
    }
    float GetFriction		() const
    {
        return m_fCoF;
    }
    float GetStaticFriction	() const
    {
        return m_fCoS;
    }
    float GetRestitution	() const
    {
        return m_fCoR;
    }


};

// HACK : use a shared material for all objects
//extern CMaterial s_xContactMaterial;

#endif
