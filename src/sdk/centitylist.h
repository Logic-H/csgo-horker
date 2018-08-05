#pragma once

#define NUM_ENT_ENTRIES (1<<13)

class CEntInfo {
    public:
        void *m_pEntity;
        long m_SerialNumber;
        CEntInfo *m_pPrev;
        CEntInfo *m_pNext;
};

class CBaseEntityList {
    public:
        void *LookupEntityById(int id) const;
    private:
        class CEntInfoList {
            public:
                const CEntInfo *Head() const { return m_pHead; }
                const CEntInfo *Tail() const { return m_pTail; }
                CEntInfo *Head() { return m_pHead; }
                CEntInfo *Tail() { return m_pTail; }
            private:
                CEntInfo *m_pHead;
                CEntInfo *m_pTail;
        };
        CEntInfo m_EntPtrArray[NUM_ENT_ENTRIES];
        CEntInfoList m_activeList;
        CEntInfoList m_freeNonNetworkableList;
};

