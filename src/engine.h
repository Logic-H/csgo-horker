#pragma once

#include "sdk/cbaseentity.h"
#include "sdk/centitylist.h"
#include "sdk/cglowobjectmanager.h"
#include "process.h"

#include <iostream>

class Engine final {
    public:
        static Engine &GetInstance();
        void Update(bool force = false);
        void SetProcessManager(Process *proc);

        // User Input
        bool IsKeyDown(int keyCode);
        static int StringToKeycode(std::string keyString);

        // Glow
        CGlowObjectManager *GetGlowObjectManager();

        // Entity List
        const CBaseEntityList &GetEntityList() const;
        void *GetEntityPtrById(int id);
        CBaseEntity GetEntityById(int id);

        // Local Player
        template <typename T>
        inline T GetLocalPlayerVariable(uintptr_t offset) {
            T retValue;
            if (!m_proc->Read(m_localplayer + offset, &retValue)) {
                std::string eMsg = "[Engine/LocalPlayer] Failed to read offset " + std::to_string(offset);
                throw std::runtime_error(eMsg);
            }
            return retValue;
        }

        template <typename T>
        void SetLocalPlayerVariable(uintptr_t offset, T value) {
            if (!m_proc->Write<T>(m_localplayer + offset, value)) {
                std::string eMsg = "[Engine/LocalPlayer] Failed to write offset" + std::to_string(offset);
                throw std::runtime_error(eMsg);
            }
        }

        uintptr_t GetLocalPlayer();
        CBaseEntity GetLocalPlayerEntity();
        int GetLocalPlayerTeam();

        // Useful things
        void ForceAttack(bool enabled);
    private:
        Engine() = default;
        ~Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
        void UpdateEntityList();

        Process *m_proc = nullptr;
        CBaseEntityList m_entitylist;
        size_t m_updateTick = 0;

        CGlowObjectManager m_glowmanager;
        uintptr_t m_localplayer;
};
