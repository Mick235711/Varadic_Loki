//
// Created by lee on 17/7/3.
//

#ifndef SMALLOBJ_SMALLOBJALLOCATOR_H
#define SMALLOBJ_SMALLOBJALLOCATOR_H

#include <cstddef>
#include <vector>
#include "FixedAllocator.h"

// SmallObjAllocator
// Offer services for allocating small-sized objects
class SmallObjAllocator
{
public:
    SmallObjAllocator(std::size_t, std::size_t = MAX_SMALL_OBJECT_SIZE);
    
    void* Allocate(std::size_t);
    void Deallocate(void*, std::size_t);
    
    SmallObjAllocator(const SmallObjAllocator&) = delete;
    SmallObjAllocator& operator=(const SmallObjAllocator&) = delete;

private:
    typedef std::vector<FixedAllocator> Pool;
    Pool m_pool;
    FixedAllocator* m_pLastAlloc;
    FixedAllocator* m_pLastDealloc;
    std::size_t m_chunkSize;
    std::size_t m_maxObjectSize;
};


#endif //SMALLOBJ_SMALLOBJALLOCATOR_H
