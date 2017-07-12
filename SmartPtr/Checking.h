//
// Created by lee on 17/7/4.
//

#ifndef SMARTPTR_CHECKING_H
#define SMARTPTR_CHECKING_H

#include <cassert>
#include <stdexcept>

// NoCheck
// no checking
template<typename P>
struct NoCheck
{
    NoCheck() = default;
    
    template<typename P1>
    NoCheck(const NoCheck<P1>&) {}
    
    static void OnDefault(const P&) {}
    
    static void OnInit(const P&) {}
    
    static void OnDereference(const P&) {}
    
    static void swap(NoCheck&) {}
};

// AssertCheck
// check using assert
template<typename P>
struct AssertCheck
{
    AssertCheck() = default;
    
    template<typename P1>
    AssertCheck(const AssertCheck<P1>&) {}
    
    template<typename P1>
    AssertCheck(const NoCheck<P1>&) {}
    
    static void OnDefault(const P&) {}
    
    static void OnInit(const P&) {}
    
    static void OnDereference(const P& val) {assert(val); (void)val;}
    
    static void swap(AssertCheck&) {}
};

// AssertCheckStrict
// strict check using assert
template<typename P>
struct AssertCheckStrict
{
    AssertCheckStrict() = default;
    
    template<typename P1>
    AssertCheckStrict(const AssertCheckStrict<P1>&) {}
    
    template<typename P1>
    AssertCheckStrict(const AssertCheck<P1>&) {}
    
    template<typename P1>
    AssertCheckStrict(const NoCheck<P1>&) {}
    
    static void OnDefault(const P& val) {assert(val); (void)val;}
    
    static void OnInit(const P& val) {assert(val); (void)val;}
    
    static void OnDereference(const P& val) {assert(val); (void)val;}
    
    static void swap(AssertCheckStrict&) {}
};

// NullPointerException
// exception of dereference of nullptr
struct NullPointerException : public std::runtime_error
{
    NullPointerException() : std::runtime_error("") {}
    
    const char* what() noexcept
    {return "Null Pointer Dereference";}
};

// RejectNullStatic
// check upon initialization and before dereference
template<typename P>
struct RejectNullStatic
{
    RejectNullStatic() = default;
    
    template<typename P1>
    RejectNullStatic(const RejectNullStatic&) {}
    
    template<typename P1>
    RejectNullStatic(const AssertCheckStrict<P1>&) {}
    
    template<typename P1>
    RejectNullStatic(const AssertCheck<P1>&) {}
    
    template<typename P1>
    RejectNullStatic(const NoCheck<P1>&) {}
    
    static void OnDefault(const P& val)
    {
        // Make it depended on template parameter
        constexpr bool DependedFalse = sizeof(P*) == 0;
    
        static_assert(DependedFalse, "This policy does not allow default initialization");
    }
    
    static void OnInit(const P& val) {if (!val) throw NullPointerException();}
    
    static void OnDereference(const P& val) {if (!val) throw NullPointerException();}
    
    static void swap(RejectNullStatic&) {}
};

// RejectNull
// check before dereference
template<typename P>
struct RejectNull
{
    RejectNull() = default;
    
    template<typename P1>
    RejectNull(const RejectNull<P1>&) {}
    
    static void OnDefault(const P& val) {OnInit(val);}
    
    static void OnInit(const P& val) {if (!val) throw NullPointerException();}
    
    static void OnDereference(const P& val) {OnInit(val);}
    
    static void swap(RejectNull&) {}
};

// RejectNullStrict
// check before dereference and upon initialization
template<typename P>
struct RejectNullStrict
{
    RejectNullStrict() = default;
    
    template<typename P1>
    RejectNullStrict(const RejectNullStrict<P1>&) {}
    
    template<typename P1>
    RejectNullStrict(const RejectNull<P1>&) {}
    
    //static void OnDefault(const P& val) {OnInit(val);}
    
    static void OnInit(const P& val) {if (!val) throw NullPointerException();}
    
    static void OnDereference(const P& val) {OnInit(val);}
    
    static void swap(RejectNullStrict&) {}
};

#endif //SMARTPTR_CHECKING_H
