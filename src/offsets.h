#pragma once

#include "process.h"
#include <cstdint>

class Signatures {
    public:
        static bool Find(Process &mem);
        static void Print(Process &mem);
};

namespace Offset {
    namespace Client {
        extern uintptr_t EntityList;
        extern uintptr_t ForceAttack;
        extern uintptr_t LocalPlayer;
        extern uintptr_t GlowObjectManager;
        extern uintptr_t PlayerResources;
    };
    namespace Engine {
        extern uintptr_t IsConnected;
    };
};

namespace Netvar {
    namespace CBaseEntity {
        constexpr size_t index = 0x94;
        constexpr size_t m_angRotation = 0x108;
        constexpr size_t m_bDormant = 0x121;
        constexpr size_t m_fFlags = 0x124;
        constexpr size_t m_iTeamNum = 0x128;
        constexpr size_t m_vecViewOffset = 0x13c;
        constexpr size_t m_vecOrigin = 0xe4;
        constexpr size_t m_nModelIndex = 0x28c;
        constexpr size_t m_lifeState = 0x293;
    };
    namespace CBasePlayer {
        constexpr size_t m_flFlashDuration = 0xABE8;
        constexpr size_t m_iCrosshairID = 0xBBD0;
        constexpr size_t m_Local = 0x36f0;
        namespace Local {
            constexpr size_t m_aimPunchAngle = m_Local + 0x74;
        };
    };
};
