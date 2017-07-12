//
// Created by lee on 17/7/3.
//

#ifndef SINGLETON_LIFETIME_H
#define SINGLETON_LIFETIME_H

#include <stdexcept>
#include "Longevity.h"

// DefaultLifetime
// Schedules an Object's destruction as per C++ rules
// forward to std::atexit
template<typename T>
struct DefaultLifetime
{
    static void ScheduleDestruction(T*, atexit_pfn_t pFun)
    {std::atexit(pFun);}
    
    static void OnDeadReference()
    {throw std::logic_error("Dead Reference Detected");}
};

// PhoenixSingleton
// Schedules an Object's destruction as phoenix (can be recreated)
template<typename T>
struct PhoenixSingleton
{
    static void ScheduleDestruction(T*, atexit_pfn_t pFun)
    {std::atexit(pFun);}
    
    static void OnDeadReference()
    {}
};

namespace detail
{
    // Adapter
    // Helper for SingletonWithLongevity
    template<typename T>
    struct Adapter
    {
        void operator()(T*) {return m_pFun();}
        atexit_pfn_t m_pFun;
    };
} // namespace detail

// SingletonWithLongevity
// Schedules an Object's destruction as logevity
// needs an function [inline unsigned int GetLongevity(T*)] in namespace-level
#define DEFINE_GETLONGEVITY(T, longevity) \
inline unsigned int GetLongevity(T*) {return longevity;}
template<typename T>
struct SingletonWithLongevity
{
    static void ScheduleDestruction(T* pObj, atexit_pfn_t pFun)
    {
        detail::Adapter<T> adapter = {pFun};
        SetLongevity(pObj, GetLongevity(pObj), adapter);
    }
    
    static void OnDeadReference()
    {throw std::logic_error("Dead Reference Detected");}
};

// NoDestroy
// Never destroy, leak memory
template<typename T>
struct NoDestroy
{
    static void ScheduleDestruction(T*, atexit_pfn_t)
    {}
    
    static void OnDeadReference()
    {}
};

#endif //SINGLETON_LIFETIME_H
