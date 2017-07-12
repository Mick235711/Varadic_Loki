//
// Created by lee on 17/7/4.
//

#ifndef SMARTPTR_OWNERSHIP_H
#define SMARTPTR_OWNERSHIP_H

#include <cassert>
#include "SmallObj/SmallObject.h"

// RefCounted
// Provides a classic external reference counting implementation of OwnershipPolicy
template<typename P>
class RefCounted
{
public:
    RefCounted()
    {
        m_pCount = static_cast<unsigned int*>(SmallObject<>::operator new(sizeof(unsigned int)));
        assert(m_pCount);
        *m_pCount = 1;
    }
    
    RefCounted(const RefCounted& rhs) : m_pCount(rhs.m_pCount) {}
    
    template<typename P1>
    RefCounted(const RefCounted<P1>& rhs)
        : m_pCount(reinterpret_cast<const RefCounted&>(rhs).m_pCount) {}
    
    P Clone(const P& val)
    {
        ++*m_pCount;
        return val;
    }
    
    bool Release(const P&)
    {
        if (!--*m_pCount)
        {
            SmallObject<>::operator delete(m_pCount, sizeof(unsigned int));
            return true;
        }
        return false;
    }
    
    void swap(RefCounted& rhs)
    {
        using std::swap;
        swap(m_pCount, rhs.m_pCount);
    }
    
    enum {destructiveCopy = false};

private:
    // Data
    unsigned int* m_pCount;
};

// RefCountedMT
// Implements external reference counting for multithreaded programs
// Usage: RefCountedMTAdj<ThreadingModel>::RefCountedMT
template<template<class> class ThreadingModel>
struct RefCountedMTAdj
{
    template<typename P>
    class RefCountedMT : public ThreadingModel<RefCountedMT<P>>
    {
        typedef ThreadingModel<RefCountedMT> base_type;
        typedef typename base_type::IntType             CountType;
        typedef volatile CountType*                     CountPtrType;
    public:
        RefCountedMT()
        {
            m_pCount = static_cast<CountPtrType>(SmallObject<>::operator new(sizeof(unsigned int)));
            assert(m_pCount);
            *m_pCount = 1;
        }
        
        RefCountedMT(const RefCountedMT& rhs) : m_pCount(rhs.m_pCount) {}
        
        template<typename P1>
        RefCountedMT(const RefCounted<P1>& rhs)
            : m_pCount(reinterpret_cast<const RefCountedMT&>(rhs).m_pCount) {}
        
        CountPtrType Clone(const CountPtrType& val)
        {
            base_type::AtomicIncrement(*m_pCount);
            return val;
        }
        
        bool Release(const CountPtrType&)
        {
            if (!base_type::AtomicDecrement(*m_pCount))
            {
                SmallObject<>::operator delete(const_cast<CountType *>(m_pCount), sizeof(unsigned int));
                return true;
            }
            return false;
        }
        
        void swap(RefCountedMT& rhs)
        {
            using std::swap;
            swap(m_pCount, rhs.m_pCount);
        }
        
        enum {destructiveCopy = false};
    
    private:
        // Data
        CountPtrType m_pCount;
    };
};

// DeepCopy
// Implements deep copy semantics, assumes existence of a Clone() member function of type P
// otherwise, define macro CLONE_NAME as your clone function
#ifndef CLONE_NAME
#define CLONE_NAME Clone
#endif // CLONE_NAME

template<typename P>
struct DeepCopy
{
    DeepCopy() = default;
    
    template<typename P1>
    DeepCopy(const DeepCopy<P1>&) {}
    
    static P Clone(const P& val)
    {return val->CLONE_NAME();}
    
    static bool Release(const P&) {return true;}
    
    static void swap(DeepCopy&) {}
    
    enum {destructiveCopy = false};
};

// RefLinked
// Implements reference linking OwnershipPolicy
namespace detail
{
    class RefLinkedBase
    {
    public:
        RefLinkedBase() :m_prev(this), m_next(this) {}
        
        RefLinkedBase(const RefLinkedBase& rhs)
            : m_prev(&rhs), m_next(rhs.m_next)
        {
            m_prev->m_next = this;
            m_next->m_prev = this;
        }
        
        bool Release()
        {
            if (m_next == this)
            {
                assert(m_prev == this);
                return true;
            }
            m_prev->m_next = m_next;
            m_next->m_prev = m_prev;
            return false;
        }
        
        void swap(RefLinkedBase& rhs)
        {
            if (m_next == this)
            {
                assert(m_prev == this);
                if (rhs.m_next == &rhs)
                {
                    assert(rhs.m_prev == &rhs);
                    // both lists are empty, nothing to do
                    return;
                }
                m_prev = rhs.m_prev;
                m_next = rhs.m_next;
                m_prev->m_next = m_next->m_prev = this;
                rhs.m_next = rhs.m_prev = &rhs;
                return;
            }
            if (rhs.m_next == &rhs)
            {
                assert(rhs.m_prev == &rhs);
                rhs.swap(*this);
                return;
            }
            using std::swap;
            swap(m_prev, rhs.m_prev);
            swap(m_next, rhs.m_next);
            swap(m_prev->m_next, rhs.m_prev->m_next);
            swap(m_next->m_prev, rhs.m_next->m_prev);
        }
        
        enum {destructiveCopy = false};

    private:
        mutable const RefLinkedBase* m_prev;
        mutable const RefLinkedBase* m_next;
    };
}

template<typename P>
class RefLinked : public detail::RefLinkedBase
{
public:
    RefLinked() = default;
    
    template<typename P1>
    RefLinked(const RefLinked<P1>& rhs)
        : detail::RefLinkedBase(rhs)
    {}
    
    static P Clone(const P& val)
    {return val;}
    
    bool Release(const P&)
    {return detail::RefLinkedBase::Release();}
};

// DestructiveCopy
// Implements destructive copy semantics (like std::auto_ptr)
template<typename P>
class DestructiveCopy
{
public:
    DestructiveCopy() = default;
    
    template<typename P1>
    DestructiveCopy(const DestructiveCopy<P1>&) {}
    
    template<typename P1>
    static P Clone(P1& val)
    {
        P result(val);
        val = P1();
        return result;
    }
    
    static bool Release(const P&)
    {return true;}
    
    static void swap(DestructiveCopy&) {}
    
    enum {destructiveCopy = true};
};

// NoCopy
// Implements OwnershipPolicy as not allow copy
template<typename P>
class NoCopy
{
public:
    NoCopy() = default;
    
    template<typename P1>
    NoCopy(const NoCopy<P1>&) {}
    
    static P Clone(const P&)
    {
        // Make it depended on template parameter
        constexpr bool DependedFalse = sizeof(P*) == 0;
    
        static_assert(DependedFalse, "This policy disallow value copying");
        
        return P();
    }
    
    static bool Release(const P&)
    {return true;}
    
    static void swap(NoCopy&) {}
    
    enum {destructiveCopy = false};
};

#endif //SMARTPTR_OWNERSHIP_H
