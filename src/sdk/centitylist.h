#pragma once

#include <cstdint>
#include <unordered_map>

class CEntInfo {
    public:
        uintptr_t m_pEntity;
        long m_SerialNumber;
        CEntInfo *m_pPrev;
        CEntInfo *m_pNext;
};

class CBaseEntityList {
    public:
        const std::unordered_map<int, CEntInfo> &Data() const { return m_entinfo; }
        void AddEntInfo(int index, CEntInfo einfo);
        uintptr_t GetEntityPtrById(int index);
        void Reset();
        size_t Count();
    private:
        std::unordered_map<int, CEntInfo> m_entinfo;
};
