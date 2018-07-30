#pragma once

#include "cbaseentity.h"
#include "cutlvector.h"

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2
struct GlowObjectDefinition_t
{
    bool ShouldDraw( int nSlot ) const
    {
        return m_pEntity && ( m_nSplitScreenSlot == -1 || m_nSplitScreenSlot == nSlot ) && ( m_bRenderWhenOccluded || m_bRenderWhenUnoccluded );
    }

    bool IsUnused() const
    {
        return m_nNextFreeSlot != ENTRY_IN_USE;
    }

    long writeStart()
    {
        return (long(&(this)->m_flGlowColor) - long(this));
    }

    long writeEnd()
    {
        return (long(&(this)->m_nNextFreeSlot) - long(this));
    }

    void SetColor(float r, float g, float b, float a)
    {
        m_flGlowColor[0] = r;
        m_flGlowColor[1] = g;
        m_flGlowColor[2] = b;
        m_flGlowAlpha = a;
    }

    void SetRender(bool occluded, bool unoccluded)
    {
        m_bRenderWhenOccluded = occluded;
        m_bRenderWhenUnoccluded = unoccluded;
    }

    CBaseEntity* m_pEntity;
    float m_flGlowColor[3];
    float m_flGlowAlpha;
    char  unknown[4];
    float m_flMovementAlpha;
    float m_flBloomAmount;
    float m_localPlayerIsZeroPoint3;
    bool  m_bRenderWhenOccluded;
    bool  m_bRenderWhenUnoccluded;
    bool  m_bFullBloomRender;
    char  m_unknown[1];
    int   m_nFullBloomStencilTestValue;
    int   m_nGlowStyle;
    int   m_nSplitScreenSlot;
    int   m_nNextFreeSlot;

}; // sizeof() == 0x34

class CGlowObjectManager
{
public:
    size_t Count() {
        return m_GlowObjectDefinitions.Count();
    }

    GlowObjectDefinition_t *Data() {
        return m_GlowObjectDefinitions.Data();
    }
private:
    CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions;
    int m_nFirstFreeSlot;
/*
    unsigned int unk1;
    unsigned int unk2;
    unsigned int unk3;
    unsigned int unk4;
    unsigned int unk5;
*/
};

