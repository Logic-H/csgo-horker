#pragma once

#include "ibase.h"
#include "../sdk/cbaseentity.h"
#include "../sdk/cglowobjectmanager.h"
#include "../sdk/types.h"

class IClient : public IBase {
    public:
        IClient(MemoryManager &mem);
        void Update();
        CGlowObjectManager GetGlowObjectManager();
        CBaseEntity GetLocalPlayerEntity();
        uintptr_t GetLocalPlayer();
        void SetForceAttack(bool enabled);
    private:
        // Stored addresses
        uintptr_t m_aGlowPtr = 0;
        uintptr_t m_aLocalPlayer = 0;
        uintptr_t m_aForceAttack = 0;
        uintptr_t m_aPlayerResource = 0;

        // Updated variables
        uintptr_t m_pLocalPlayer = 0;
        uintptr_t m_pGlowMgr = 0;
};

