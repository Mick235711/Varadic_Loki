//
// Created by lee on 17/7/3.
//

#ifndef SINGLETON_CREATE_H
#define SINGLETON_CREATE_H

#include <cstdlib>

// CreateUsingNew
// Creates objects using a straight call to the new operator
template<typename T>
struct CreateUsingNew
{
    static T* Create()
    {return new T;}
    
    static void Destroy(T* p)
    {delete p;}
};

// CreateUsingMalloc
// Creates objects using a call to std::malloc and the placement new operator
template<typename T>
struct CreateUsingMalloc
{
    static T* Create()
    {
        void* p = std::malloc(sizeof(T));
        if (!p) return nullptr;
        return new(p) T;
    }
    
    static void Destroy(T* p)
    {
        p->~T();
        std::free(p);
    }
};

// CreateStatic
// Creates objects in static memory
template<typename T>
struct CreateStatic
{
    // MaxAlign trick
    union MaxAlign
    {
        char m_t[sizeof(T)];
        short int m_shortInt;
        int m_int;
        long int m_longInt;
        float m_float;
        double m_double;
        long double m_longDouble;
        struct Test;
        int Test::* m_pMember;
        int (Test::*m_pMemberFn)(int);
    };
    
    static T* Create()
    {
        static MaxAlign m_staticMemory;
        return new(&m_staticMemory) T;
    }
    
    static void Destroy(T* p)
    {
        p->~T();
    }
};

#endif //SINGLETON_CREATE_H
