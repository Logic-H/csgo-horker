#pragma once

#include <cstdint>

#define OFF_TEAM  0x128
#define OFF_FLASHDURATION 0xABE8
#define OFF_INCROSSHAIRID 0xBBD0

#define OFFSET(name) extern uintptr_t name;

namespace Offset {
    namespace Client {
        OFFSET(EntityList);
        OFFSET(ForceAttack);
        OFFSET(LocalPlayer);
        OFFSET(GlowObjectManager);
        OFFSET(PlayerResources);
    };
    namespace Engine {
        OFFSET(IsConnected);
    };
};
