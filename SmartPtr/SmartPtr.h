//
// Created by lee on 17/7/4.
//

#ifndef SMARTPTR_SMARTPTR_H
#define SMARTPTR_SMARTPTR_H

#include "Ownership.h"
#include "Conversion.h"
#include "Checking.h"
#include "Storage.h"
#include "../TypeList/TypeManip.h"

// ByRef
// Helper for Colvin/Gibbons trick for SmartPtr
template<typename T>
class ByRef
{
public:
    ByRef(T& v) : m_value(v) {}
    operator T&() {return m_value;}
    
    ByRef& operator=(const ByRef&) = delete;

private:
    T& m_value;
};

// SmartPtr (declaration)
// Smart Pointer
template
    <
        typename T,
        template<class> class OwnershipPolicy  = RefCounted,
        typename              ConversionPolicy = ExplicitConversion,
        template<class> class CheckingPolicy   = AssertCheck,
        template<class> class StoragePolicy    = DefaultSPStorage
    >
class SmartPtr;

// SmartPtrDef
// unify the usage of SmartPtr
template
    <
        typename T,
        template<class> class OwnershipPolicy  = RefCounted,
        typename              ConversionPolicy = ExplicitConversion,
        template<class> class CheckingPolicy   = AssertCheck,
        template<class> class StoragePolicy    = DefaultSPStorage
    >
class SmartPtrDef
{
    typedef SmartPtr<T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy> type;
};

// SmartPtr (definition)
// Smart Pointer
template
    <
        typename T,
        template<class> class OwnershipPolicy,
        class                 ConversionPolicy,
        template<class> class CheckingPolicy,
        template<class> class StoragePolicy
    >
