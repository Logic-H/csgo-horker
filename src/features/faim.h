#pragma once

#include "../sdk/types.h"
#include "../sdk/vector.h"
#include "../process.h"
#include "fbase.h"
#include <chrono>

/**
 * Description: Provides features such as aimbot/triggerbot/etc.
 */

class FAim : public FBase {
    Process &m_mem;
    public:
        FAim(Process &mem) : m_mem(mem)
        {
        }
    protected:
        void Run();
    private:
        void Aim(uintptr_t localPlayer, int myTeam);
        void Recoil(uintptr_t localPlayer);
        void DoTrigger();
        bool GetBonePosition(uintptr_t ePtr, int bone, Vector *out);
        size_t m_nTick = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_nLastTick;
        Vector m_vecOldPunchAngle;
};
