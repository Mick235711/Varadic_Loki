//
// Created by lee on 17/7/3.
//

#ifndef SMALLOBJ_SMALLOBJECT_H
#define SMALLOBJ_SMALLOBJECT_H

#include <cstddef>
#include "Threading/Threading.h"
#include "FixedAllocator.h"
#include "SmallObjAllocator.h"
#include "Singleton/SingletonHolder.h"

template
<
    template<class> class ThreadingModel = DEFAULT_THREADING,
    std::size_t chunkSize = DEFAULT_CHUNK_SIZE,
    std::size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE
>
class SmallObject : public ThreadingModel<SmallObject<ThreadingModel, chunkSize, maxSmallObjectSize>>
{
    typedef ThreadingModel<SmallObject> MyThreadingModel;
    
    struct MySmallObjAllocator : public SmallObjAllocator
    {
        MySmallObjAllocator() : SmallObjAllocator(chunkSize, maxSmallObjectSize) {}
    };

public:
    static void* operator new(std::size_t size)
    {
#if (MAX_SMALL_OBJECT_SIZE != 0) && (DEFAULT_CHUNK_SIZE != 0)
        typename MyThreadingModel::Lock lock;
        (void)lock;
        
        return SingletonHolder<MySmallObjAllocator, CreateStatic, PhoenixSingleton>::Instance().Allocate(size);
#else
        return ::operator new(size);
#endif
    }
    static void operator delete(void* p, std::size_t size)
    {
#if (MAX_SMALL_OBJECT_SIZE != 0) && (DEFAULT_CHUNK_SIZE != 0)
        typename MyThreadingModel::Lock lock;
        (void)lock;
    
        SingletonHolder<MySmallObjAllocator, CreateStatic, PhoenixSingleton>::Instance().Deallocate(p, size);
#else
        ::operator delete(p);
#endif
    }
    virtual ~SmallObject() = default;
};


#endif //SMALLOBJ_SMALLOBJECT_H
