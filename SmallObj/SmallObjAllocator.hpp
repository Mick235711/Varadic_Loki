//
// Created by lee on 17/7/3.
//

#include "SmallObjAllocator.h"
#include <stdexcept>
#include <cassert>

SmallObjAllocator::SmallObjAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
: m_pLastAlloc(nullptr), m_pLastDealloc(nullptr), m_chunkSize(chunkSize), m_maxObjectSize(maxObjectSize)
{
}

namespace // anonymous
{
    struct CompareFixedAllocatorSize : std::binary_function<const FixedAllocator&, std::size_t, bool>
    {
        bool operator()(const FixedAllocator& x, std::size_t numBytes) const
        {
            return x.BlockSize() < numBytes;
        }
    };
} // namespace

void* SmallObjAllocator::Allocate(std::size_t numBytes)
{
    if (numBytes > m_maxObjectSize) return operator new(numBytes);
    
    if (m_pLastAlloc && m_pLastAlloc->BlockSize() == numBytes)
    {
        return m_pLastAlloc->Allocate();
    }
    Pool::iterator i = std::lower_bound(m_pool.begin(), m_pool.end(), numBytes, CompareFixedAllocatorSize());
    if (i == m_pool.end() || i->BlockSize() != numBytes)
    {
        i = m_pool.insert(i, FixedAllocator(numBytes));
        m_pLastDealloc = &*m_pool.begin();
    }
    m_pLastAlloc = &*i;
    return m_pLastAlloc->Allocate();
}

void SmallObjAllocator::Deallocate(void* p, std::size_t numBytes)
{
    if (numBytes > m_maxObjectSize) return operator delete(p);
    
    if (m_pLastDealloc && m_pLastDealloc->BlockSize() == numBytes)
    {
        m_pLastDealloc->Deallocate(p);
        return;
    }
    Pool::iterator i = std::lower_bound(m_pool.begin(), m_pool.end(), numBytes, CompareFixedAllocatorSize());
    assert(i != m_pool.end());
    assert(i->BlockSize() == numBytes);
    m_pLastDealloc = &*i;
    m_pLastDealloc->Deallocate(p);
}
