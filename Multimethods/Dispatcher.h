//
// Created by lee on 17/7/10.
//

#ifndef MULTIMETHODS_DISPATCHER_H
#define MULTIMETHODS_DISPATCHER_H

#include <utility>
#include "../TypeList/TypeManip.h"
#include "../TypeList/TypeList.h"
#include "TypeInfo/TypeInfo.hpp"
#include "../AssocVector/AssocVector.h"
#include "Functor/Functor.hpp"
#include "Functor/FunctorHandler.hpp"
#include "Casting.h"

// InvocationTraits
// Helper trait for implementing optional symmetry
namespace detail
{
    template<typename SomeLhs, typename SomeRhs, typename Executor, typename ResultType = void>
    struct InvocationTraits
    {
        static ResultType DoDispatch(SomeLhs& lhs, SomeRhs& rhs, Executor& exec, Int2Type<0>)
        {
            return exec.Fire(lhs, rhs);
        }
        static ResultType DoDispatch(SomeLhs& lhs, SomeRhs& rhs, Executor& exec, Int2Type<1>)
        {
            return exec.Fire(rhs, lhs);
        }
    };
} // namespace detail

// StaticDispatcher
// Implements an automatic static double-dispatcher based on two TypeLists
template
    <
        typename Executor,
        typename BaseLhs,
        typename TypesLhs,
        typename BaseRhs = BaseLhs,
        typename TypesRhs = TypesLhs,
        typename ResultType = void,
        bool symmetric = true
    >
class StaticDispatcher
{
    template<typename SomeLhs>
    static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs, Executor exec, TL::EmptyType)
    {return exec.OnError(lhs, rhs);}
    
    template<typename SomeLhs, typename Head, typename... Tail>
    static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs, Executor exec, TL::TypeList<Head, Tail...>)
    {
        if (Head* p = dynamic_cast<Head*>(&rhs))
        {
            Int2Type<
                (symmetric &&
                    (int(TL::IndexOf<TypesRhs, Head>::value) <
                        int(TL::IndexOf<TypesLhs, SomeLhs>::value)
                    )
                )> i2t;
            
            typedef detail::InvocationTraits<SomeLhs, Head, Executor, ResultType> CallTraits;
            
            return CallTraits::DoDispatch(lhs, *p, exec, i2t);
        }
        return DispatchRhs(lhs, rhs, exec, typename TL::TypeList<Head, Tail...>::Tail());
    }
    
    static ResultType DispatchLhs(BaseLhs& lhs, BaseRhs& rhs, Executor exec, TL::EmptyType)
    {return exec.OnError(lhs, rhs);}
    
    template<typename Head, typename... Tail>
    static ResultType DispatchLhs(BaseLhs& lhs, BaseRhs& rhs, Executor exec, TL::TypeList<Head, Tail...>)
    {
        if (Head* p = dynamic_cast<Head*>(&lhs))
        {
            return DispatchRhs(*p, rhs, exec, TypesRhs());
        }
        return DispatchLhs(lhs, rhs, exec, typename TL::TypeList<Head, Tail...>::Tail());
    }

public:
    static ResultType Go(BaseLhs& lhs, BaseRhs& rhs, Executor exec)
    {return DispatchLhs(lhs, rhs, exec, TypesLhs());}
};

// BasicDispatcher
// Implements a logarithmic double dispatcher for functors (or functions)
// NOTE: Doesn't offer automated casts or symmetry
template
    <
        typename BaseLhs,
        typename BaseRhs = BaseLhs,
        typename ResultType = void,
        typename CallbackType = ResultType (*)(BaseLhs&, BaseRhs&)
    >
class BasicDispatcher
{
    typedef std::pair<TypeInfo, TypeInfo> KeyType;
    typedef CallbackType MappedType;
    typedef AssocVector<KeyType, MappedType> MapType;
    MapType m_callbackMap;
    
    void DoAdd(TypeInfo, TypeInfo, CallbackType);
    bool DoRemove(TypeInfo, TypeInfo);

public:
    template<typename SomeLhs, typename SomeRhs>
    void Add(CallbackType fun)
    {DoAdd(typeid(SomeLhs), typeid(SomeRhs), fun);}
    
    template<typename SomeLhs, typename SomeRhs>
    bool Remove()
    {return DoRemove(typeid(SomeLhs), typeid(SomeRhs));}
    
    ResultType Go(BaseLhs&, BaseRhs&);
};

// Non-inline just for reduce compile time
template<typename BL, typename BR, typename R, typename CT>
void BasicDispatcher<BL, BR, R, CT>::DoAdd(TypeInfo lhs, TypeInfo rhs, CT fun)
{m_callbackMap[KeyType(lhs, rhs)] = fun;}

template<typename BL, typename BR, typename R, typename CT>
bool BasicDispatcher<BL, BR, R, CT>::DoRemove(TypeInfo lhs, TypeInfo rhs)
{return m_callbackMap.erase(KeyType(lhs, rhs)) == 1;}

template<typename BL, typename BR, typename R, typename CT>
R BasicDispatcher<BL, BR, R, CT>::Go(BL& lhs, BR& rhs)
{
    typename MapType::key_type k(typeid(lhs), typeid(rhs));
    typename MapType::iterator i = m_callbackMap.find(k);
    if (i == m_callbackMap.end())
    {
        throw std::runtime_error("Function not found");
    }
    return (i->second)(lhs, rhs);
}

