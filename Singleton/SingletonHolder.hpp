//
// Created by lee on 17/7/3.
//

#include "SingletonHolder.h"
#include <stdexcept>

template<class T, template<class> class C, template<class> class L, template<class> class M>
typename SingletonHolder<T, C, L, M>::PtrInstanceType SingletonHolder<T, C, L, M>::m_pInstance;

template<class T, template<class> class C, template<class> class L, template<class> class M>
bool SingletonHolder<T, C, L, M>::m_destroyed;

template<class T, template<class> class C, template<class> class L, template<class> class M>
inline T& SingletonHolder<T, C, L, M>::Instance()
{
    if (!m_pInstance)
    {
        MakeInstance();
    }
    return *m_pInstance;
}

template
    <
        class T,
        template<class> class CreationPolicy,
        template<class> class LifetimePolicy,
        template<class> class ThreadingModel>
void SingletonHolder<T, CreationPolicy, LifetimePolicy, ThreadingModel>::MakeInstance()
{
    typename ThreadingModel<T>::Lock guard;
    (void)guard;
    
    if (!m_pInstance)
    {
        if (m_destroyed)
        {
            LifetimePolicy<T>::OnDeadReference();
            m_destroyed = false;
        }
        m_pInstance = CreationPolicy<T>::Create();
        LifetimePolicy<T>::ScheduleDestruction(m_pInstance, &DestroySingleton);
    }
}

#define Assert(e) do {if (!(e)) throw std::runtime_error("Assert failed: " #e);} while(0)

template<class T, template<class> class CreationPolicy, template<class> class L, template<class> class M>
void SingletonHolder<T, CreationPolicy, L, M>::DestroySingleton()
{
    Assert(!m_destroyed);
    CreationPolicy<T>::Destroy(m_pInstance);
    m_pInstance = nullptr;
    m_destroyed = true;
}
