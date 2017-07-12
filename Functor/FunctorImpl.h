//
// Created by lee on 17/6/23.
//

#ifndef FUNCTOR_FUNCTORIMPL_H
#define FUNCTOR_FUNCTORIMPL_H

#include "../TypeList/TypeList.h"
#include "Functor.h"

template<typename R, typename... P> class Functor;

namespace detail
{
    template<typename Func> struct FunctionTraits;
    template<typename R, typename... P>
    struct FunctionTraits<Functor<R, P...>>
    {
        typedef R                  result_type;
        typedef TL::TypeList<P...> param_list_type;
        template<int ind> using    param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;
    };
    template<typename R, typename... P>
    struct FunctionTraits<R (*)(P...)>
    {
        typedef R                  result_type;
        typedef TL::TypeList<P...> param_list_type;
        template<int ind> using    param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;
    };
    template<typename R, typename M, typename... P>
    struct FunctionTraits<R (M::*)(P...)>
    {
        typedef R                  result_type;
        typedef M                  object_type;
        typedef TL::TypeList<P...> param_list_type;
        template<int ind> using    param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;
    };
}

template<typename R, typename... P>
class FunctorImpl
{
public:
    // typedefs
    typedef R                    result_type;
    typedef FunctorImpl<R, P...> class_type;

public:
    // functions
    virtual R operator()(P...) = 0;
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() = default;
};

#endif //FUNCTOR_FUNCTORIMPL_H
