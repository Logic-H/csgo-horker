#include "centitylist.h"

void *CBaseEntityList::LookupEntityById(int id) const
{
    return m_EntPtrArray[id].m_pEntity;
}