class SmartPtr
    : public StoragePolicy<T>,
      public OwnershipPolicy<typename StoragePolicy<T>::PointerType>,
      public CheckingPolicy<typename StoragePolicy<T>::StoredType>,
      public ConversionPolicy
{
    typedef StoragePolicy<T> SP;
    typedef OwnershipPolicy<typename SP::PointerType> OP;
    typedef CheckingPolicy<typename SP::StoredType> KP;
    typedef ConversionPolicy CP;
    
    typedef typename Select<OP::destructiveCopy, SmartPtr, const SmartPtr>::type CopyArg;

public:
    typedef typename SP::PointerType PointerType;
    typedef typename SP::StoredType StoredType;
    typedef typename SP::ReferenceType ReferenceType;

private:
    struct NeverMatched;
    
    typedef const StoredType& ExplicitArg;
    typedef typename Select<false, const StoredType&, NeverMatched>::type ImplicitArg;

public:
    SmartPtr() {KP::OnDefault(GetImpl(*this));}
    
    explicit
    SmartPtr(ExplicitArg p) : SP(p) {KP::OnInit(GetImpl(*this));}
    
    SmartPtr(ImplicitArg p) : SP(p) {KP::OnInit(GetImpl(*this));}
    
    SmartPtr(CopyArg& rhs) : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
    {GetImplRef(*this) = OP::Clone(GetImplRef(rhs));}
    
    template
        <
            typename T1,
            template<class> class OP1,
            typename CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    SmartPtr(const SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
    {GetImplRef(*this) = OP::Clone(GetImplRef(rhs));}
    
    template
        <
            typename T1,
            template<class> class OP1,
            typename CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    SmartPtr(SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
    {GetImplRef(*this) = OP::Clone(GetImplRef(rhs));}
    
    SmartPtr(ByRef<SmartPtr> rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
    {}
    
    operator ByRef<SmartPtr>()
    {return ByRef<SmartPtr>(*this);}
    
    SmartPtr& operator=(CopyArg& rhs)
    {
        SmartPtr temp(rhs);
        temp.swap(*this);
        return *this;
    }
    
    template
        <
            typename T1,
            template<class> class OP1,
            class CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    SmartPtr& operator=(const SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
    {
        SmartPtr temp(rhs);
        temp.swap(*this);
        return *this;
    }
    
    template
        <
            typename T1,
            template<class> class OP1,
            class CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    SmartPtr& operator=(SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs)
    {
        SmartPtr temp(rhs);
        temp.swap(*this);
        return *this;
    }
    
    void swap(SmartPtr& rhs)
    {
        OP::swap(rhs);
        CP::swap(rhs);
        KP::swap(rhs);
        SP::swap(rhs);
    }
    
    ~SmartPtr()
    {
        if (OP::Release(GetImpl(*static_cast<SP*>(this))))
        {
            SP::Destroy();
        }
    }
    
    friend inline void Release(SmartPtr& sp, StoredType& p)
    {
        p = GetImplRef(sp);
        GetImplRef(sp) = SP::Default();
    }
    
    friend inline void Reset(SmartPtr& sp, StoredType p)
    {SmartPtr(p).swap(sp);}
    
    PointerType operator->()
    {
        KP::OnDereference(GetImplRef(*this));
        return SP::operator->();
    }
    
    PointerType operator->() const
    {
        KP::OnDereference(GetImplRef(*this));
        return SP::operator->();
    }
    
    PointerType operator*()
    {
        KP::OnDereference(GetImplRef(*this));
        return SP::operator*();
    }
    
    PointerType operator*() const
    {
        KP::OnDereference(GetImplRef(*this));
        return SP::operator*();
    }
    
    bool operator!() const // Enables "if (!sp) ..."
    {return GetImpl(*this) == 0;}
    
    // Ambiguity buster
    template
        <
            typename T1,
            template<class> class OP1,
            class CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    bool operator==(const SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs) const
    {return GetImpl(*this) == GetImpl(rhs);}
    
    // Ambiguity buster
    template
        <
            typename T1,
            template<class> class OP1,
            class CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    bool operator!=(const SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs) const
    {return !(*this == rhs);}
    
    // Ambiguity buster
    template
        <
            typename T1,
            template<class> class OP1,
            class CP1,
            template<class> class KP1,
            template<class> class SP1
        >
    bool operator<(const SmartPtr<T1, OP1, CP1, KP1, SP1>& rhs) const
    {return GetImpl(*this) < GetImpl(rhs);}

private:
    // Helper for enabling 'if (sp)'
    struct Tester
    {
        Tester(int) {}
        void dummy() {}
    };
    struct NotAnInt {};
    
    typedef void (Tester::*unspecified_boolean_type_)();
    
    typedef typename Select<CP::allow && !CP::explicit_conv, Tester, unspecified_boolean_type_>::type unspecified_boolean_type;
    
    typedef typename Select<!CP::explicit_conv, NotAnInt, bool>::type exp_unspecified_boolean_type;
    
public:
    // enable 'if (sp)'
    operator unspecified_boolean_type() const
    {return !*this ? 0 : &Tester::dummy;}
    
    explicit operator exp_unspecified_boolean_type() const
    {return (!*this) == 0;}

private:
    // Helper for disallowing automatic conversion
    struct Insipid
    {
        Insipid(PointerType) {}
    };
    
    typedef typename Select<CP::allow, PointerType, Insipid>::type AutomaticConversionResult;
    
    typedef typename Select<CP::explicit_conv, PointerType, Insipid>::type ExplicitAutomaticConversionResult;
    
public:
    operator AutomaticConversionResult() const
    {return GetImpl(*this);}
    
    explicit operator ExplicitAutomaticConversionResult() const
    {return GetImpl(*this);}
};

// free comparison operators for SmartPtr
template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator==(const SmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs)
{return GetImpl(lhs) == rhs;}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator==(U* lhs, const SmartPtr<T, OP, CP, KP, SP>& rhs)
{return lhs == GetImpl(rhs);}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator!=(const SmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs)
{return !(lhs == rhs);}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator!=(U* lhs, const SmartPtr<T, OP, CP, KP, SP>& rhs)
{return rhs != lhs;}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator<(const SmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs); // not defined

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator<(U* lhs, const SmartPtr<T, OP, CP, KP, SP>& rhs); // not defined

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator>(const SmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs)
{return rhs < lhs;}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator>(U* lhs, const SmartPtr<T, OP, CP, KP, SP>& rhs)
{return lhs < rhs;}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator<=(const SmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs)
{return !(rhs < lhs);}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator<=(U* lhs, const SmartPtr<T, OP, CP, KP, SP>& rhs)
{return !(lhs < rhs);}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator>=(const SmartPtr<T, OP, CP, KP, SP>& lhs, U* rhs)
{return !(lhs < rhs);}

template
    <
        typename T,
        template<class> class OP,
        typename CP,
        template<class> class KP,
        template<class> class SP,
        typename U
    >
inline bool operator>=(U* rhs, const SmartPtr<T, OP, CP, KP, SP>& lhs)
{return !(lhs < rhs);}

// std::less specialization
namespace std
{
    template
        <
            typename T,
            template<class> class OP,
            typename CP,
            template<class> class KP,
            template<class> class SP
        >
    struct less<SmartPtr<T, OP, CP, KP, SP>>
        : public binary_function<SmartPtr<T, OP, CP, KP, SP>, SmartPtr<T, OP, CP, KP, SP>, bool>
    {
        bool operator()(const SmartPtr<T, OP, CP, KP, SP>& lhs, const SmartPtr<T, OP, CP, KP, SP>& rhs) const
        {return less<T*>()(GetImpl(lhs), GetImpl(rhs));}
    };
} // namespace std

#endif //SMARTPTR_SMARTPTR_H
