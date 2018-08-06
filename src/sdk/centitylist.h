#pragma once

#include <cstdint>
#include <unordered_map>

class CEntInfo {
    public:
        void *m_pEntity;
        long m_SerialNumber;
        CEntInfo *m_pPrev;
        CEntInfo *m_pNext;
};

class CBaseEntityList {
    public:
        void AddEntInfo(int index, CEntInfo einfo);
        void *GetEntityPtrById(int index);
        void Reset();
        size_t Count();
    private:
        std::unordered_map<int, CEntInfo> m_entinfo;
};
