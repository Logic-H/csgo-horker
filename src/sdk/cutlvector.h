#pragma once

#include "cutlmemory.h"
template <class T, class A = CUtlMemory<T>> class CUtlVector
{
	typedef A CAllocator;

	public:
		T& operator[](int i)
		{
			return m_Memory[i];
		}

        T* Data()
        {
            return m_pElements;
        }

		int Count() const
		{
			return m_Size;
		}
	protected:
		CAllocator m_Memory;
		int m_Size;
		T* m_pElements;
};

