#pragma once

#include "types.h"

#define MAX_PATH 260

#define LIFE_STATE_ALIVE 0
#define LIFE_STATE_DYING 1
#define LIFE_STATE_DEAD 2

#define TEAM_NONE 0
#define TEAM_SPEC 1
#define TEAM_T    2
#define TEAM_CT   3

class CBaseEntity {
    public:
        int GetHealth() {
            return m_iHealth;
        }
        
        int GetLifeState() {
            return m_lifeState;
        }

        int GetTeam() {
            return m_iTeamNum;
        }
        
        int GetIndex() {
            return index;
        }

    private:
        char __buf_0x00[0x94]; // 0x0
        int index; // 0x94
        unsigned short m_EntClientFlags; // 0x98
        const void *model; // 0xA0
        char m_clrRender[0x4]; // 0xA8
        int m_cellbits; // 0xAC
        int m_cellwidth; // 0xB0
        int m_cellX; // 0xB4
        int m_cellY; // 0xB8
        int m_cellZ; // 0xBC
        Vector m_vecCellOrigin; // 0xC0
        Vector m_vecAbsVelocity; // 0xCC
        Vector m_vecAbsOrigin; // 0xD8
        Vector m_vecOrigin; // 0xE4
        Vector m_vecAngVelocity; // 0xF0
        QAngle m_angAbsRotation; // 0xFC
        QAngle m_angRotation; // 0x108
        float m_flGravity; // 0x114
        float m_flProxyRandomValue; // 0x118
        int m_iEFlags; // 0x11C
        bool m_nWaterType; // 0x120
        bool m_bDormant; // 0x121
        char __buf_0x122[0x2]; // 0x122
        float m_fEffects; // 0x124
        int m_iTeamNum; // 0x128
        int m_iNextThinkTick; // 0x12C
        char __buf_0x130[0x4]; // 0x130
        int m_iHealth; // 0x134
        int m_fFlags; // 0x138
        Vector m_vecViewOffset; // 0x13C
        Vector m_vecVelocity; // 0x148
        Vector m_vecBaseVelocity; // 0x154
        QAngle m_angNetworkAngles   ; // 0x160
        Vector m_vecNetworkOrigin; // 0x16C
        float m_flFriction; // 0x178
        char __buf_0x17C[0xC]; //0x17C
        char m_iName[MAX_PATH];
        short m_nModelIndex;
        unsigned char m_nRenderFX;
        unsigned char m_nRenderMode;
        unsigned char m_MoveType;
        unsigned char m_MoveCollide;
        unsigned char m_nWaterLevel;
        char m_lifeState;
};

typedef struct {
    CBaseEntity entity;
    void* entityPtr;
} EntityInfo;

typedef struct {
    Vector head;
    Vector origin;
    Vector eyes;
    Vector aimLine;
    EntityInfo entityInfo;
    char name[256];
    int health;
    bool scoped;
    bool myTeam;
    bool defusing;
    bool reloading;
    float lby;
    bool spectatingMe;
} EntityToScreen;