// FnDispatchHelper
// Implements trampoline functions and argumment swapping used by FnDispatcher
namespace detail
{
    template
        <
            typename BaseLhs,
            typename BaseRhs,
            typename SomeLhs,
            typename SomeRhs,
            typename ResultType,
            typename CastLhs,
            typename CastRhs,
            ResultType (*Callback)(SomeLhs&, SomeRhs&)
        >
    struct FnDispatcherHelper
    {
        static ResultType Trampoline(BaseLhs& lhs, BaseRhs& rhs)
        {return Callback(CastLhs::Cast(lhs), CastRhs::Cast(rhs));}
        static ResultType TrampolineR(BaseRhs& rhs, BaseLhs& lhs)
        {return Trampoline(lhs, rhs);}
    };
} // namespace detail

// FnDispatcher
// Implements an automatic logarithmic double-dispatcher for functions
// Features automated conversions
template
    <
        typename BaseLhs,
        typename BaseRhs,
        typename ResultType = void,
        template<typename, typename> class CastingPolicy = DynamicCaster,
        template<typename, typename, typename, typename>
            class DispatcherBackend = BasicDispatcher
    >
class FnDispatcher
{
    typedef ResultType (*FunctionType)(BaseLhs&, BaseRhs&);
    DispatcherBackend<BaseLhs, BaseRhs, ResultType, FunctionType> m_backEnd;
    
public:
    template<typename SomeLhs, typename SomeRhs>
    void Add(FunctionType pFun)
    {return m_backEnd.template Add<SomeLhs, SomeRhs>(pFun);}
    
    template<typename SomeLhs, typename SomeRhs, ResultType (*callback)(SomeLhs&, SomeRhs&), bool symmetric = true>
    void Add()
    {
        typedef detail::FnDispatcherHelper
            <
                BaseLhs, BaseRhs, SomeLhs, SomeRhs, ResultType,
                CastingPolicy<SomeLhs, BaseLhs>,
                CastingPolicy<SomeRhs, BaseRhs>,
                callback
            > Local;
        
        Add<SomeLhs, SomeRhs>(&Local::Trampoline);
        if (symmetric)
        {
            Add<SomeRhs, SomeLhs>(&Local::TrampolineR);
        }
    };
    
    ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
    {return m_backEnd.Go(lhs, rhs);}
};

// FunctorDispatcherHelper
// Helper class for FunctorDispatcher
namespace detail
{
    template
        <
            typename BaseLhs,
            typename BaseRhs,
            typename SomeLhs,
            typename SomeRhs,
            typename ResultType,
            typename CastLhs,
            typename CastRhs,
            typename Fun,
            bool     SwapArgs
        >
    class FunctorDispatcherHelper
    {
        Fun m_fun;
        ResultType Fire(BaseLhs& lhs, BaseRhs& rhs, Int2Type<0>)
        {return m_fun(CastLhs::Cast(lhs), CastRhs::Cast(rhs));}
        ResultType Fire(BaseRhs& rhs, BaseLhs& lhs, Int2Type<1>)
        {return m_fun(CastLhs::Cast(lhs), CastRhs::Cast(rhs));}

    public:
        FunctorDispatcherHelper(const Fun& fun) :m_fun(fun) {}
        
        ResultType operator()(BaseLhs& lhs, BaseRhs& rhs)
        {return Fire(lhs, rhs, Int2Type<SwapArgs>());}
    };
} // namespace detail

// FunctorDispatcher
// Implements a logarithmic double-dispatcher for functors
// Features automated casting
template
    <
        typename BaseLhs,
        typename BaseRhs = BaseLhs,
        typename ResultType = void,
        template<typename, typename> class CastingPolicy = DynamicCaster,
        template<typename, typename, typename, typename>
            class DispatcherBackend = BasicDispatcher
    >
class FunctorDispatcher
{
    typedef TL::TypeList<BaseLhs&, BaseRhs&> ArgsList;
    typedef Functor<ResultType, BaseLhs&, BaseRhs&> FunctorType;
    
    DispatcherBackend<BaseLhs, BaseRhs, ResultType, FunctorType> m_backEnd;

public:
    template<typename SomeLhs, typename SomeRhs, bool symmetric = true, typename Fun>
    void Add(const Fun& fun)
    {
        typedef detail::FunctorDispatcherHelper
            <
                BaseLhs, BaseRhs, SomeLhs, SomeRhs, ResultType,
                CastingPolicy<SomeLhs, BaseLhs>,
                CastingPolicy<SomeRhs, BaseRhs>,
                Fun, false
            > Adapter;
        
        m_backEnd.template Add<SomeLhs, SomeRhs>(FunctorType(Adapter(fun)));
        
        if (symmetric)
        {
            // NOTE: BaseLhs must equal BaseRhs for symmetric
            typedef detail::FunctorDispatcherHelper
                <
                    BaseLhs, BaseRhs, SomeLhs, SomeRhs, ResultType,
                    CastingPolicy<SomeLhs, BaseLhs>,
                    CastingPolicy<SomeRhs, BaseRhs>,
                    Fun, true
                > AdapterR;
            
            m_backEnd.template Add<SomeRhs, SomeLhs>(FunctorType(AdapterR(fun)));
        }
    }
    
    template<typename SomeLhs, typename SomeRhs>
    void Remove()
    {m_backEnd.template Remove<SomeLhs, SomeRhs>();}
    
    ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
    {return m_backEnd.Go(lhs, rhs);}
};

#endif //MULTIMETHODS_DISPATCHER_H
