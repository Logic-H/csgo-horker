#pragma once

#include <string>

bool ReadConfig(const std::string &configFile);

namespace Config {
    namespace AimBot {
        extern bool        AimAssist;
        extern float       AimCorrection;
        extern float       AimFieldOfView;
        extern float       AimSpeed;
        extern int         TargetMode;
        extern int         TargetBone;
        extern bool        Trigger;
        extern int         TriggerDelay;
        extern bool        UseTriggerKey;
        extern bool        RecoilControl;
        extern std::string TriggerKey;
        extern bool        UseMouseEvents;
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

