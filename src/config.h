#pragma once

#include <string>

bool ReadConfig(const std::string &configFile);

namespace Config {
    namespace AimBot {
        extern bool        Trigger;
        extern bool        UseTriggerKey;
        extern std::string TriggerKey;
    };

    namespace Glow {
        extern bool  Enabled;
        extern bool  LegitGlow;
        extern bool  GlowAllies;
        extern bool  GlowEnemies;
        extern bool  GlowOther;
        extern bool  GlowWeapons;
        extern float EnemyR;
        extern float EnemyG;
        extern float EnemyB;
        extern float EnemyA;

        extern float AllyR;
        extern float AllyG;
        extern float AllyB;
        extern float AllyA;
    };

    namespace Visual {
        extern bool NoFlash;
    };
};

