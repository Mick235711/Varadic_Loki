//
// Created by lee on 17/7/3.
//

#ifndef SINGLETON_LONGEVITY_H
#define SINGLETON_LONGEVITY_H

#include <cstdlib>
#include <new>
#include <algorithm>

typedef void (*atexit_pfn_t)();

namespace detail
{
    // LifetimeTracker
    // Helper class for SetLongevity
    class LifetimeTracker
    {
        unsigned int m_logevity;
        
    public:
        LifetimeTracker(unsigned int l) : m_logevity(l) {}
        
        virtual ~LifetimeTracker() = 0;
        
        static bool Compare(const LifetimeTracker* lhs, const LifetimeTracker* rhs)
        {
            return lhs->m_logevity < rhs->m_logevity;
        }
    };
    
    // Definition
    inline LifetimeTracker::~LifetimeTracker() {}
    
    // Helper data
    typedef LifetimeTracker** TrackerArray;
    extern TrackerArray pTrackerArray;
    extern unsigned int elements;
    
    // Deleter
    // Helper destroyer functor
    template<typename T>
    struct Deleter
    {
        static void Delete(T* pObj)
        {
            delete pObj;
        }
    };
    
    // ConcreteLifetimeTracker
    // Concrete Lifetime Tracker for objects of type T
    template<typename T, typename Destroyer>
    class ConcreteLifetimeTracker : public LifetimeTracker
    {
        T* m_pTracked;
        Destroyer m_destroyer;

    public:
        ConcreteLifetimeTracker(T* p, unsigned int longevity, Destroyer d)
            : LifetimeTracker(longevity),
              m_pTracked(p),
              m_destroyer(d)
        {}
        
        ~ConcreteLifetimeTracker()
        {
            m_destroyer(m_pTracked);
        }
    };
    
    void AtExitFn(); // definition below
} // namespace detail

// SetLongevity
// Assigns an object an longevity; ensures ordered destructions of objects registered
// destroyed during exit
template<typename T, typename Destroyer = void (*)(T*)>
void SetLongevity(T* pDynObject, unsigned int longevity, Destroyer d = &detail::Deleter<T>::Delete)
{
    using namespace detail;
    
    TrackerArray pNewArray = static_cast<TrackerArray>(
        std::realloc(pTrackerArray, sizeof(*pTrackerArray) * (elements + 1))
    );
    if (!pNewArray) throw std::bad_alloc();
    
    // Delayed assignment for exception safety
    pTrackerArray = pNewArray;
    
    LifetimeTracker* p = new ConcreteLifetimeTracker<T, Destroyer>(pDynObject, longevity, d);
    
    // Insert a pointer to the object into the queue
    TrackerArray pos = std::upper_bound(pTrackerArray, pTrackerArray + elements, p, LifetimeTracker::Compare);
    std::copy_backward(pos, pTrackerArray + elements, pTrackerArray + elements + 1);
    *pos = p;
    ++elements;
    
    // Register a call to AtExitFn
    std::atexit(AtExitFn);
}

#endif //SINGLETON_LONGEVITY_H
