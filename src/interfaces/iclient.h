#pragma once

#include "ibase.h"
#include "../sdk/cglowobjectmanager.h"

class IClient : public IBase {
    public:
        IClient(MemoryManager &mem, const std::string &sClient);
        void PrintOffsets();
        bool IsConnected();
        bool GetGlowManager(CGlowObjectManager &out);
        bool GetLocalPlayer(uintptr_t &out);
    private:
        std::string m_sClient;
        // Stored addresses
        uintptr_t m_aGlowPtr = 0;
        uintptr_t m_aLocalPlayer = 0;
        uintptr_t m_aForceAttack = 0;
        uintptr_t m_aPlayerResource = 0;
        uintptr_t m_aConnected = 0;
};

