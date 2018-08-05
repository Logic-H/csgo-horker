#include "engine.h"

#include "offsets.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

static Display *internDpy = nullptr;

Engine &Engine::GetInstance()
{
    internDpy = XOpenDisplay(NULL);
    if (internDpy == nullptr) {
        throw std::runtime_error("Failed to open display");
    }
    static Engine instance;
    return instance;
}

Engine::~Engine() {
    if (internDpy) {
        XCloseDisplay(internDpy);
    }
}

int Engine::StringToKeycode(std::string keyString)
{
    KeySym ks = XStringToKeysym(keyString.data());
    return XKeysymToKeycode(internDpy, ks);
}

bool Engine::IsKeyDown(int keyCode)
{
    char keys[32];
    XQueryKeymap(internDpy, keys);
    if (keys[keyCode/8] & (1<<(keyCode%8))) {
        return true;
    }
    return false;
}

void Engine::SetProcessManager(Process *proc)
{
    m_proc = proc;
}

void Engine::Update(bool force)
{
    if (m_proc == nullptr) {
        return;
    }
    m_updateTick++;

    if (force || m_updateTick >= 20) {
        m_updateTick = 0;
        m_proc->Read(Offset::Client::EntityList, m_entitylist);
        m_proc->Read(Offset::Client::LocalPlayer, m_localplayer);
    }
}

CGlowObjectManager *Engine::GetGlowObjectManager()
{
    if (!m_proc->Read(Offset::Client::GlowObjectManager, m_glowmanager)) {
        throw std::runtime_error("Failed to get GlowObjectManager");
    }
    return &m_glowmanager;
}

inline void *Engine::GetEntityPtrById(int id)
{
    return m_entitylist.LookupEntityById(id);
}

CBaseEntity Engine::GetEntityById(int id)
{
    void *ptr = this->GetEntityPtrById(id);
    CBaseEntity ent;
    if (ptr == nullptr || !m_proc->Read(ptr, ent)) {
        throw std::runtime_error("Failed to get Entity class by ID");
    }
    return ent;
}

uintptr_t Engine::GetLocalPlayer()
{
    uintptr_t localPlayer = 0;
    if (!m_proc->Read(Offset::Client::LocalPlayer, localPlayer)) {
        throw std::runtime_error("Failed to get local player address");
    }
    return localPlayer;
}

int Engine::GetLocalPlayerTeam()
{
    return this->GetLocalPlayerVariable<int>(OFF_TEAM);
}

const CBaseEntityList &Engine::GetEntityList() const
{
    return m_entitylist;
}

void Engine::ForceAttack(bool enabled)
{
    if (!m_proc->Write(Offset::Client::ForceAttack, enabled)) {
        throw std::runtime_error("Failed to set ForceAttack value.");
    }
}
