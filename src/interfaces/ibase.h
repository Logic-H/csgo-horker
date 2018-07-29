#ifndef  __IBASE_H__
#define  __IBASE_H__

#include "../memorymanager.h"

class IBase {
    public:
        IBase(MemoryManager &mem) : m_mem(mem) {};
        virtual ~IBase() {};
        IBase(const IBase&) = delete;
        void operator=(const IBase&) = delete;
    protected:
        MemoryManager &m_mem;
};
#endif //__CHEATBASE_H__
