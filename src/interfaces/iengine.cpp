#include "iengine.h"
#include "../patterns.h"

IEngine::IEngine(MemoryManager &mem) : IBase(mem)
{
    constexpr char sMod[] = "engine_client.so";

    // IsConnected Address
    uintptr_t connectedMov = m_mem.FindInModule(sMod,
        PAT_CONNECTEDMOV, PAT_CONNECTEDMOV_OFF);
    m_aConnected = m_mem.GetCallAddress(connectedMov) + 1;
    
    Update();
}

void IEngine::Update()
{
    uint8_t connected = 0;
    if (m_mem.Read(m_aConnected, connected)) {
        m_bConnected = (connected == 1);
    }
}

bool IEngine::IsConnected()
{
    return m_bConnected;
}
