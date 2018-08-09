#pragma once

#include <cstdint>

template <typename T>
class CUtlVector {
    public:
        size_t Count() { return m_nMaxSize; }
        uintptr_t Data() { return m_pData; }
    private:
    uintptr_t m_pData;
    uint32_t m_nMaxSize;
    uint32_t unk1;
    uint32_t m_nCount;
    uint32_t m_pDataPtrBack;
};

