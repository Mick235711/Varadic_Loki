//
// Created by lee on 17/7/4.
//

#ifndef SMARTPTR_STORAGE_H
#define SMARTPTR_STORAGE_H

#include <functional>

// DefaultSPStorage
// Default Storage Policy
template<typename T>
class DefaultSPStorage
{
public:
    typedef T* StoredType;    // the type of m_pointee
    typedef T* PointerType;   // type returned by operator->
    typedef T& ReferenceType; // type returned by operator* (unary)
    
    DefaultSPStorage() : m_pointee(Default()) {}
    
    // Don't initialize pointer, that is OwnershipPolicy::Clone()
    DefaultSPStorage(const DefaultSPStorage&) {}
    
    template<typename U>
    DefaultSPStorage(const DefaultSPStorage<U>&) {}
    
    DefaultSPStorage(const StoredType& p) : m_pointee(p) {}
    
    PointerType operator->() const {return m_pointee;}
    
    ReferenceType operator*() const {return *m_pointee;}
    
    void swap(DefaultSPStorage& rhs)
    {
        using std::swap;
        swap(m_pointee, rhs.m_pointee);
    }
    
    // Accessors
    friend inline PointerType GetImpl(const DefaultSPStorage& sp)
    {return sp.m_pointee;}
    
    friend inline const StoredType& GetImplRef(const DefaultSPStorage& sp)
    {return sp.m_pointee;}
    
    friend inline StoredType& GetImplRef(DefaultSPStorage& sp)
    {return sp.m_pointee;}

protected:
    // Destroys the data stored
    // (OwnershipPolicy can take over that)
    void Destroy() {delete m_pointee;}
    
    // Default value to initialize the pointer
    static StoredType Default() {return nullptr;}

private:
    // Data
    StoredType m_pointee;
};

// ArraySPStorage
// Array Storage Policy
template<typename T>
class ArraySPStorage
{
public:
    typedef T* StoredType;    // the type of m_pointee
    typedef T* PointerType;   // type returned by operator->
    typedef T& ReferenceType; // type returned by operator* (unary)
    
    ArraySPStorage() : m_pointee(Default()) {}
    
    // Don't initialize pointer, that is OwnershipPolicy::Clone()
    ArraySPStorage(const ArraySPStorage&) {}
    
    template<typename U>
    ArraySPStorage(const ArraySPStorage<U>&) {}
    
    ArraySPStorage(const StoredType& p) : m_pointee(p) {}
    
    PointerType operator->() const {return m_pointee;}
    
    ReferenceType operator*() const {return *m_pointee;}
    
    void swap(ArraySPStorage& rhs)
    {
        using std::swap;
        swap(m_pointee, rhs.m_pointee);
    }
    
    // Accessors
    friend inline PointerType GetImpl(const ArraySPStorage& sp)
    {return sp.m_pointee;}
    
    friend inline const StoredType& GetImplRef(const ArraySPStorage& sp)
    {return sp.m_pointee;}
    
    friend inline StoredType& GetImplRef(ArraySPStorage& sp)
    {return sp.m_pointee;}

protected:
    // Destroys the data stored
    // (OwnershipPolicy can take over that)
    void Destroy() {delete[] m_pointee;}
    
    // Default value to initialize the pointer
    static StoredType Default() {return nullptr;}

private:
    // Data
    StoredType m_pointee;
};

#endif //SMARTPTR_STORAGE_H
