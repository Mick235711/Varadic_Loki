//
// Created by lee on 17/6/30.
//

#ifndef THREADING_THREADING_H
#define THREADING_THREADING_H

#include <mutex>
#include <atomic>

// default threading model
#ifndef DEFAULT_THREADING
#define DEFAULT_THREADING SingleThreaded
#endif // DEFAULT_THREADING

// SingleThreaded
// threading model in single thread
template<typename Host>
class SingleThreaded
{
public:
    // lock
    struct Lock
    {
        Lock() = default;
        explicit Lock(const SingleThreaded&) {}
    };
    
    // typedefs
    typedef Host VolatileType;
    typedef int IntType;
    
    // functions
#define DEF_ATOMIC_OPERATION(name,op) static IntType Atomic##name (volatile IntType& lhs, volatile IntType& rhs) \
    {return lhs op rhs;}
    DEF_ATOMIC_OPERATION(Add,+=)
    DEF_ATOMIC_OPERATION(Subtract,-=)
    DEF_ATOMIC_OPERATION(Multiply,*=)
    DEF_ATOMIC_OPERATION(Divide,/=)
#undef DEF_ATOMIC_OPERATION
    static IntType AtomicIncrement(volatile IntType& lval)
    { return ++lval; }
    static IntType AtomicDecrement(volatile IntType& lval)
    { return --lval; }
    static void AtomicAssign(volatile IntType & lval, IntType val)
    { lval = val; }
    static void AtomicAssign(IntType & lval, volatile IntType & val)
    { lval = val; }
};

// ClassLevelLockable
// threading model in multiple thread using std::mutex ans std::atomic
template<typename Host>
class ClassLevelLockable
{
public:
    // lock
    struct Lock
    {
        mutable std::mutex m_mut;
        Lock() = default;
        explicit Lock(const ClassLevelLockable&) {}
        ~Lock() = default;
    };
    
    // typedefs
    typedef volatile Host VolatileType;
    typedef std::atomic<int> IntType;
        
        // functions
#define DEF_ATOMIC_OPERATION(name,op) static int Atomic##name (IntType& lhs, IntType& rhs) \
    { \
        int oldValue = lhs.load(); \
        while (!lhs.compare_exchange_weak(oldValue, oldValue op rhs)) \
        {} \
        return oldValue; \
    }
    DEF_ATOMIC_OPERATION(Add,+)
    DEF_ATOMIC_OPERATION(Subtract,-)
    DEF_ATOMIC_OPERATION(Multiply,*)
    DEF_ATOMIC_OPERATION(Divide,/)
#undef DEF_ATOMIC_OPERATION
    static int AtomicIncrement(IntType& lval)
    { return ++lval; }
    static int AtomicDecrement(IntType& lval)
    { return --lval; }
    static void AtomicAssign(IntType & lval, IntType & val)
    {
        int oldValue = lval.load();
        while (!lval.compare_exchange_weak(oldValue, val))
        {}
    }
};

// ClassLevelLockable
// threading model in multiple thread using std::mutex ans std::atomic
template<typename Host>
class ObjectLevelLockable
{
    mutable std::mutex* m_pmtx;
    
public:
    // lock
    struct Lock;
    friend Lock;
    
    struct Lock
    {
        const ObjectLevelLockable& m_host;
        explicit Lock(const ObjectLevelLockable& host) :m_host(host) {m_host.m_pmtx = new std::mutex();}
        ~Lock() {delete m_host.m_pmtx;}
    };
    
    // typedefs
    typedef volatile Host VolatileType;
    typedef std::atomic<int> IntType;
    
    // functions
#define DEF_ATOMIC_OPERATION(name,op) static int Atomic##name (IntType& lhs, IntType& rhs) \
    { \
        int oldValue = lhs.load(); \
        while (!lhs.compare_exchange_weak(oldValue, oldValue op rhs)) \
        {} \
        return oldValue; \
    }
    DEF_ATOMIC_OPERATION(Add,+)
    DEF_ATOMIC_OPERATION(Subtract,-)
    DEF_ATOMIC_OPERATION(Multiply,*)
    DEF_ATOMIC_OPERATION(Divide,/)
#undef DEF_ATOMIC_OPERATION
    static int AtomicIncrement(IntType& lval)
    { return ++lval; }
    static int AtomicDecrement(IntType& lval)
    { return --lval; }
    static void AtomicAssign(IntType & lval, IntType & val)
    {
        int oldValue = lval.load();
        while (!lval.compare_exchange_weak(oldValue, val))
        {}
    }
};

#endif //THREADING_THREADING_H
