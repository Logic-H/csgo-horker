#pragma once

#include "../sdk/types.h"
#include "../sdk/vector.h"
#include "../process.h"
#include "fbase.h"
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
        void DoTrigger();
        bool GetBonePosition(uintptr_t ePtr, int bone, Vector *out);
};
