//
// Created by lee on 17/7/3.
//

#ifndef SINGLETON_SINGLETONHOLDER_H
#define SINGLETON_SINGLETONHOLDER_H

#include "Create.h"
#include "Lifetime.h"
#include "../Threading/Threading.h"

// SingletonHolder
// Provides Singleton amenities for a type T
// for type T:
// 1) hide its ctors, copy-members, dtors, address-of operators, etc.
// 2) make CreationPolicy you choose as it's friend
template
<
    typename T,
    template<class> class CreationPolicy = CreateUsingNew,
    template<class> class LifetimePolicy = DefaultLifetime,
    template<class> class ThreadingModel = DEFAULT_THREADING
>
class SingletonHolder
{
public:
    static T& Instance();

private:
    // Helpers
    static void MakeInstance();
    static void DestroySingleton();

public:
    // Protection
    SingletonHolder() = delete;
    SingletonHolder(const SingletonHolder&) = delete;
    SingletonHolder& operator=(const SingletonHolder&) = delete;

private:
    // Data
    typedef typename ThreadingModel<T*>::VolatileType PtrInstanceType;
    static PtrInstanceType m_pInstance;
    static bool m_destroyed;
};

#endif //SINGLETON_SINGLETONHOLDER_H
