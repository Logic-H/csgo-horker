#pragma once

#include "ibase.h"

class IEngine : public IBase {
    public:
        IEngine(MemoryManager &mem);
        void Update();
        bool IsConnected();
    private:
        // Stored addresses
        uintptr_t m_aConnected = 0;

        // Updating variables
        bool m_bConnected = false;
};

