#include "config.h"

#include <fstream>
#include <iostream>

#include "external/inih/INIReader.h"

constexpr char defConfigFile[] = "config.ini";

bool  Config::Glow::Enabled   = true;
bool  Config::Glow::LegitGlow = false;
bool  Config::Glow::GlowAllies = true;
bool  Config::Glow::GlowEnemies = true;
bool  Config::Glow::GlowOther   = false;
bool  Config::Glow::GlowWeapons = false;
float Config::Glow::EnemyR    = 0.8f;
float Config::Glow::EnemyG    = 0.8f;
float Config::Glow::EnemyB    = 0.0f;
float Config::Glow::EnemyA    = 0.8f;

float Config::Glow::AllyR     = 0.0f;
float Config::Glow::AllyG     = 0.8f;
float Config::Glow::AllyB     = 0.8f;
float Config::Glow::AllyA     = 0.8f;

bool  Config::Visual::NoFlash = true;

bool  Config::AimBot::AimAssist = true;
float Config::AimBot::AimCorrection = 0.4f;
float Config::AimBot::AimFieldOfView = 35.2f;
float Config::AimBot::AimSpeed = 5.0f;
int   Config::AimBot::TargetMode = 1;
int   Config::AimBot::TargetBone = 8;
bool  Config::AimBot::Trigger = true;
int   Config::AimBot::TriggerDelay = 50;
bool  Config::AimBot::UseTriggerKey = true;
std::string Config::AimBot::TriggerKey = "F";

#define WriteSection(key) \
    conf << "[" #key "]" << "\n";
#define WritePair(section, key) \
    conf << #key " = " << Config::section::key << "\n";
#define WriteSectionEnd() conf << "\n";
#define WriteComment(msg) conf << "; " << msg << '\n';

void UpdateConfig()
{
    std::ofstream conf(defConfigFile);
    if (conf.is_open()) {
        WriteSection(AimBot);
        WriteComment("Enable Aim Assist: 1/0");
        WritePair(AimBot, AimAssist);
        WriteComment("Aim correction (Higher values reduce jitter w/ high mouse sensitivity)");
        WritePair(AimBot, AimCorrection);
        WriteComment("Aim field of view (float) - Adjustments for TargetMode = 1");
        WritePair(AimBot, AimFieldOfView);
        WriteComment("Aim smoothing as a floating point (lower looks more legit)");
        WritePair(AimBot, AimSpeed);
        WriteComment("TargetMode Valid List:");
        WriteComment("-- Nearest: 0");
        WriteComment("-- FOV: 1");
        WritePair(AimBot, TargetMode);

        WriteComment("TargetBone Valid List:");
        WriteComment("-- Head = 8");
        WriteComment("-- Neck = 7");
        WriteComment("-- Torso = 3, 4, 5, or 6");
        WriteComment("-- Pelvis = 2");

        WritePair(AimBot, TargetBone);
        WriteComment("Enable auto-trigger: 1/0");
        WritePair(AimBot, Trigger);
        WriteComment("Delay between shots in milliseconds");
        WritePair(AimBot, TriggerDelay);
        WriteComment("Use trigger key for Aim/Trigger (Recommended on)");
        WritePair(AimBot, UseTriggerKey);
        WriteComment("Keyboard key to use for Aim/Trigger (Default is F)");
        WritePair(AimBot, TriggerKey);
        WriteSectionEnd();

        WriteSection(Glow);
        WritePair(Glow, Enabled);
        WritePair(Glow, LegitGlow);
        WritePair(Glow, GlowAllies);
        WritePair(Glow, GlowEnemies);
        WritePair(Glow, GlowOther);
        WritePair(Glow, GlowWeapons);
        WritePair(Glow, EnemyR);
        WritePair(Glow, EnemyG);
        WritePair(Glow, EnemyB);
        WritePair(Glow, EnemyA);
        WritePair(Glow, AllyR);
        WritePair(Glow, AllyG);
        WritePair(Glow, AllyB);
        WritePair(Glow, AllyA);
        WriteSectionEnd();

        WriteSection(Visual);
        WritePair(Visual, NoFlash);
        conf.close();
    }
}

#define RCBOOL(section, key) \
    Config::section::key = reader.GetBoolean(#section, #key, Config::section::key);
#define RCSTR(section, key) \
    Config::section::key = reader.Get(#section, #key, Config::section::key);
#define RCDBL(section, key) \
    Config::section::key = reader.GetReal(#section, #key, Config::section::key);
#define RCINT(section, key) \
    Config::section::key = reader.GetInteger(#section, #key, Config::section::key);
bool ReadConfig(const std::string &configFile)
{
    INIReader reader(configFile);

    if (reader.ParseError() < 0) {
        UpdateConfig();
        return false;
    }
    
    RCBOOL(AimBot, AimAssist);
    RCDBL(AimBot, AimCorrection);
    RCDBL(AimBot, AimFieldOfView);
    RCDBL(AimBot, AimSpeed);
    RCINT(AimBot, TargetMode);
    RCINT(AimBot, TargetBone);
    RCBOOL(AimBot, Trigger);
    RCINT(AimBot, TriggerDelay);
    RCSTR (AimBot, TriggerKey);
    RCBOOL(AimBot, UseTriggerKey);

    RCBOOL(Glow, Enabled);
    RCBOOL(Glow, LegitGlow);
    RCBOOL(Glow, GlowAllies);
    RCBOOL(Glow, GlowEnemies);
    RCBOOL(Glow, GlowOther);
    RCBOOL(Glow, GlowWeapons);
    RCDBL (Glow, EnemyR);
    RCDBL (Glow, EnemyG);
    RCDBL (Glow, EnemyB);
    RCDBL (Glow, EnemyA);
    RCDBL (Glow, AllyR);
    RCDBL (Glow, AllyG);
    RCDBL (Glow, AllyB);
    RCDBL (Glow, AllyA);

    RCBOOL(Visual, NoFlash);
    UpdateConfig();
    return true;
}
